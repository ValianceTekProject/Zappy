/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** MinecraftResourceModel.hpp
*/

#pragma once

#include "AResourceModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class MinecraftResourceModel : public AResourceModel {
                public:
                    MinecraftResourceModel(const int &id, const zappy::game::Resource &resourceType);
                    ~MinecraftResourceModel() override = default;

                    void init() override;
            };
        }
    }
}
