/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** Skybox.hpp
*/

#pragma once

#include <string>
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

                    TextureCubemap genCubemapFromHDR(const Texture2D &panorama, Shader shader, int size, int format) {
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
            };
        }
    }
}
