/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Inventory.hpp
*/

#pragma once

#include "ResourceContainer.hpp"

namespace zappy {
    namespace game {

        /**
         * @brief Represents an inventory in the game.
         * 
         * Inherits from ResourceContainer to manage a collection of resources
         * (e.g., food, stones). Used by players and tiles to store items.
         */
        class Inventory : public ResourceContainer {
            public:
                /**
                 * @brief Construct a new empty Inventory object.
                 */
                Inventory() = default;

                /**
                 * @brief Copy constructor for Inventory.
                 * 
                 * @param other The inventory to copy.
                 */
                Inventory(const Inventory &other) = default;

                /**
                 * @brief Destroy the Inventory object.
                 */
                ~Inventory() = default;
        };

    } // namespace game
} // namespace zappy
