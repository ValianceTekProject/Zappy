/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** ParsingError
*/

#pragma once

#include "AError.hpp"

namespace zappy {

    /**
     * @brief Exception class for errors encountered during parsing.
     * 
     * Inherits from AError to provide detailed error messages and context
     * specific to parsing failures (e.g. config file, arguments).
     */
    class ParsingError : public AError
    {
        public:
            /**
             * @brief Construct a new ParsingError object.
             * 
             * @param msg The error message.
             * @param where The location or context where the error occurred.
             */
            ParsingError(const std::string &msg, const std::string &where)
                noexcept : AError(msg, where) {};
    };

} // namespace zappy
