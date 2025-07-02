/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonResourceModel.hpp
*/

#pragma once

#include "AResourceModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class PokemonResourceModel : public AResourceModel {
                public:
                    PokemonResourceModel(const int &id, const zappy::game::Resource &resourceType);
                    ~PokemonResourceModel() override = default;

                    void init() override;
            };
        }
    }
}
