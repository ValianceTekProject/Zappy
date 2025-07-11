/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ABroadcastEffect.hpp
*/

#pragma once

#include "AEffect.hpp"

#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            class ABroadcastEffect : public AEffect
            {
                public:
                    ABroadcastEffect(
                        const int &playerId,
                        const float &duration,
                        const Color &color
                    ) : AEffect::AEffect(playerId, duration, color) {}
                    virtual ~ABroadcastEffect() override = default;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
