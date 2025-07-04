/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** AxolotlPlayerModel.hpp
*/

#pragma once

#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class AxolotlPlayerModel : public APlayerModel {
                public:
                    AxolotlPlayerModel(const int &id);
                    ~AxolotlPlayerModel() override = default;

                    void init() override;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
