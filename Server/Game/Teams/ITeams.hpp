/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ITeams
*/

#pragma once

#include "Client.hpp"
#include "ServerPlayer.hpp"
#include <memory>
#include <mutex>
#include <queue>
#include <string>

namespace zappy {
    namespace game {
        /**
         * @brief Interface for team management in the Zappy game
         * 
         * This interface defines the contract for team implementations in the Zappy game.
         * It provides pure virtual methods for managing team properties and player membership.
         * Concrete implementations must provide thread-safe operations for player management
         * and maintain team identity throughout the game lifecycle.
         */
        class ITeams {
           public:
            /**
                 * @brief Get the team name
                 * 
                 * Pure virtual method that must be implemented by concrete team classes
                 * to return the team's identifying name.
                 * 
                 * @return std::string The name of the team
                 */
            virtual std::string getName() const = 0;

            /**
                 * @brief Remove a player from the team
                 * 
                 * Pure virtual method that must be implemented to remove a player
                 * from the team based on their socket identifier. Implementations
                 * should ensure thread-safe removal of players.
                 * 
                 * @param playerSocket Socket identifier of the player to remove from the team
                 */
            virtual void removePlayer(int playerSocket) = 0;

            /**
                 * @brief Get the list of players in the team
                 * 
                 * Pure virtual method that must be implemented to return the current
                 * list of players belonging to this team. Implementations should
                 * provide read-only access to prevent external modification.
                 * 
                 * @return const std::vector<std::shared_ptr<ServerPlayer>> List of players in the team
                 */
            virtual const std::vector<std::shared_ptr<ServerPlayer>>
            getPlayerList() const = 0;

            /**
                 * @brief Add a player to the team
                 * 
                 * Pure virtual method that must be implemented to add a new player
                 * to the team. Implementations should ensure thread-safe addition
                 * of players to handle concurrent access.
                 * 
                 * @param player Shared pointer to the ServerPlayer to add to the team
                 */
            virtual void addPlayer(std::shared_ptr<ServerPlayer> player) = 0;

            /**
                 * @brief Get the team identifier
                 * 
                 * Pure virtual method that must be implemented to return the unique
                 * identifier of the team used for game logic and team differentiation.
                 * 
                 * @return int The unique identifier of the team
                 */
            virtual int getTeamId() const = 0;
        };
    }  // namespace game
}  // namespace zappy
