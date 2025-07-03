/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** FlyingPikachuPlayerModel.hpp
*/

#pragma once

#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class FlyingPikachuPlayerModel : public APlayerModel {
                public:
                    FlyingPikachuPlayerModel(const int &id);
                    ~FlyingPikachuPlayerModel() override = default;

                    void init() override;

                    void idle() override;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
