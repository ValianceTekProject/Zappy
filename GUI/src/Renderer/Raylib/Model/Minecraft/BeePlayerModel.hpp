/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** BeePlayerModel.hpp
*/

#pragma once

#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class BeePlayerModel : public APlayerModel {
                public:
                    BeePlayerModel(const int &id);
                    ~BeePlayerModel() override = default;

                    void init() override;

                    void idle() override;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
