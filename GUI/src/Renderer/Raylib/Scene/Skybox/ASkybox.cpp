/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** ASkybox.cpp
*/

#include "ASkybox.hpp"

zappy::gui::raylib::ASkybox::ASkybox(const std::string &imagePath,
                                        const std::string &skyboxVS,
                                        const std::string &skyboxFS,
                                        const std::string &cubemapVS,
                                        const std::string &cubemapFS)
    : _imagePath(imagePath),
    _skyboxVS(skyboxVS),
    _skyboxFS(skyboxFS),
    _cubemapVS(cubemapVS),
    _cubemapFS(cubemapFS)
{}

bool zappy::gui::raylib::ASkybox::load()
{
    _mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    _model = LoadModelFromMesh(_mesh);

    if (_model.materialCount == 0) {
        std::cerr << "[Skybox] Failed to load model materials\n";
        return false;
    }

    _model.materials[0].shader = LoadShader(_skyboxVS.c_str(), _skyboxFS.c_str());

    int cubemapMapType = MATERIAL_MAP_CUBEMAP;
    SetShaderValue(_model.materials[0].shader,
                GetShaderLocation(_model.materials[0].shader, "environmentMap"),
                &cubemapMapType, SHADER_UNIFORM_INT);

    int doGammaValue = 0;
    SetShaderValue(_model.materials[0].shader,
                GetShaderLocation(_model.materials[0].shader, "doGamma"),
                &doGammaValue, SHADER_UNIFORM_INT);

    int vflippedValue = 0;
    SetShaderValue(_model.materials[0].shader,
                GetShaderLocation(_model.materials[0].shader, "vflipped"),
                &vflippedValue, SHADER_UNIFORM_INT);

    int equirectangularMap = 0;
    SetShaderValue(_shaderCubemap,
                GetShaderLocation(_shaderCubemap, "equirectangularMap"),
                &equirectangularMap, SHADER_UNIFORM_INT);

    _shaderCubemap = LoadShader(TextFormat(_cubemapVS.c_str(), GLSL_VERSION),
                                TextFormat(_cubemapFS.c_str(), GLSL_VERSION));

    if (_shaderCubemap.id == 0) {
        std::cerr << "[Skybox] Failed to load cubemap shader\n";
        return false;
    }

    _skyboxFileName = _imagePath;
    Image img = LoadImage(_imagePath.c_str());
    _model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);

    return true;
}

void zappy::gui::raylib::ASkybox::update()
{
    if (IsFileDropped()) {
        FilePathList droppedFiles = LoadDroppedFiles();
        if (droppedFiles.count == 1 &&
            IsFileExtension(droppedFiles.paths[0], ".png;.jpg;.hdr;.bmp;.tga")) {

            UnloadTexture(_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);

            Image img = LoadImage(droppedFiles.paths[0]);
            _model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
            UnloadImage(img);

            _skyboxFileName = droppedFiles.paths[0];
        }
        UnloadDroppedFiles(droppedFiles);
    }
}

void zappy::gui::raylib::ASkybox::render() const
{
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(_model, { 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

void zappy::gui::raylib::ASkybox::renderInfo() const
{
    DrawText(TextFormat("Skybox image: %s", GetFileName(_skyboxFileName.c_str())),
    10, GetScreenHeight() - 20, 10, BLACK);
}

void zappy::gui::raylib::ASkybox::unload()
{
    if (_model.materialCount > 0)
        UnloadShader(_model.materials[0].shader);
    UnloadTexture(_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(_model);
}