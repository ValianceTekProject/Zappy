/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** GuiError
*/

#pragma once

#include "AError.hpp"

namespace zappy {
    namespace gui {

        /**
         * @brief Exception class for errors specific to the GUI module.
         * 
         * Inherits from AError to provide contextual error information
         * for GUI-related issues.
         */
        class GuiError : public AError
        {
            public:
                /**
                 * @brief Construct a new GuiError object.
                 * 
                 * @param msg The error message.
                 * @param where The location or context where the error occurred.
                 */
                GuiError(const std::string &msg, const std::string &where)
                    noexcept : AError(msg, where) {};
        };

    } // namespace gui
} // namespace zappy

