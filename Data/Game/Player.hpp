/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Player.hpp
*/

#pragma once

#include "Egg.hpp"
#include "Orientation.hpp"
#include "Inventory.hpp"

namespace zappy {
    namespace game {
        /**
         * @class Player
         * @brief Represents a player in the game, inherits from Egg.
         */
        class Player : public Egg
        {
            public:
                size_t level;           ///< Player's current level
                Orientation orientation;///< Player's current orientation
                std::string teamName;   ///< Name of the team the player belongs to

                /**
                 * @brief Constructs a new Player object.
                 * 
                 * @param id Player's unique identifier.
                 * @param x Initial x-coordinate on the map.
                 * @param y Initial y-coordinate on the map.
                 * @param orientation Initial orientation, defaults to NORTH.
                 * @param level Initial level, defaults to 1.
                 */
                explicit Player(
                    int id,
                    int x,
                    int y,
                    Orientation orientation = Orientation::NORTH,
                    size_t level = 1
                );
                
                ~Player() = default;

                /**
                 * @brief Sets the player to praying state.
                 */
                void pray() { this->_isPraying = true; }

                /**
                 * @brief Checks if the player is currently praying.
                 * 
                 * @return true If the player is praying.
                 * @return false Otherwise.
                 */
                bool isPraying() const { return this->_isPraying; }

                /**
                 * @brief Stops the praying state of the player.
                 */
                void stopPraying() { this->_isPraying = false; }

                /**
                 * @brief Turns the player to the left (counter-clockwise).
                 */
                void lookLeft() { this->orientation--; }

                /**
                 * @brief Turns the player to the right (clockwise).
                 */
                void lookRight() { this->orientation++; }

                /**
                 * @brief Moves the player one step forward based on its orientation.
                 * 
                 * @param width Width of the game map (for wrapping).
                 * @param height Height of the game map (for wrapping).
                 */
                void stepForward(int width, int height);

                /**
                 * @brief Ejects the player from a position based on another player's orientation.
                 * 
                 * @param playerOrientation Orientation of the player performing the eject.
                 * @param width Width of the game map (for wrapping).
                 * @param height Height of the game map (for wrapping).
                 */
                void ejectFrom(Orientation playerOrientation, int width, int height);

                /**
                 * @brief Sets the player's inventory.
                 * 
                 * @param inventory The inventory to set.
                 */
                void setInventory(const Inventory &inventory) { this->_inventory = inventory; }

                /**
                 * @brief Gets the player's current inventory.
                 * 
                 * @return const Inventory& Reference to the player's inventory.
                 */
                const Inventory &getInventory() const { return this->_inventory; }

                /**
                 * @brief Adds resources to the player's inventory.
                 * 
                 * @param resource The type of resource to collect.
                 * @param quantity Quantity of resource to collect (default is 1).
                 */
                void collectRessource(Resource resource, size_t quantity = 1);

                /**
                 * @brief Removes resources from the player's inventory.
                 * 
                 * @param resource The type of resource to drop.
                 * @param quantity Quantity of resource to drop (default is 1).
                 */
                void dropRessource(Resource resource, size_t quantity = 1);

            private:
                bool _isPraying;       ///< Flag indicating if the player is praying
                Inventory _inventory;  ///< Player's inventory data
        };
    }
}
