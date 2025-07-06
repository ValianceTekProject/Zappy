/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ATeams
*/

#pragma once

#include "ITeams.hpp"

namespace zappy {
    namespace game {
        /**
         * @brief Abstract base implementation of the ITeams interface
         * 
         * This class provides a concrete implementation of the ITeams interface,
         * managing team properties and player membership. It handles thread-safe
         * operations for adding and removing players from the team, maintaining
         * team identity and player associations in the Zappy game.
         */
        class ATeams : public ITeams {
           public:
            /**
                 * @brief Constructor for ATeams
                 * 
                 * Creates a new team with a specified name and unique identifier.
                 * The team name is immutable once set during construction.
                 * 
                 * @param name The name of the team (immutable)
                 * @param id Unique identifier for the team
                 */
            ATeams(const std::string &name, int id) : _name(name), _teamId(id)
            {}

            /**
                 * @brief Get the team name
                 * 
                 * @return std::string The name of the team
                 */
            std::string getName() const { return _name; }

            /**
                 * @brief Add a player to the team
                 * 
                 * Adds a new player to the team's player list in a thread-safe manner.
                 * The operation is protected by a mutex to prevent race conditions
                 * when multiple threads attempt to modify the player list simultaneously.
                 * 
                 * @param player Shared pointer to the ServerPlayer to add to the team
                 */
            void addPlayer(std::shared_ptr<ServerPlayer> player);

            /**
                 * @brief Remove a player from the team
                 * 
                 * Removes a player from the team's player list based on their socket identifier.
                 * The operation is thread-safe and searches through the player list to find
                 * and remove the player with the matching socket.
                 * 
                 * @param playerSocket Socket identifier of the player to remove
                 */
            void removePlayer(int playerSocket);

            /**
                 * @brief Get the list of players in the team
                 * 
                 * Returns a copy of the current player list for the team.
                 * This method provides read-only access to the team's members
                 * without exposing the internal data structure.
                 * 
                 * @return const std::vector<std::shared_ptr<ServerPlayer>> Copy of the team's player list
                 */
            const std::vector<std::shared_ptr<ServerPlayer>>
            getPlayerList() const;

            /**
                 * @brief Get the team identifier
                 * 
                 * @return int The unique identifier of the team
                 */
            int getTeamId() const { return this->_teamId; }

           private:
            /**
                 * @brief The name of the team
                 * 
                 * Immutable string that identifies the team by name.
                 * Set during construction and cannot be changed afterward.
                 */
            const std::string _name;

            /**
                 * @brief Unique identifier for the team
                 * 
                 * Numeric identifier that uniquely identifies this team
                 * within the game server.
                 */
            int _teamId;

            /**
                 * @brief List of players belonging to this team
                 * 
                 * Vector containing shared pointers to all ServerPlayer objects
                 * that are currently members of this team. Access is protected
                 * by the _playerListLock mutex.
                 */
            std::vector<std::shared_ptr<ServerPlayer>> _playerList;

            /**
                 * @brief Mutex for thread-safe player list operations
                 * 
                 * Protects the _playerList from race conditions when multiple
                 * threads attempt to add, remove, or access players simultaneously.
                 * Ensures data consistency in multi-threaded server environment.
                 */
            std::mutex _playerListLock;
        };
    }  // namespace game
}  // namespace zappy
