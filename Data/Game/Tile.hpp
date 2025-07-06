/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Tile.hpp
*/

#pragma once

#include "ResourceContainer.hpp"

namespace zappy {
    namespace game {
        /**
         * @class Tile
         * @brief Represents a tile in the game map, inheriting resource storage capabilities.
         */
        class Tile : public ResourceContainer {
            public:
                /**
                 * @brief Default constructor.
                 */
                Tile() = default;

                /**
                 * @brief Copy constructor.
                 * @param other Tile to copy from.
                 */
                Tile(const Tile &other) = default;

                /**
                 * @brief Destructor.
                 */
                ~Tile() = default;
        };
    }
}
