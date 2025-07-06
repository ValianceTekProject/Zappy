/*
** EPITECH PROJECT, 2025
** B-OOP-400-BDX-4-1-zappy-baptiste.blambert
** File description:
** RendererError
*/

#pragma once

#include "AError.hpp"

namespace zappy {
    namespace gui {

        /**
         * @brief Exception class for GUI rendering-related errors.
         * 
         * Inherits from AError to provide error details when rendering
         * fails or encounters unexpected behavior in the GUI.
         */
        class RendererError : public AError
        {
            public:
                /**
                 * @brief Construct a new RendererError object.
                 * 
                 * @param msg The error message.
                 * @param where The location or context where the error occurred.
                 */
                RendererError(const std::string &msg, const std::string &where)
                    noexcept : AError(msg, where) {};
        };

    } // namespace gui
} // namespace zappy

