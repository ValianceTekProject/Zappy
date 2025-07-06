/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** NetworkError.hpp
*/

#pragma once

#include "AError.hpp"

namespace zappy {
    namespace network {

        /**
         * @brief Exception class for network-related errors.
         * 
         * Inherits from AError to provide detailed error context
         * for failures occurring in the networking layer.
         */
        class NetworkError : public AError
        {
            public:
                /**
                 * @brief Construct a new NetworkError object.
                 * 
                 * @param msg The error message.
                 * @param where The location or context where the error occurred.
                 */
                NetworkError(const std::string &msg, const std::string &where)
                    noexcept : AError(msg, where) {};
        };

    } // namespace network
} // namespace zappy

