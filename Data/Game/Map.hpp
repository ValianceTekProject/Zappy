/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Map.hpp
*/

#pragma once

#include "Tile.hpp"
#include <vector>

namespace zappy {
    namespace game {
        /**
         * @brief Represents the game map composed of tiles.
         * 
         * The map is a 2D grid of Tile objects with given width and height.
         */
        class Map
        {
            public:
                /**
                 * @brief Construct a new Map object with specified dimensions.
                 * 
                 * @param width Width of the map (number of tiles horizontally).
                 * @param height Height of the map (number of tiles vertically).
                 */
                Map(const size_t &width = 0, const size_t &height = 0) { _init(width, height); }

                /// Default destructor.
                ~Map() = default;

                /**
                 * @brief Get the width of the map.
                 * 
                 * @return const size_t& The width of the map.
                 */
                const size_t &getWidth() const { return this->_width; }

                /**
                 * @brief Get the height of the map.
                 * 
                 * @return const size_t& The height of the map.
                 */
                const size_t &getHeight() const { return this->_height; }

                /**
                 * @brief Access a tile at given coordinates (non-const).
                 * 
                 * @param x X coordinate (horizontal).
                 * @param y Y coordinate (vertical).
                 * @return Tile& Reference to the tile.
                 */
                Tile &getTile(const size_t &x, const size_t &y) { return this->_map[x][y]; }

                /**
                 * @brief Access a tile at given coordinates (const).
                 * 
                 * @param x X coordinate (horizontal).
                 * @param y Y coordinate (vertical).
                 * @return const Tile& Const reference to the tile.
                 */
                const Tile &getTile(const size_t &x, const size_t &y) const { return this->_map[x][y]; }

                /**
                 * @brief Get the total quantity of a given resource on the map.
                 * 
                 * @param type Resource type to count.
                 * @return size_t Total quantity of the resource.
                 */
                size_t getResourceQuantity(const Resource &type) const;

                /**
                 * @brief Clear the resources/items on a specific tile.
                 * 
                 * @param x X coordinate of the tile.
                 * @param y Y coordinate of the tile.
                 */
                void clearTile(const size_t &x, const size_t &y);

                /**
                 * @brief Clear all tiles on the map.
                 */
                void clear();

                /**
                 * @brief Replace a tile at the specified coordinates.
                 * 
                 * @param x X coordinate of the tile.
                 * @param y Y coordinate of the tile.
                 * @param tile The new Tile to set.
                 */
                void setTile(const size_t &x, const size_t &y, const Tile &tile);

            protected:
                /**
                 * @brief Initialize the map with given dimensions.
                 * 
                 * @param width Width of the map.
                 * @param height Height of the map.
                 */
                void _init(const size_t &width, const size_t &height);

                /// Width of the map.
                size_t _width;

                /// Height of the map.
                size_t _height;

                /// 2D vector holding the tiles.
                std::vector<std::vector<Tile>> _map;
        };
    } // namespace game
} // namespace zappy

