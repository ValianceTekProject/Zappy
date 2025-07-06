/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ServerError copy.hpp
*/

#pragma once

#include "AError.hpp"

namespace zappy {
    namespace server {

        /**
         * @brief Exception class for server-related errors.
         * 
         * Inherits from AError to provide detailed error messages and context
         * related to server-side issues (e.g. socket operations, connection handling).
         */
        class ServerError : public AError
        {
            public:
                /**
                 * @brief Construct a new ServerError object.
                 * 
                 * @param msg The error message.
                 * @param where The location or context where the error occurred.
                 */
                ServerError(const std::string &msg, const std::string &where)
                    noexcept : AError(msg, where) {};
        };

    } // namespace server
} // namespace zappy
