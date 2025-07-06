/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** BulbasaurPlayerModel.hpp
*/

#pragma once

#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class BulbasaurPlayerModel : public APlayerModel {
                public:
                    BulbasaurPlayerModel(const int &id);
                    ~BulbasaurPlayerModel() override = default;

                    void init() override;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
