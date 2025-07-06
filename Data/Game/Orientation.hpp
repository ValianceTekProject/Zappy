/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Orientation.hpp
*/

#pragma once

#include <iostream>
#include <array>

namespace zappy {
    namespace game {
        /**
         * @enum Orientation
         * @brief Represents the four cardinal directions.
         */
        enum class Orientation {
            NORTH, ///< North direction
            EAST,  ///< East direction
            SOUTH, ///< South direction
            WEST   ///< West direction
        };

        /**
         * @brief Adds an integer offset to an Orientation, cycling through directions.
         * 
         * @param orientation The starting orientation.
         * @param offset The offset to add.
         * @return Orientation The resulting orientation.
         */
        Orientation operator+(const Orientation &orientation, int offset) noexcept;

        /**
         * @brief Adds an integer offset to an Orientation in place.
         * 
         * @param orientation The orientation to modify.
         * @param offset The offset to add.
         * @return Orientation& Reference to the modified orientation.
         */
        Orientation &operator+=(Orientation &orientation, int offset) noexcept;

        /**
         * @brief Returns the opposite orientation (180 degrees turn).
         * 
         * @param orientation The orientation to invert.
         * @return Orientation The opposite orientation.
         */
        Orientation operator-(const Orientation &orientation) noexcept;

        /**
         * @brief Subtracts an integer offset from an Orientation, cycling through directions.
         * 
         * @param orientation The starting orientation.
         * @param offset The offset to subtract.
         * @return Orientation The resulting orientation.
         */
        Orientation operator-(const Orientation &orientation, int offset) noexcept;

        /**
         * @brief Subtracts an integer offset from an Orientation in place.
         * 
         * @param orientation The orientation to modify.
         * @param offset The offset to subtract.
         * @return Orientation& Reference to the modified orientation.
         */
        Orientation &operator-=(Orientation &orientation, int offset) noexcept;

        /**
         * @brief Prefix increment operator to rotate orientation clockwise.
         * 
         * @param o The orientation to increment.
         * @return Orientation& Reference to the incremented orientation.
         */
        Orientation &operator++(Orientation &o) noexcept;

        /**
         * @brief Postfix increment operator to rotate orientation clockwise.
         * 
         * @param o The orientation to increment.
         * @param int Dummy parameter to distinguish postfix.
         * @return Orientation The original orientation before increment.
         */
        Orientation operator++(Orientation &o, int) noexcept;

        /**
         * @brief Prefix decrement operator to rotate orientation counter-clockwise.
         * 
         * @param o The orientation to decrement.
         * @return Orientation& Reference to the decremented orientation.
         */
        Orientation &operator--(Orientation &o) noexcept;

        /**
         * @brief Postfix decrement operator to rotate orientation counter-clockwise.
         * 
         * @param o The orientation to decrement.
         * @param int Dummy parameter to distinguish postfix.
         * @return Orientation The original orientation before decrement.
         */
        Orientation operator--(Orientation &o, int) noexcept;

        /**
         * @brief String abbreviations for orientations.
         */
        constexpr std::array<const char*, 4> orientationStrings = {
            "N", "E", "S", "W"
        };

        /**
         * @brief Full string names for orientations.
         */
        constexpr std::array<const char*, 4> orientationFullStrings = {
            "North", "East", "South", "West"
        };

        /**
         * @brief Converts a string to an Orientation enum value.
         * 
         * @param orientation The string to convert (e.g., "N", "East").
         * @return Orientation The corresponding Orientation.
         */
        Orientation convertOrientation(const std::string &orientation);

        /**
         * @brief Inserts an Orientation as string into an output stream.
         * 
         * @param os The output stream.
         * @param orientation The Orientation to output.
         * @return std::ostream& Reference to the output stream.
         */
        std::ostream &operator<<(std::ostream &os, const Orientation &orientation);
    }
}

#include "Orientation.ipp"
