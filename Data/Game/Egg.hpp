/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Egg.hpp
*/

#pragma once

#include <iostream>

namespace zappy {
    namespace game {

        /**
         * @brief Represents an Egg in the game world.
         * 
         * An egg has a unique ID, a position on the map (x, y),
         * and optionally a father ID indicating which player created it.
         */
        class Egg
        {
            public:
                /// X coordinate of the egg's position on the map.
                int x;

                /// Y coordinate of the egg's position on the map.
                int y;

                /**
                 * @brief Construct a new Egg object with a father ID.
                 * 
                 * @param id Unique ID of the egg.
                 * @param fatherId ID of the player who spawned the egg.
                 * @param x X coordinate.
                 * @param y Y coordinate.
                 */
                explicit Egg(const int &id, const int &fatherId, const int &x, const int &y) :
                    x(x), y(y), _id(id), _fatherId(fatherId)
                {}

                /**
                 * @brief Construct a new Egg object without a father ID.
                 * 
                 * @param id Unique ID of the egg.
                 * @param x X coordinate.
                 * @param y Y coordinate.
                 */
                explicit Egg(const int &id, const int &x, const int &y) :
                    Egg(id, -1, x, y)
                {}
                /// Default destructor.
                ~Egg() = default;

                /**
                 * @brief Get the egg's unique ID.
                 * 
                 * @return int The egg's ID.
                 */
                int getId() const { return this->_id; }

                /**
                 * @brief Get the father (creator) ID.
                 * 
                 * @return int The ID of the player that created the egg.
                 */
                int getFatherId() const { return this->_fatherId; }

                /**
                 * @brief Set the father (creator) ID.
                 * 
                 * @param id The new father ID.
                 */
                void setFatherId(const int &id) {this->_fatherId = id;}

            private:
                /// Unique identifier of the egg.
                int _id;

                /// ID of the player who spawned the egg (-1 if none).
                int _fatherId;
        };

    } // namespace game
} // namespace zappy

