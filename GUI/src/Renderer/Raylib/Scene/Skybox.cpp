/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Skybox.cpp
*/

#include "Skybox.hpp"
#include "AssetPaths.hpp"

zappy::gui::raylib::Skybox::Skybox(const std::string &imagePath)
    : _imagePath(imagePath)
{}

bool zappy::gui::raylib::Skybox::load()
{
    _mesh = GenMeshCube(1.0f, 1.0f, 1.0f);
    _model = LoadModelFromMesh(_mesh);

    if (_model.materialCount == 0)
        return false;

    _model.materials[0].shader = LoadShader(zappy::gui::raylib::assets::BASIC_SKYBOX_VS_PATH.c_str(), zappy::gui::raylib::assets::BASIC_SKYBOX_FS_PATH.c_str());

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

    _shaderCubemap = LoadShader(TextFormat(zappy::gui::raylib::assets::BASIC_SKYBOX_CUBEMAP_VS_PATH.c_str(), GLSL_VERSION),
                                TextFormat(zappy::gui::raylib::assets::BASIC_SKYBOX_CUBEMAP_FS_PATH.c_str(), GLSL_VERSION));

    if (_shaderCubemap.id == 0)
        return false;

    _skyboxFileName = _imagePath;
    Image img = LoadImage(_imagePath.c_str());
    _model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(img, CUBEMAP_LAYOUT_AUTO_DETECT);
    UnloadImage(img);

    return true;
}

void zappy::gui::raylib::Skybox::update()
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

void zappy::gui::raylib::Skybox::render() const
{
    rlDisableBackfaceCulling();
    rlDisableDepthMask();
    DrawModel(_model, { 0, 0, 0 }, 1.0f, WHITE);
    rlEnableBackfaceCulling();
    rlEnableDepthMask();
}

void zappy::gui::raylib::Skybox::renderInfo() const
{
    DrawText(TextFormat("Skybox image: %s", GetFileName(_skyboxFileName.c_str())),
    10, GetScreenHeight() - 20, 10, BLACK);
}

void zappy::gui::raylib::Skybox::unload()
{
    if (_model.materialCount > 0)
        UnloadShader(_model.materials[0].shader);
    UnloadTexture(_model.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    UnloadModel(_model);
}