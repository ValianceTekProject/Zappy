/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Skybox.cpp
*/

#include "Skybox.hpp"
#include "AssetPaths.hpp"

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

void zappy::gui::raylib::Skybox::render() const
{
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(this->_model, { 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

void zappy::gui::raylib::Skybox::renderInfo() const
{
    DrawText(TextFormat("Skybox image: %s", GetFileName(this->_skyboxFileName.c_str())),
    10, GetScreenHeight() - 20, 10, BLACK);
}

void zappy::gui::raylib::Skybox::unload()
{
    if (this->_model.materialCount > 0)
        UnloadShader(this->_model.materials[0].shader);
    UnloadTexture(this->_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(this->_model);
}
