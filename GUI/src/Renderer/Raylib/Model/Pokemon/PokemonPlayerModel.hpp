/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonPlayerModel.hpp
*/

#pragma once

#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class PokemonPlayerModel : public APlayerModel {
                public:
                    PokemonPlayerModel(const int &id);
                    ~PokemonPlayerModel() override = default;

                    void init() override;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
