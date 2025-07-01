/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonEggModel.hpp
*/

#pragma once

#include "AEggModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class PokemonEggModel : public AEggModel {
                public:
                    PokemonEggModel(const int &id);
                    ~PokemonEggModel() override = default;

                    void init() override;
            };
        }
    }
}
