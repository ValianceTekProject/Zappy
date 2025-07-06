/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GuiCommand
*/

#pragma once

#include "ServerMap.hpp"
#include "TeamsPlayer.hpp"

#include <sstream>

namespace zappy {
    namespace game {
        class MapServer;

        /**
         * @brief Command handler for GUI client communication in the Zappy game
         * 
         * This class handles all GUI protocol commands for the Zappy game server.
         * It processes client inputs from GUI clients and manages the communication
         * between the game server and GUI clients using the Zappy GUI protocol.
         */
        class CommandHandlerGui {
           public:
            /**
                 * @brief Constructor for CommandHandlerGui
                 * 
                 * @param freq Reference to the game frequency (time units per second)
                 * @param width Width of the game map
                 * @param height Height of the game map
                 * @param clientNb Number of clients connected
                 * @param map Reference to the game map server
                 * @param teamList Reference to the list of teams in the game
                 */
            CommandHandlerGui(int &freq, int width, int height, int clientNb,
                zappy::game::MapServer &map,
                std::vector<std::shared_ptr<ITeams>> &teamList)
                : _teamList(teamList), _freq(freq), _widthMap(width),
                  _heightMap(height), _clientNb(clientNb), _map(map) {};

            /**
                 * @brief Default destructor
                 */
            ~CommandHandlerGui() = default;

            /**
                 * @brief Process input from GUI client
                 * 
                 * @param input Reference to the input string from the client
                 * @param player Reference to the server player object
                 */
            virtual void processClientInput(
                std::string &input, zappy::game::ServerPlayer &player);

            /**
                 * @brief Initialize the command map with GUI protocol commands
                 */
            virtual void initCommandMap();

            /**
                 * @brief Command map for GUI protocol commands
                 * 
                 * Maps command strings to their corresponding handler functions
                 */
            std::unordered_map<std::string,
                std::function<void(ServerPlayer &, const std::string &)>>
                _commandMapGui;

            /**
                 * @brief Reference to the list of teams in the game
                 */
            std::vector<std::shared_ptr<ITeams>> &_teamList;

            /**
                 * @brief Handle map size command (msz)
                 * 
                 * Sends the map dimensions to the GUI client.
                 * Response format: "msz X Y\n" where X is width and Y is height
                 * 
                 * @param player Reference to the GUI player requesting map size
                 */
            void handleMsz(zappy::game::ServerPlayer &player);

            /**
                 * @brief Handle tile content command (bct)
                 * 
                 * Sends the content of a specific tile to the GUI client.
                 * Response format: "bct X Y q0 q1 q2 q3 q4 q5 q6\n"
                 * where X,Y are coordinates and q0-q6 are resource quantities
                 * 
                 * @param player Reference to the GUI player requesting tile content
                 * @param arg Arguments containing tile coordinates "X Y"
                 */
            void handleBct(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
                 * @brief Handle map content command (mct)
                 * 
                 * Sends the content of all tiles on the map to the GUI client.
                 * Response format: Multiple "bct X Y q0 q1 q2 q3 q4 q5 q6\n" lines
                 * 
                 * @param player Reference to the GUI player requesting map content
                 */
            void handleMct(zappy::game::ServerPlayer &player);

            /**
                 * @brief Handle team names command (tna)
                 * 
                 * Sends the names of all teams to the GUI client.
                 * Response format: Multiple "tna N\n" lines where N is team name
                 * 
                 * @param player Reference to the GUI player requesting team names
                 */
            void handleTna(zappy::game::ServerPlayer &player);

            /**
                 * @brief Handle player new connection command (pnw)
                 * 
                 * Sends information about a newly connected player to the GUI client.
                 * Response format: "pnw #n X Y O L N\n"
                 * where n=player number, X,Y=position, O=orientation, L=level, N=team name
                 * 
                 * @param gui Reference to the GUI player to notify
                 */
            void handlePnw(zappy::game::ServerPlayer &gui);

            /**
                 * @brief Handle player position command (ppo)
                 * 
                 * Sends a player's current position to the GUI client.
                 * Response format: "ppo #n X Y O\n"
                 * where n=player number, X,Y=position, O=orientation
                 * 
                 * @param player Reference to the GUI player requesting position
                 * @param arg Arguments containing player number "#n"
                 */
            void handlePpo(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
                 * @brief Handle player level command (plv)
                 * 
                 * Sends a player's current level to the GUI client.
                 * Response format: "plv #n L\n"
                 * where n=player number and L=level
                 * 
                 * @param player Reference to the GUI player requesting level
                 * @param arg Arguments containing player number "#n"
                 */
            void handlePlv(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
                 * @brief Handle player inventory command (pin)
                 * 
                 * Sends a player's inventory to the GUI client.
                 * Response format: "pin #n X Y q0 q1 q2 q3 q4 q5 q6\n"
                 * where n=player number, X,Y=position, q0-q6=resource quantities
                 * 
                 * @param player Reference to the GUI player requesting inventory
                 * @param arg Arguments containing player number "#n"
                 */
            void handlePin(
                zappy::game::ServerPlayer &player, const std::string &arg);

            /**
                 * @brief Handle server time unit get command (sgt)
                 * 
                 * Sends the current time unit (frequency) to the GUI client.
                 * Response format: "sgt T\n" where T is the time unit
                 * 
                 * @param player Reference to the GUI player requesting time unit
                 */
            void handleSgt(zappy::game::ServerPlayer &player);

            /**
                 * @brief Handle server time unit set command (sst)
                 * 
                 * Sets the server time unit (frequency) and confirms to the GUI client.
                 * Response format: "sst T\n" where T is the new time unit
                 * 
                 * @param player Reference to the GUI player setting time unit
                 * @param arg Arguments containing new time unit value "T"
                 */
            void handleSst(
                zappy::game::ServerPlayer &player, const std::string &arg);

           protected:
            /**
                     * @brief Reference to the game frequency (time units per second)
                     */
            int &_freq;

            /**
                     * @brief Width of the game map
                     */
            int _widthMap;

            /**
                     * @brief Height of the game map
                     */
            int _heightMap;

            /**
                     * @brief Number of clients connected to the server
                     */
            int _clientNb;

            /**
                     * @brief Reference to the game map server
                     */
            MapServer &_map;

            /**
                     * @brief Command map for protocol commands
                     * 
                     * Maps command strings to their corresponding handler functions
                     */
            std::unordered_map<std::string,
                std::function<void(ServerPlayer &, const std::string &)>>
                _commandMap;
        };
    }  // namespace game
}  // namespace zappy
