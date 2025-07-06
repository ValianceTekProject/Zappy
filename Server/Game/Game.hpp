/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game
*/

#pragma once

#include "ITeams.hpp"
#include "TeamsPlayer.hpp"
#include "ServerMap.hpp"
#include "my_macros.hpp"
#include "ClientCommand.hpp"
#include "GuiCommand.hpp"
#include <atomic>
#include <chrono>
#include <memory>

namespace zappy {
    namespace game {
        /**
         * @brief Time constant for resource respawn mechanism
         * 
         * Defines the time interval (in seconds) before resources respawn on the map.
         * This constant controls the resource regeneration rate in the game.
         */
        #define TIME_BEFORE_RESPAWN 20
        
        /**
         * @brief Main game controller for the Zappy game server
         * 
         * This class serves as the central coordinator for the Zappy game server,
         * managing all game components including the map, players, teams, and
         * command handlers. It orchestrates the game loop, handles player connections,
         * manages team membership, implements win conditions, and coordinates
         * communication between different game subsystems. The class provides
         * thread-safe operations for managing game state in a multi-threaded
         * server environment.
         */
        class Game {
           public:
            /**
             * @brief Constructor for Game
             * 
             * Initializes the complete game environment including map, command handlers,
             * teams, and game parameters. Sets up the initial game state by placing
             * eggs on the map for each team and configuring team capacities.
             * 
             * @param mapWidth Width of the game map
             * @param mapHeight Height of the game map
             * @param teamList Vector of teams participating in the game
             * @param freq Reference to the game frequency (time units per second)
             * @param clientNb Maximum number of clients per team
             */
            Game(int mapWidth, int mapHeight, std::vector<std::shared_ptr<ITeams>> teamList, int &freq, int clientNb)
                : _commandHandlerGui(freq, mapWidth, mapHeight, clientNb, _map, _teamList),
                _map(mapWidth, mapHeight, _commandHandlerGui),
                _commandHandler(freq, _map.getWidth(), _map.getHeight(), clientNb, _map, _teamList),
                _teamList(teamList),
                _baseFreqMs(freq),
                _clientNb(clientNb)
            {
                for (auto &team : this->_teamList) {
                    auto teamCast = (dynamic_cast<TeamsPlayer*>(&(*team)));
                    if (teamCast)
                        teamCast->setClientNb(_clientNb);
                }
                std::srand(std::time(nullptr));
                this->_map.setEggsonMap(teamList, clientNb);
            }
            
            /**
             * @brief Default destructor
             */
            ~Game() = default;
            
            /**
             * @brief Main game execution loop
             * 
             * Starts and manages the primary game loop, handling game state updates,
             * player actions, and server operations. This method runs continuously
             * until the game is stopped or a win condition is met.
             */
            void runGame();
            
            /**
             * @brief Execute core game logic
             * 
             * Processes game mechanics including player actions, resource management,
             * food consumption, win condition checking, and other game-specific
             * logic that needs to be executed each game cycle.
             */
            void gameLogic();
            
            /**
             * @brief Set the game running state
             * 
             * Controls the game execution state (running, paused, stopped).
             * This method is thread-safe using atomic operations.
             * 
             * @param run New running state for the game
             */
            void setRunningState(RunningState run) { this->_isRunning = run; };
            
            /**
             * @brief Get the current game running state
             * 
             * @return RunningState Current state of the game (running, paused, stopped)
             */
            RunningState getRunningState() { return _isRunning; };
            
            /**
             * @brief Handle a player's request to join a team
             * 
             * Processes team join requests from clients, validates team availability,
             * manages team capacity, and assigns players to teams. This operation
             * is thread-safe and handles concurrent join requests.
             * 
             * @param clientSocket Socket identifier of the client requesting to join
             * @param teamName Name of the team the client wants to join
             * @return bool True if the join was successful, false otherwise
             */
            bool handleTeamJoin(int clientSocket, const std::string &teamName);
            
            /**
             * @brief Remove a player from their current team
             * 
             * Removes a player from their team when they disconnect or leave.
             * This method handles cleanup of player references and team membership.
             * 
             * @param clientSocket Socket identifier of the client to remove
             */
            void removeFromTeam(int clientSocket);
            
            /**
             * @brief Check if any team has won the game
             * 
             * Evaluates win conditions based on game rules (typically involving
             * player levels and team composition). Returns true if a team has
             * achieved victory conditions.
             * 
             * @return bool True if a team has won, false otherwise
             */
            bool checkWin();
            
            /**
             * @brief Get reference to the game frequency
             * 
             * @return int& Reference to the game frequency (time units per second)
             */
            int &getFreq() { return this->_baseFreqMs; }
            
            /**
             * @brief Get the maximum number of clients per team
             * 
             * @return int Maximum number of clients that can join each team
             */
            int getClientNb() const { return this->_clientNb; }
            
            /**
             * @brief Get reference to the game map
             * 
             * @return MapServer& Reference to the game map server
             */
            MapServer &getMap() { return this->_map; }
            
            /**
             * @brief Get reference to the team list
             * 
             * @return std::vector<std::shared_ptr<zappy::game::ITeams>>& Reference to the list of teams
             */
            std::vector<std::shared_ptr<zappy::game::ITeams>> &getTeamList() { return this->_teamList; };
            
            /**
             * @brief Manage food consumption for a team
             * 
             * Handles the food consumption mechanics for all players in a team,
             * including starvation checks and player death due to lack of food.
             * This method is called periodically to maintain survival mechanics.
             * 
             * @param team Reference to the team whose food needs to be managed
             */
            void foodManager(std::shared_ptr<ITeams> &team);
            
            /**
             * @brief Remove food from a player or handle player death
             * 
             * Processes food consumption for individual players and handles
             * player death when food runs out. Manages the transition from
             * living player to death state.
             * 
             * @param player Reference to the player whose food is being managed
             * @param itPlayerTeam Reference to the team the player belongs to
             */
            void removeFoodOrDiedPlayer(std::shared_ptr<zappy::game::ServerPlayer> &player,
                std::shared_ptr<zappy::game::TeamsPlayer> itPlayerTeam);
            
            /**
             * @brief Get reference to the player command handler
             * 
             * @return zappy::game::CommandHandler& Reference to the command handler for player clients
             */
            zappy::game::CommandHandler &getCommandHandler() { return _commandHandler; }
            
            /**
             * @brief Get reference to the GUI command handler
             * 
             * @return zappy::game::CommandHandlerGui& Reference to the command handler for GUI clients
             */
            zappy::game::CommandHandlerGui &getCommandHandlerGui() { return _commandHandlerGui; }
            
           private:
            /**
             * @brief Total player ID counter
             * 
             * Keeps track of the next available player ID to ensure
             * each player has a unique identifier. Starts at 1.
             */
            int _idPlayerTot = 1;
            
            /**
             * @brief GUI command handler instance
             * 
             * Handles all commands and communication with GUI clients,
             * providing visual interface support for the game.
             */
            zappy::game::CommandHandlerGui _commandHandlerGui;
            
            /**
             * @brief Game map server instance
             * 
             * Manages the game world, including tiles, resources, eggs,
             * and spatial relationships between game objects.
             */
            MapServer _map;
            
            /**
             * @brief Player command handler instance
             * 
             * Handles all commands and communication with player clients,
             * processing game actions and responses.
             */
            zappy::game::CommandHandler _commandHandler;
            
            /**
             * @brief List of teams in the game
             * 
             * Contains all teams participating in the current game session,
             * including both player teams and GUI teams.
             */
            std::vector<std::shared_ptr<zappy::game::ITeams>> _teamList;
            
            /**
             * @brief List of all players in the game
             * 
             * Maintains weak references to all players to avoid circular
             * dependencies while tracking active players across teams.
             */
            std::vector<std::weak_ptr<zappy::game::Player>> _playerList;
            
            /**
             * @brief Reference to the base game frequency
             * 
             * Controls the game speed and timing for all game operations.
             * This value can be modified during gameplay.
             */
            int &_baseFreqMs;
            
            /**
             * @brief Maximum number of clients per team
             * 
             * Defines the capacity limit for team membership,
             * controlling game balance and server load.
             */
            int _clientNb;
            
            /**
             * @brief Atomic game running state
             * 
             * Thread-safe flag indicating the current execution state
             * of the game (running, paused, stopped). Default is PAUSE.
             */
            std::atomic<RunningState> _isRunning = RunningState::PAUSE;
            
            /**
             * @brief Check if a client is already in a team
             * 
             * Internal method to verify if a client socket is already
             * associated with a team before allowing team joins.
             * 
             * @param clientSocket Socket identifier to check
             * @return bool True if client is already in a team, false otherwise
             */
            bool _checkAlreadyInTeam(int clientSocket);
            
            /**
             * @brief Add a player to a specific team
             * 
             * Internal method that handles the actual process of adding
             * a player to a team, including capacity checks and player creation.
             * 
             * @param team Shared pointer to the team to join
             * @param clientSocket Socket identifier of the client
             * @return void* Result of the addition operation
             */
            void _addPlayerToTeam(std::shared_ptr<ITeams> team, int clientSocket);
            
            /**
             * @brief Convert an egg into a player
             * 
             * Internal method that handles the hatching process when a new
             * player joins the game, converting an available egg into a
             * fully functional player object.
             * 
             * @param orientation Initial orientation for the new player
             * @param team Team the player will belong to
             * @param user Client connection object
             * @return std::shared_ptr<zappy::game::ServerPlayer> The newly created player
             */
            std::shared_ptr<zappy::game::ServerPlayer> _changeEggToPlayer(zappy::game::Orientation orientation,
                std::shared_ptr<zappy::game::ITeams> team, zappy::server::Client &user);
            
            /**
             * @brief Send new player notification to GUI clients
             * 
             * Internal method that notifies all connected GUI clients
             * about a new player joining the game, updating their visual
             * representation of the game state.
             * 
             * @param newPlayer Reference to the newly created player
             * @return void* Result of the notification operation
             */
            void _sendNewPlayerToGui(std::shared_ptr<zappy::game::ServerPlayer> &newPlayer);
        };
    }  // namespace game
}  // namespace zappy
