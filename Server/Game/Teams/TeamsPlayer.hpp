//
// EPITECH PROJECT, 2025
// Teams
// File description:
// Teams
//

#pragma once

#include "ATeams.hpp"

namespace zappy {
    namespace game {
        class ServerPlayer;

        /**
         * @brief Player-specific team implementation for the Zappy game
         * 
         * This class extends ATeams to provide a team implementation specifically
         * designed for managing actual game players (as opposed to GUI clients).
         * It includes additional functionality for managing client capacity,
         * controlling team membership limits, and providing thread-safe operations
         * for player admission control. This class handles the server-side logic
         * for determining when new players can join a team.
         */
        class TeamsPlayer : public ATeams {
           public:
            /**
             * @brief Constructor for TeamsPlayer
             * 
             * Creates a new player team by calling the parent ATeams constructor.
             * The team is initialized with a name and unique identifier,
             * inheriting all standard team functionality while adding
             * player-specific capacity management.
             * 
             * @param name The name of the team (immutable)
             * @param id Unique identifier for the team
             */
            TeamsPlayer(const std::string &name, int id) : ATeams(name, id) {}

            /**
             * @brief Default destructor
             * 
             * Uses the default destructor as no additional cleanup is required
             * beyond what the parent ATeams class provides.
             */
            ~TeamsPlayer() = default;

            /**
             * @brief Set the maximum number of clients allowed on this team
             * 
             * Sets the capacity limit for how many players can be members
             * of this team simultaneously. This is used to enforce team
             * balance and game rules regarding team size.
             * 
             * @param clientNb Maximum number of clients that can join this team
             */
            void setClientNb(const int clientNb) { _clientNb = clientNb; }

            /**
             * @brief Get the maximum number of clients allowed on this team
             * 
             * Returns the current capacity limit for this team.
             * 
             * @return int Maximum number of clients that can join this team
             */
            int getClientNb() const { return this->_clientNb; }

            /**
             * @brief Allow a new player to join the team
             * 
             * Handles the logic for admitting new players to the team.
             * This method should implement capacity checking and other
             * admission control logic. The operation is thread-safe
             * through the use of the client number lock.
             */
            void allowNewPlayer();

           private:
            /**
             * @brief Maximum number of clients allowed on this team
             * 
             * Stores the capacity limit for this team. This value
             * determines how many players can be active members
             * of the team at any given time.
             */
            int _clientNb;

            /**
             * @brief Mutex for thread-safe client number operations
             * 
             * Protects operations related to client capacity management
             * from race conditions when multiple threads attempt to
             * check or modify team membership limits simultaneously.
             * Ensures data consistency in multi-threaded server environment.
             */
            std::mutex _clientNbLock;
        };
    }  // namespace game
}  // namespace zappy
