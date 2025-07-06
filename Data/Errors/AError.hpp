/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** AError
*/

#pragma once

#include "IError.hpp"

namespace zappy {

    /**
     * @brief Abstract base class for custom error handling.
     * 
     * Inherits from IError and provides basic functionality
     * for storing error messages and their origin.
     */
    class AError : public IError
    {
        public:
            /**
             * @brief Construct a new AError object.
             * 
             * @param msg The error message.
             * @param where The location or context where the error occurred.
             */
            AError(const std::string &msg, const std::string &where) noexcept;

            /**
             * @brief Retrieve the error message.
             * 
             * @return const char* The error message as a C-string.
             */
            const char *what() const noexcept;

            /**
             * @brief Retrieve the location where the error occurred.
             * 
             * @return const char* The error location as a C-string.
             */
            const char *where() const noexcept;

        private:
            /**
             * @brief The error message.
             */
            std::string _msg;

            /**
             * @brief The location or context where the error occurred.
             */
            std::string _where;
    };

} // namespace zappy
