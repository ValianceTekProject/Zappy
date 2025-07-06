/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Resource.hpp
*/

#pragma once

#include "GameError.hpp"

#include <array>

namespace zappy {
    namespace game {
        /**
         * @enum Resource
         * @brief Enum representing the different resource types in the game.
         */
        enum class Resource: size_t {
            FOOD = 0,       ///< Food resource
            LINEMATE = 1,   ///< Linemate resource
            DERAUMERE = 2,  ///< Deraumere resource
            SIBUR = 3,      ///< Sibur resource
            MENDIANE = 4,   ///< Mendiane resource
            PHIRAS = 5,     ///< Phiras resource
            THYSTAME = 6,   ///< Thystame resource
            RESOURCE_SIZE   ///< Number of resources
        };

        /**
         * @brief Total number of resource types.
         */
        const size_t RESOURCE_QUANTITY = static_cast<size_t>(Resource::RESOURCE_SIZE);

        /**
         * @brief Coefficients associated with each resource type.
         */
        const std::array<float, static_cast<int> (Resource::RESOURCE_SIZE)> coeff = {
            0.5,
            0.3,
            0.15,
            0.1,
            0.1,
            0.08,
            0.05
        };

        /**
         * @brief Names of the resources as strings.
         */
        const std::array<std::string, RESOURCE_QUANTITY> names = {
            "food",
            "linemate",
            "deraumere",
            "sibur",
            "mendiane",
            "phiras",
            "thystame"
        };

        /**
         * @brief Converts a Resource enum to its underlying size_t value.
         * 
         * @param res The resource enum.
         * @return size_t The numeric representation of the resource.
         */
        size_t castResource(const Resource &res);

        /**
         * @brief Gets the string name of a given resource.
         * 
         * @param res The resource enum.
         * @return const std::string& The string name corresponding to the resource.
         */
        const std::string &getName(const Resource &res);

        /**
         * @brief Converts a string name to the corresponding Resource enum.
         * 
         * @param name The string name of the resource.
         * @return Resource The corresponding resource enum.
         */
        Resource getResource(const std::string &name);
    }
}
