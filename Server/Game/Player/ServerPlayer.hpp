//
// EPITECH PROJECT, 2025
// Player
// File description:
// Player
//

#pragma once

#include <atomic>
#include <chrono>

#include "Client.hpp"
#include "Player.hpp"

namespace zappy {
    namespace game {
        class ITeams;

        /**
         * @brief Server-side player implementation for the Zappy game
         * 
         * This class extends the base Player class to provide server-specific functionality
         * including client communication, timing mechanisms, action management, and team
         * association. It handles the server-side representation of connected players
         * with thread-safe operations and lifecycle management.
         */
        class ServerPlayer : public Player {
           public:
            /**
             * @brief Constructor for ServerPlayer
             * 
             * Creates a new server player with initial food supply and associates them
             * with a client connection and team. The player starts with 10 food units
             * and their timing mechanisms are initialized.
             * 
             * @param user Client connection object for network communication
             * @param id Unique identifier for the player
             * @param x Initial X coordinate position on the map
             * @param y Initial Y coordinate position on the map
             * @param orientation Initial orientation of the player (N, E, S, W)
             * @param team Reference to the team this player belongs to
             * @param level Initial level of the player (default: 1)
             */
            ServerPlayer(zappy::server::Client user, size_t id, size_t x,
                size_t y, Orientation orientation, zappy::game::ITeams &team,
                size_t level = 1)
                : Player::Player(id, x, y, orientation, level),
                  _user(std::move(user)),
                  _startTime(std::chrono::steady_clock::now()),
                  _lifeTime(std::chrono::steady_clock::now()), _team(team)
            {
                constexpr int startFood = 10;
                this->collectRessource(zappy::game::Resource::FOOD, startFood);
            }

            /**
             * @brief Default destructor
             */
            ~ServerPlayer() = default;

            /**
             * @brief Get reference to the client connection
             * 
             * @return zappy::server::Client& Reference to the client object for network communication
             */
            zappy::server::Client &getClient() { return this->_user; };

            /**
             * @brief Start or restart the action timer
             * 
             * Initializes the timing mechanism for tracking action duration.
             * This is typically called when a player starts performing an action.
             */
            void startChrono()
            {
                _startTime = std::chrono::steady_clock::now();
            }

            /**
             * @brief Get elapsed time since action started
             * 
             * Calculates the duration since the last call to startChrono().
             * Used to determine if enough time has passed for an action to complete.
             * 
             * @return std::chrono::duration<double> Time elapsed since action start
             */
            std::chrono::duration<double> getChrono() const
            {
                auto now = std::chrono::steady_clock::now();
                return now - _startTime;
            }

            /**
             * @brief Get elapsed time since player creation or last life reset
             * 
             * Calculates the duration since the player was created or since
             * the last call to resetLifeChrono(). Used for life management
             * and survival mechanics.
             * 
             * @return std::chrono::duration<double> Time elapsed since life timer start
             */
            std::chrono::duration<double> getLifeChrono() const
            {
                auto now = std::chrono::steady_clock::now();
                return now - _lifeTime;
            }

            /**
             * @brief Reset the life timer
             * 
             * Resets the life chronometer to the current time.
             * Typically called when the player consumes food or performs
             * actions that affect their survival timer.
             */
            void resetLifeChrono()
            {
                _lifeTime = std::chrono::steady_clock::now();
            }

            /**
             * @brief Check if player is currently performing an action
             * 
             * @return bool True if the player is currently executing an action, false otherwise
             */
            bool isInAction() { return _actionStarted; }

            /**
             * @brief Set the action status of the player
             * 
             * Updates whether the player is currently performing an action.
             * This is used to prevent multiple simultaneous actions and
             * manage action queuing.
             * 
             * @param status True to mark player as in action, false to mark as available
             */
            void setInAction(bool status) { _actionStarted = status; }

            /**
             * @brief Get reference to the player's team
             * 
             * @return zappy::game::ITeams& Reference to the team this player belongs to
             */
            zappy::game::ITeams &getTeam() { return _team; }

            /**
             * @brief Mutex for thread-safe message operations
             * 
             * Protects message-related operations from race conditions
             * when multiple threads need to send messages to or from this player.
             */
            std::mutex msgMutex;

            /**
             * @brief Atomic flag indicating if player operations should be interrupted
             * 
             * Thread-safe flag used to signal that the player's current operations
             * should be interrupted, typically when the player disconnects or
             * the server is shutting down.
             */
            std::atomic<bool> interrupted;

           private:
            /**
             * @brief Client connection object
             * 
             * Handles network communication with the connected client.
             * Moved into the player object during construction.
             */
            zappy::server::Client _user;

            /**
             * @brief Timestamp when the current action started
             * 
             * Used to track how long an action has been running
             * and determine when it should complete.
             */
            std::chrono::steady_clock::time_point _startTime;

            /**
             * @brief Timestamp for life/survival tracking
             * 
             * Used to track player survival time for food consumption
             * and death mechanics.
             */
            std::chrono::steady_clock::time_point _lifeTime;

            /**
             * @brief Atomic flag indicating if player is currently performing an action
             * 
             * Thread-safe flag to prevent multiple simultaneous actions
             * and manage action state in a multi-threaded environment.
             */
            std::atomic<bool> _actionStarted = false;

            /**
             * @brief Reference to the player's team
             * 
             * Associates this player with a specific team in the game,
             * affecting spawn locations, win conditions, and team-based mechanics.
             */
            zappy::game::ITeams &_team;
        };
    }  // namespace game
}  // namespace zappy
