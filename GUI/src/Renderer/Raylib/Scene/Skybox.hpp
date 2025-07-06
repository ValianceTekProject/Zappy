/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Skybox.hpp
*/

#pragma once

#include "AssetPaths.hpp"

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

namespace zappy {
    namespace gui {
        namespace raylib {
            class Skybox {
                public:
                    Skybox() = default;
                    ~Skybox() = default;

                    bool init(const std::string &imagePath);

                    void update();

                    void render() const;

                    void renderInfo() const;

                    void unload();

                private:
                    std::string _skyboxFileName;
                    Shader _shaderCubemap = {};
                    Model _model = {};
                    Mesh _mesh = {};

                    TextureCubemap _genCubemapFromHDR(const Texture2D &panorama, Shader shader, int size, int format);
            };
        }
    }
}
