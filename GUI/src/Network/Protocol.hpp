/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Protocol.hpp
*/

#pragma once

#include "GuiProtocol.hpp"
#include "NetworkManager.hpp"
#include "IRenderer.hpp"

#include <memory>
#include <functional>
#include <sstream>
#include <chrono>
#include <thread>

namespace zappy {
    namespace network {
        class Protocol {
            public:
                explicit Protocol(
                    std::shared_ptr<gui::IRenderer> renderer,
                    std::shared_ptr<game::GameState> gameState,
                    bool debug = false
                );
                ~Protocol();

                bool connectToServer(const std::string& host, int port);
                void disconnect();
                bool isConnected() const;
                void update();

                void requestMapSize();
                void requestTileContent(int x, int y);
                void requestMapContent();
                void requestTeamNames();
                void requestPlayerPosition(int playerId);
                void requestPlayerLevel(int playerId);
                void requestPlayerInventory(int playerId);
                void requestTimeUnit();
                void setTimeUnit(int timeUnit);

            private:
                void removeSharp(std::string &message);
                // string / outputs stream default cout
                void printDebug(const std::string &message, std::ostream &stream = std::cout);

                // Message handlers
                void handleMapSize(const std::string &params);
                void handleTileContent(const std::string &params);
                // void handleMapContent(const std::string &params);
                void handleTeamNames(const std::string &params);
                void handleNewPlayer(const std::string &params);
                void handlePlayerPosition(const std::string &params);
                void handlePlayerLevel(const std::string &params);
                void handlePlayerInventory(const std::string &params);
                void handlePlayerExpulsion(const std::string &params);
                void handlePlayerBroadcast(const std::string &params);
                void handleIncantationStart(const std::string &params);
                void handleIncantationEnd(const std::string &params);
                void handleEggLaying(const std::string &params);
                void handleResourceDrop(const std::string &params);
                void handleResourceCollect(const std::string &params);
                void handlePlayerDeath(const std::string &params);
                void handleEggCreated(const std::string &params);
                void handleEggHatch(const std::string &params);
                void handleEggDeath(const std::string &params);
                void handleTimeUnit(const std::string &params);
                void handleGameEnd(const std::string &params);
                void handleServerMessage(const std::string &params);
                void handleUnknownCommand(const std::string &params);
                void handleBadCommand(const std::string &params);

                void initHandlers();
                void initRequestsCommands();
                void onServerMessage(const ServerMessage &msg);

                bool _debug;

                std::unique_ptr<NetworkManager> _network;
                std::shared_ptr<gui::IRenderer> _renderer;
                std::shared_ptr<game::GameState> _gameState;
                bool _authenticated;

                using HandlerFunc = std::function<void(const std::string &)>;
                std::unordered_map<GuiProtocol, HandlerFunc> _handlers;
                std::unordered_map<GuiProtocol, std::string> _requestsCommands;
        };
    } // namespace network
} // namespace zappy
