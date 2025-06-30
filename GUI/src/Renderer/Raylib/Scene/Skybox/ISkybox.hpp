/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** ISkybox.hpp
*/

#pragma once

#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <string>
#include <iostream>

#if defined(PLATFORM_DESKTOP)
    #define GLSL_VERSION 330
#else
    #define GLSL_VERSION 100
#endif

namespace zappy {
    namespace gui {
        namespace raylib {
            class ISkybox {
                public:
                    virtual ~ISkybox() = default;

                    virtual bool load() = 0;

                    virtual void update() = 0;

                    virtual void render() const {}

                    virtual void renderInfo() const = 0;

                    virtual void unload() = 0;
            };
        }
    }
}