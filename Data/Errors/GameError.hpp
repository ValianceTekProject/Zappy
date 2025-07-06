/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** GameError
*/

#pragma once

#include "AError.hpp"

namespace zappy {
    namespace game {

        /**
         * @brief Exception class for errors specific to the Game module.
         * 
         * Inherits from AError to provide contextual error information
         * for issues occurring in the game logic.
         */
        class GameError : public AError
        {
            public:
                /**
                 * @brief Construct a new GameError object.
                 * 
                 * @param msg The error message.
                 * @param where The location or context where the error occurred.
                 */
                GameError(const std::string &msg, const std::string &where)
                    noexcept : AError(msg, where) {};
        };

    } // namespace game
} // namespace zappy
