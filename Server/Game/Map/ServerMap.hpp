//
// EPITECH PROJECT, 2025
// Map
// File description:
// Map
//

#pragma once

#include "Egg.hpp"
#include "Map.hpp"
#include "ITeams.hpp"
#include <exception>
#include <sstream>
#include <list>
#include <chrono>
#include "TeamsGui.hpp"
#include "GuiCommand.hpp"

namespace zappy {
    namespace game {
        /**
         * @brief Server father ID constant
         * 
         * Defines the ID used for server-related operations when no specific parent is assigned
         */
        #define SERVER_FATHER_ID -1
        
        class CommandHandlerGui;
        
        /**
         * @brief Server-side map implementation for the Zappy game
         * 
         * This class extends the base Map class to provide server-specific functionality
         * including resource management, egg handling, and thread-safe operations.
         * It manages the game world state and coordinates with the GUI command handler
         * for client communication.
         */
        class MapServer : public Map {
           public:
            /**
             * @brief Constructor for MapServer
             * 
             * @param mapWidth Width of the game map
             * @param mapHeight Height of the game map
             * @param commandHandlerGui Reference to the GUI command handler for client communication
             */
            explicit MapServer(int mapWidth, int mapHeight, zappy::game::CommandHandlerGui &commandHandlerGui);
            
            /**
             * @brief Default destructor
             */
            ~MapServer() = default;
            
            /**
             * @brief Initialize eggs on the map for all teams
             * 
             * Places initial eggs on the map based on team configuration and client capacity.
             * This method distributes eggs across the map for each team according to the
             * number of clients that can connect.
             * 
             * @param teamList Reference to the list of teams in the game
             * @param clientNb Maximum number of clients per team
             */
            void setEggsonMap(std::vector<std::shared_ptr<ITeams>> &teamList, int clientNb);
            
            /**
             * @brief Add a new egg to the map
             * 
             * Creates a new egg for a specific team at given coordinates.
             * The egg will be available for players of that team to hatch from.
             * 
             * @param teamId ID of the team the egg belongs to
             * @param x X coordinate where the egg is placed
             * @param y Y coordinate where the egg is placed
             * @return int The unique ID of the newly created egg
             */
            int addNewEgg(int teamId, int x, int y);
            
            /**
             * @brief Remove and return an egg from the map
             * 
             * Pops an egg from the egg list, typically when a player connects
             * and hatches from an egg. This operation is thread-safe.
             * 
             * @return zappy::game::Egg The egg that was removed from the map
             */
            zappy::game::Egg popEgg();
            
            /**
             * @brief Replace resources on the map
             * 
             * Periodically regenerates resources across the map tiles to maintain
             * resource availability for players. This method handles the automatic
             * resource respawn mechanism.
             */
            void replaceResources();
            
            /**
             * @brief Add a replacement resource to a random tile
             * 
             * Adds a specific type of resource to a randomly selected tile on the map.
             * This is used by the resource replacement system to maintain resource
             * distribution across the game world.
             * 
             * @param resourceIdx Index of the resource type to add (0-6):
             *                   0=food, 1=linemate, 2=deraumere, 3=sibur,
             *                   4=mendiane, 5=phiras, 6=thystame
             */
            void addReplaceResourceOnTile(int resourceIdx);
            
            /**
             * @brief Get reference to the egg list
             * 
             * @return std::list<Egg>& Reference to the list of eggs on the map
             */
            std::list<Egg> &getEggList() { return _eggList; }
            
            /**
             * @brief Timestamp of the last resource respawn
             * 
             * Tracks when resources were last regenerated on the map.
             * Used to determine when the next resource respawn should occur.
             */
            std::chrono::steady_clock::time_point _lastResourceRespawn = std::chrono::steady_clock::now();
            
            /**
             * @brief Mutex for thread-safe resource operations
             * 
             * Protects resource-related operations from race conditions
             * in multi-threaded server environment.
             */
            std::mutex _resourceMutex;
            
            /**
             * @brief Mutex for thread-safe egg operations
             * 
             * Protects egg list operations from race conditions
             * when multiple threads access or modify eggs.
             */
            std::mutex _eggMutex;
            
            /**
             * @brief Mutex for thread-safe egg popping operations
             * 
             * Protects the egg popping mechanism from race conditions
             * when multiple players try to connect simultaneously.
             */
            std::mutex _popMutex;
            
           private:
            /**
             * @brief Total egg ID counter
             * 
             * Keeps track of the next available egg ID to ensure
             * each egg has a unique identifier. Starts at 2.
             */
            int _idEggTot = 2;
            
            /**
             * @brief Place initial resources on the map
             * 
             * Internal method to distribute initial resources across
             * all tiles when the map is first created.
             */
            void _placeResources();
            
            /**
             * @brief List of eggs currently on the map
             * 
             * Maintains all eggs that are available for players to hatch from.
             * Each egg belongs to a specific team and has a unique position.
             */
            std::list<Egg> _eggList;
            
            /**
             * @brief Reference to the GUI command handler
             * 
             * Used to communicate map state changes and events
             * to connected GUI clients.
             */
            zappy::game::CommandHandlerGui &_commandHandlerGui;
        };
    }  // namespace game
}  // namespace zappy
