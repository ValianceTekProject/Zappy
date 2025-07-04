/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server
*/

#pragma once

#include <csignal>
#include <functional>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <optional>

#include "Client/Client.hpp"
#include "SocketServer.hpp"
#include "Error/Error.hpp"
#include "Game.hpp"
#include "my_macros.hpp"
#include "Utils.hpp"
#include "TeamsGui.hpp"

namespace zappy {

    namespace server {

        class Server {
           public:
            Server(int argc, char const *argv[]);
            ~Server() = default;

            void runServer();

            int getWidth() const { return this->_width; }
            int getHeight() const { return this->_height; }
            int getClientNb() const { return this->_clientNb; }
            int getFreq() const { return this->_freq; }

            void runLoop();
            void pfdLoop();
            void handleClientMessage(int clientSocket, std::string buffer);

            void attachObserver(std::shared_ptr<zappy::observer::IObserver> observer);
            void notifyObservers(int sig);

            void setRunningState(RunningState state) { _serverRun = state; }
            void clearTeams() { _teamList.clear(); }

            void sendMessage(const std::string &buf, int socket) { send(socket, buf.c_str(), buf.size(), 0); }

            std::optional<std::shared_ptr<zappy::game::ServerPlayer>> getPlayerBySocket(const int &socket);

           private:
            std::vector<std::shared_ptr<zappy::observer::IObserver>> _observers;
            std::unique_ptr<zappy::game::Game> _game = nullptr;
            std::unique_ptr<server::SocketServer> _socket = nullptr;

            RunningState _serverRun = RunningState::RUN;

            std::vector<struct pollfd> _fds;

            std::vector<std::shared_ptr<zappy::game::ITeams>> _teamList;
            std::unordered_map<std::string, std::function<void(int)>> _flags;

            std::mutex _socketLock;
            std::mutex _endLock;

            int _port = noValue;
            int _width = noValue;
            int _height = noValue;
            int _clientNb = noValue;
            int _freq = noValue;
            std::vector<std::string> _namesTeam;

            void _parseFlags(int argc, char const *argv[]);
            void _parseName(int &index, char const *argv[]);
            void _parseFlagsInt(int &index, std::string arg, std::string value);
            void _checkParams();

            bool _handleNewConnection(struct pollfd &pfd);
            std::string _getClientCommand(const struct pollfd &fd);
            ClientState _handleClientDisconnection(const std::string &content, struct pollfd &fd);
            void _handleClientCommand(const std::string &command, struct pollfd &pfd);
            void _playerConnect(std::shared_ptr<zappy::game::ITeams> &team, struct pollfd &pfd);
            void _guiConnect(std::shared_ptr<zappy::game::ITeams> &team);
            void _initialCommandGui(std::shared_ptr<zappy::game::TeamsGui> &teamsGui);

        };

    }  // namespace server
}  // namespace zappy
