/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** IError
*/

#pragma once

#include <iostream>
#include <stdexcept>

namespace zappy {

    /**
     * @brief Interface for custom error handling in the Zappy project.
     * 
     * Inherits from std::exception and adds a method to retrieve
     * the context or location where the error occurred.
     */
    class IError : public std::exception
    {
        public:
            /**
             * @brief Returns the error message.
             * 
             * @return A C-style string describing the error.
             */
            virtual const char *what() const noexcept override = 0;

            /**
             * @brief Returns the location or context where the error occurred.
             * 
             * @return A C-style string indicating the source of the error.
             */
            virtual const char *where() const noexcept = 0;
    };

} // namespace zappy

