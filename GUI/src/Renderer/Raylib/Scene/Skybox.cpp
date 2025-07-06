/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Skybox.cpp
*/

#include "Skybox.hpp"
#include "AssetPaths.hpp"

/// @brief Initialise le skybox avec l'image donnée.
/// @param imagePath Chemin de l'image HDR utilisée pour le skybox.
/// @return true si le skybox est correctement initialisé, false sinon.
bool zappy::gui::raylib::Skybox::init(const std::string &imagePath)
{
    this->_mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    this->_model = LoadModelFromMesh(this->_mesh);

    if (this->_model.materialCount == 0)
        return false;

    this->_shaderCubemap = LoadShader(
        TextFormat(assets::BASIC_SKYBOX_CUBEMAP_VS_PATH.c_str(), GLSL_VERSION),
        TextFormat(assets::BASIC_SKYBOX_CUBEMAP_FS_PATH.c_str(), GLSL_VERSION)
    );

    if (this->_shaderCubemap.id == 0)
        return false;

    int equirectangularMap = 0;
    SetShaderValue(
        this->_shaderCubemap,
        GetShaderLocation(this->_shaderCubemap, "equirectangularMap"),
        &equirectangularMap, SHADER_UNIFORM_INT
    );

    Shader baseShader = LoadShader(
        assets::BASIC_SKYBOX_VS_PATH.c_str(),
        assets::BASIC_SKYBOX_FS_PATH.c_str()
    );
    this->_model.materials[0].shader = baseShader;

    int cubemapMapType = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(baseShader,
        GetShaderLocation(baseShader, "environmentMap"),
        &cubemapMapType, SHADER_UNIFORM_INT
    );
    int doGammaValue = 0;
    SetShaderValue(baseShader,
        GetShaderLocation(baseShader, "doGamma"),
        &doGammaValue, SHADER_UNIFORM_INT
    );
    int vflippedValue = 0;
    SetShaderValue(baseShader,
        GetShaderLocation(baseShader, "vflipped"),
        &vflippedValue, SHADER_UNIFORM_INT
    );

    Image img = LoadImage(imagePath.c_str());
    TextureCubemap cubemap = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);

    this->_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = cubemap;

    this->_skyboxFileName = imagePath;
    return true;
}

/// @brief Met à jour le skybox. Si un nouveau fichier est déposé, charge l'image en tant que nouveau skybox.
void zappy::gui::raylib::Skybox::update()
{
    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        if (droppedFiles.count == 1 &&
            IsFileExtension(droppedFiles.paths[0], ".png;.jpg;.hdr;.bmp;.tga")) {

            UnloadTexture(this->_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

            Image img = LoadImage(droppedFiles.paths[0]);
            this->_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
            UnloadImage(img);

            this->_skyboxFileName = droppedFiles.paths[0];
        }
        UnloadDroppedFiles(droppedFiles);
    }
}

/// @brief Affiche le skybox dans la scène.
void zappy::gui::raylib::Skybox::render() const
{
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(this->_model, { 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

/// @brief Affiche des informations sur le fichier utilisé pour le skybox (en bas de l'écran).
void zappy::gui::raylib::Skybox::renderInfo() const
{
    DrawText(TextFormat("Skybox image: %s", GetFileName(this->_skyboxFileName.c_str())),
    10, GetScreenHeight() - 20, 10, BLACK);
}

/// @brief Libère toutes les ressources associées au skybox (shader, textures, modèle).
void zappy::gui::raylib::Skybox::unload()
{
    if (this->_model.materialCount > 0)
        UnloadShader(this->_model.materials[0].shader);
    UnloadTexture(this->_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(this->_model);
}

/**
 * @brief Génère un cubemap à partir d'une texture HDR.
 * 
 * @param panorama Texture2D contenant l'image HDR.
 * @param shader Shader utilisé pour le rendu.
 * @param size Taille du cubemap.
 * @param format Format de la texture.
 * @return TextureCubemap généré.
 */
TextureCubemap zappy::gui::raylib::Skybox::_genCubemapFromHDR(const Texture2D &panorama, Shader shader, int size, int format) {
    TextureCubemap cubemap = {};

    rlDisableBackfaceCulling();

    unsigned int rbo = rlLoadTextureDepth(size, size, true);
    cubemap.id = rlLoadTextureCubemap(nullptr, size, format, 1);

    unsigned int fbo = rlLoadFramebuffer();
    rlFramebufferAttach(fbo, rbo, RL_ATTACHMENT_DEPTH, RL_ATTACHMENT_RENDERBUFFER, 0);
    rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X, 0);

    rlEnableShader(shader.id);
    Matrix matProj = MatrixPerspective(90.0f * DEG2RAD, 1.0f, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_PROJECTION], matProj);

    Matrix fboViews[6] = {
        MatrixLookAt({0,0,0}, {1,0,0}, {0,-1,0}),
        MatrixLookAt({0,0,0}, {-1,0,0}, {0,-1,0}),
        MatrixLookAt({0,0,0}, {0,1,0}, {0,0,1}),
        MatrixLookAt({0,0,0}, {0,-1,0}, {0,0,-1}),
        MatrixLookAt({0,0,0}, {0,0,1}, {0,-1,0}),
        MatrixLookAt({0,0,0}, {0,0,-1}, {0,-1,0})
    };

    rlViewport(0, 0, size, size);
    rlActiveTextureSlot(0);
    rlEnableTexture(panorama.id);

    for (int i = 0; i < 6; ++i) {
        rlSetUniformMatrix(shader.locs[SHADER_LOC_MATRIX_VIEW], fboViews[i]);
        rlFramebufferAttach(fbo, cubemap.id, RL_ATTACHMENT_COLOR_CHANNEL0, RL_ATTACHMENT_CUBEMAP_POSITIVE_X + i, 0);
        rlEnableFramebuffer(fbo);
        rlClearScreenBuffers();
        rlLoadDrawCube();
    }

    rlDisableShader();
    rlDisableTexture();
    rlDisableFramebuffer();
    rlUnloadFramebuffer(fbo);

    rlViewport(0, 0, rlGetFramebufferWidth(), rlGetFramebufferHeight());
    rlEnableBackfaceCulling();

    cubemap.width = size;
    cubemap.height = size;
    cubemap.mipmaps = 1;
    cubemap.format = format;

    return cubemap;
}
