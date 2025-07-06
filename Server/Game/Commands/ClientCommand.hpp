/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientCommand
*/

#pragma once

#include "GameError.hpp"
#include "GuiCommand.hpp"
#include "ITeams.hpp"
#include "Orientation.hpp"
#include "Player.hpp"
#include "Resource.hpp"
#include "ServerMap.hpp"
#include "ServerPlayer.hpp"

#include <algorithm>
#include <chrono>
#include <functional>
#include <iostream>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <thread>
#include <unistd.h>

namespace zappy {
    namespace game {
        /** @brief Minimum level for a player */
        constexpr int minLevel = 1;

        /** @brief Maximum level for a player */
        constexpr int maxLevel = 8;

        /**
         * @brief Structure representing the requirements for level elevation
         * 
         * This structure defines the number of players and resources needed
         * to perform an elevation ritual at a specific level.
         */
        typedef struct elevation_s {
            size_t players;   /**< Number of players required for elevation */
            size_t linemate;  /**< Number of linemate stones required */
            size_t deraumere; /**< Number of deraumere stones required */
            size_t sibur;     /**< Number of sibur stones required */
            size_t mendiane;  /**< Number of mendiane stones required */
            size_t phiras;    /**< Number of phiras stones required */
            size_t thystame;  /**< Number of thystame stones required */
        } elevation_t;

        /**
         * @brief Array containing elevation requirements for each level
         * 
         * This array defines the specific requirements (players and resources)
         * needed to elevate from one level to the next, from level 1 to level 8.
         */
        const std::array<elevation_t, 7> elevationRequirements = {
            {{1, 1, 0, 0, 0, 0, 0}, {2, 1, 1, 1, 0, 0, 0},
                {2, 2, 0, 1, 0, 2, 0}, {4, 1, 1, 2, 0, 1, 0},
                {4, 1, 2, 1, 3, 0, 0}, {6, 1, 2, 3, 0, 1, 0},
                {6, 2, 2, 2, 2, 2, 1}}};

        /**
         * @brief Main command handler class for the Zappy game
         * 
         * This class handles all player commands and game logic for the Zappy game.
         * It inherits from CommandHandlerGui and provides functionality for player
         * movement, resource management, communication, and level elevation.
         */
        class CommandHandler : public CommandHandlerGui {
           public:
            /**
             * @brief Enumeration of time limits for different commands
             * 
             * Each command has a specific time limit (in time units) that determines
             * how long the command takes to execute.
             */
            enum class timeLimit {
                FORWARD = 7,     /**< Time limit for forward movement */
                RIGHT = 7,       /**< Time limit for right turn */
                LEFT = 7,        /**< Time limit for left turn */
                LOOK = 7,        /**< Time limit for look command */
                INVENTORY = 1,   /**< Time limit for inventory check */
                BROADCAST = 7,   /**< Time limit for broadcast message */
                CONNECT_NBR = 0, /**< Time limit for connection number check */
                FORK = 42,       /**< Time limit for fork command */
                EJECT = 7,       /**< Time limit for eject command */
                TAKE = 7,        /**< Time limit for take command */
                SET = 7,         /**< Time limit for set/drop command */
                INCANTATION = 300 /**< Time limit for incantation ritual */
            };

            /**
             * @brief Enumeration for sound direction in broadcast messages
             * 
             * Defines the direction from which a broadcast message originates
             * relative to the receiving player's position.
             */
            enum class SoundDirection : int {
                SAME_POSITION = 0, /**< Sound comes from same position */
                NORTH = 1,         /**< Sound comes from north */
                NORTHWEST = 2,     /**< Sound comes from northwest */
                WEST = 3,          /**< Sound comes from west */
                SOUTHWEST = 4,     /**< Sound comes from southwest */
                SOUTH = 5,         /**< Sound comes from south */
                SOUTHEAST = 6,     /**< Sound comes from southeast */
                EAST = 7,          /**< Sound comes from east */
                NORTHEAST = 8      /**< Sound comes from northeast */
            };

            /**
             * @brief Constructor for CommandHandler
             * 
             * @param freq Reference to the game frequency
             * @param width Width of the game map
             * @param height Height of the game map
             * @param clientNb Number of clients
             * @param map Reference to the game map server
             * @param teamList Reference to the list of teams
             */
            CommandHandler(int &freq, int width, int height, int clientNb,
                zappy::game::MapServer &map,
                std::vector<std::shared_ptr<ITeams>> &teamList)
                : CommandHandlerGui(
                      freq, width, height, clientNb, map, teamList) {};

            /**
             * @brief Destructor for CommandHandler
             */
            ~CommandHandler() = default;

            /**
             * @brief Process client input and execute corresponding commands
             * 
             * @param input The input string from the client
             * @param player Reference to the server player object
             */
            void processClientInput(std::string &input,
                zappy::game::ServerPlayer &player) override;

            /**
             * @brief Handle incantation printing for GUI
             * 
             * @param player Reference to the player performing incantation
             */
            void incantationPrinting(zappy::game::ServerPlayer &player);

            /**
             * @brief Send resource information to GUI
             * 
             * @param player Reference to the player whose resources to send
             */
            void resourceSendGui(zappy::game::ServerPlayer &player);

            /**
             * @brief Initialize the command map with available commands
             */
            void initCommandMap() override;

            /**
             * @brief Get the current game frequency
             * 
             * @return Reference to the game frequency
             */
            int &getFreq() { return _freq; }

            /**
             * @brief Send a message to the GUI
             * 
             * @param msg The message to send to the GUI
             */
            void messageToGUI(const std::string &msg);

            /**
             * @brief Execute a command with proper timing and validation
             * 
             * @param player Reference to the player executing the command
             * @param function Function pointer to the command to execute
             * @param args Arguments for the command
             */
            void _executeCommand(zappy::game::ServerPlayer &player,
                std::function<void(ServerPlayer &, const std::string &)>
                    function,
                const std::string &args);

           private:
            /**
             * @brief Handle forward movement command
             * 
             * @param player Reference to the player moving forward
             */
            void handleForward(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle right turn command
             * 
             * @param player Reference to the player turning right
             */
            void handleRight(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle left turn command
             * 
             * @param player Reference to the player turning left
             */
            void handleLeft(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle look command to see surrounding tiles
             * 
             * @param player Reference to the player looking around
             */
            void handleLook(zappy::game::ServerPlayer &player);

            /**
             * @brief Build the look message containing visible tiles
             * 
             * @param player Reference to the player performing the look
             * @return String containing the look message
             */
            std::string _buildLookMessage(ServerPlayer &player);

            /**
             * @brief Get the content of a specific line in the look view
             * 
             * @param player Reference to the player looking
             * @param line The line number in the look view
             * @return String containing the line content
             */
            std::string _lookLine(ServerPlayer &player, size_t line);

            /**
             * @brief Compute the target coordinates for a look operation
             * 
             * @param player Reference to the player looking
             * @param line The line number in the look view
             * @param offset The offset within the line
             * @return Pair of coordinates (x, y)
             */
            std::pair<int, int> _computeLookTarget(
                ServerPlayer &player, size_t line, int offset);

            /**
             * @brief Get the content of a tile at specific coordinates
             * 
             * @param x X coordinate of the tile
             * @param y Y coordinate of the tile
             * @param isPlayerTile Whether this is the player's current tile
             * @return String describing the tile content
             */
            std::string _getTileContent(size_t x, size_t y, bool isPlayerTile);

            /**
             * @brief Check if a tile is the last tile in the look view
             * 
             * @param playerLevel Level of the player
             * @param line Line number in the look view
             * @param offset Offset within the line
             * @return True if it's the last tile, false otherwise
             */
            bool _checkLastTileInLook(
                size_t playerLevel, size_t line, int offset);

            /**
             * @brief Handle inventory command to show player's resources
             * 
             * @param player Reference to the player checking inventory
             */
            void handleInventory(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle broadcast command to send messages to other players
             * 
             * @param player Reference to the player broadcasting
             * @param arg The message to broadcast
             */
            void handleBroadcast(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
             * @brief Compute the distance between two points for broadcast
             * 
             * @param x1 X coordinate of first point
             * @param y1 Y coordinate of first point
             * @param x2 X coordinate of second point
             * @param y2 Y coordinate of second point
             * @return Pair containing the distance components
             */
            std::pair<int, int> _computeBroadcastDistance(
                int x1, int y1, int x2, int y2);

            /**
             * @brief Compute the sound direction for a broadcast message
             * 
             * @param player Reference to the broadcasting player
             * @param receiver Reference to the receiving player
             * @return Integer representing the sound direction
             */
            int _computeSoundDirection(
                const ServerPlayer &player, const ServerPlayer &receiver);

            /**
             * @brief Get the cardinal point for sound direction
             * 
             * @param relativeX Relative X coordinate
             * @param relativeY Relative Y coordinate
             * @return Integer representing the cardinal point
             */
            int _getSoundCardinalPoint(int relativeX, int relativeY);

            /**
             * @brief Handle connect_nbr command to get available connections
             * 
             * @param player Reference to the player requesting connection count
             */
            void handleConnectNbr(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle fork command to create a new player
             * 
             * @param player Reference to the player forking
             */
            void handleFork(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle eject command to push other players
             * 
             * @param player Reference to the player performing eject
             */
            void handleEject(zappy::game::ServerPlayer &player);

            /**
             * @brief Handle take command to pick up resources
             * 
             * @param player Reference to the player taking resources
             * @param arg The resource type to take
             */
            void handleTake(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
             * @brief Handle drop command to place resources on the ground
             * 
             * @param player Reference to the player dropping resources
             * @param arg The resource type to drop
             */
            void handleDrop(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
             * @brief Handle incantation command to perform level elevation
             * 
             * @param player Reference to the player performing incantation
             */
            void handleIncantation(zappy::game::ServerPlayer &player);

            /**
             * @brief Check if incantation conditions are met
             * 
             * @param player Reference to the player attempting incantation
             * @return True if conditions are met, false otherwise
             */
            bool _checkIncantationConditions(const ServerPlayer &player);

            /**
             * @brief Get players eligible for incantation at a location
             * 
             * @param x X coordinate of the location
             * @param y Y coordinate of the location
             * @param level Required level for incantation
             * @return Vector of weak pointers to eligible players
             */
            std::vector<std::weak_ptr<ServerPlayer>> _getPlayersForIncant(
                int x, int y, size_t level);

            /**
             * @brief Get players currently incanting at a location
             * 
             * @param x X coordinate of the location
             * @param y Y coordinate of the location
             * @param level Required level for incantation
             * @return Vector of weak pointers to incanting players
             */
            std::vector<std::weak_ptr<ServerPlayer>> _getPlayersIncanting(
                int x, int y, size_t level);

            /**
             * @brief Check if required resources are available for incantation
             * 
             * @param x X coordinate of the location
             * @param y Y coordinate of the location
             * @param level Level for which to check resources
             * @return True if resources are sufficient, false otherwise
             */
            bool _checkIncantationResources(size_t x, size_t y, size_t level);

            /**
             * @brief Consume resources required for elevation
             * 
             * @param x X coordinate of the location
             * @param y Y coordinate of the location
             * @param level Level for which to consume resources
             */
            void _consumeElevationResources(size_t x, size_t y, size_t level);

            /**
             * @brief Elevate a player to the next level
             * 
             * @param player Reference to the player to elevate
             */
            void _elevatePlayer(ServerPlayer &player);

            /**
             * @brief Set a player in prayer state for incantation
             * 
             * @param player Reference to the player to set in prayer
             */
            void _setPrayer(zappy::game::ServerPlayer &player);

            /**
             * @brief Get all players on a specific tile
             * 
             * @param x X coordinate of the tile
             * @param y Y coordinate of the tile
             * @return Vector of weak pointers to players on the tile
             */
            std::vector<std::weak_ptr<ServerPlayer>> _getPlayerOnTile(
                int x, int y);

            /**
             * @brief Eject a player forward from their current position
             * 
             * @param player Reference to the player being ejected
             * @param orientation Reference to the player's orientation
             * @param pushingPlayer Reference to the player doing the ejecting
             */
            void ejectPlayerForward(ServerPlayer &player,
                Orientation &orientation, ServerPlayer &pushingPlayer);

            /**
             * @brief Send expulsion message to affected players
             * 
             * @param player Reference to the ejected player
             * @param pushingPlayer Reference to the ejecting player
             */
            void _sendExpulseMsg(
                ServerPlayer &player, ServerPlayer &pushingPlayer);

            /**
             * @brief Wait for a command to complete within time limit
             * 
             * @param player Reference to the player waiting
             * @param limit Time limit for the command
             * @return True if command completed within limit, false otherwise
             */
            [[nodiscard]] bool _waitCommand(
                ServerPlayer &player, timeLimit limit);

            /**
             * @brief Normalize coordinates to fit within map boundaries
             * 
             * @param x X coordinate to normalize
             * @param y Y coordinate to normalize
             * @return Pair of normalized coordinates
             */
            std::pair<size_t, size_t> _normalizeCoords(size_t x, size_t y);

            /**
             * @brief Get direction vector based on player orientation
             * 
             * @param player Reference to the player
             * @param dx Reference to X direction component
             * @param dy Reference to Y direction component
             */
            void _getDirectionVector(const Player &player, int &dx, int &dy);
        };
    }  // namespace game

}  // namespace zappy
