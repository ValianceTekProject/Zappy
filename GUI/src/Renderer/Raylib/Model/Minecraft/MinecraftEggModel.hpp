/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** MinecraftEggModel.hpp
*/

#pragma once

#include "AEggModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class MinecraftEggModel : public AEggModel {
                public:
                    MinecraftEggModel(const int &id);
                    ~MinecraftEggModel() override = default;

                    void init() override;
            };
        }
    }
}
