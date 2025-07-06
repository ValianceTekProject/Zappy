/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ResourceContainer.hpp
*/

#pragma once

#include "Resource.hpp"

namespace zappy {
    namespace game {
        /**
         * @class ResourceContainer
         * @brief Manages a collection of resources and their quantities.
         */
        class ResourceContainer
        {
            public:
                /**
                 * @brief Construct a new ResourceContainer object and clear all resources.
                 */
                ResourceContainer() { clear(); }

                /**
                 * @brief Copy constructor.
                 * 
                 * @param other Another ResourceContainer to copy from.
                 */
                ResourceContainer(const ResourceContainer &other) = default;

                /**
                 * @brief Destructor.
                 */
                ~ResourceContainer() = default;

                /**
                 * @brief Clears all resources, setting their quantities to zero.
                 */
                void clear();

                /**
                 * @brief Adds a given quantity of a resource.
                 * 
                 * @param resource The resource to add.
                 * @param quantity The quantity to add (default is 1).
                 */
                void addResource(Resource resource, size_t quantity = 1);

                /**
                 * @brief Adds a single unit of the specified resource.
                 * 
                 * @param resource The resource to add.
                 */
                void addSingleResource(Resource resource);

                /**
                 * @brief Removes a given quantity of a resource.
                 * 
                 * @param resource The resource to remove.
                 * @param quantity The quantity to remove (default is 1).
                 */
                void removeResource(Resource resource, size_t quantity = 1);

                /**
                 * @brief Gets the quantity of a specific resource.
                 * 
                 * @param resource The resource to query.
                 * @return size_t Quantity of the resource.
                 */
                size_t getResourceQuantity(Resource resource) const;

                /**
                 * @brief Gets all resources and their quantities.
                 * 
                 * @return const std::array<size_t, RESOURCE_QUANTITY>& Reference to the resource quantities array.
                 */
                const std::array<size_t, RESOURCE_QUANTITY> &getResources() const { return this->_resources; }

            private:
                std::array<size_t, RESOURCE_QUANTITY> _resources; ///< Array holding quantities of each resource.
        };
    }
}
