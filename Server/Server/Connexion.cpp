/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientConnexion
*/

#include "Client.hpp"
#include "Game.hpp"
#include "Server.hpp"
#include "algorithm"
#include <mutex>

void zappy::server::Server::handleClientMessage(
    int clientSocket, std::string buffer)
{
    for (auto &team : this->_game->getTeamList()) {
        for (auto &player : team->getPlayerList()) {
            if (clientSocket == player->getClient().getSocket() &&
                player->getClient().getState() == zappy::server::ClientState::CONNECTED) {
                std::lock_guard<std::mutex> lock(*(player->getClient().queueMutex));
                player->getClient().queueMessage.push(buffer);
                return;
            }
        }
    }
    this->sendMessage("ko\n", clientSocket);
}

zappy::server::ClientState zappy::server::Server::_handleClientDisconnection(
    const std::string &content, struct pollfd &pfd)
{
    if (content.compare("exit") == 0) {
        auto optPlayer = this->getPlayerBySocket(pfd.fd);
        if (optPlayer.has_value() && optPlayer.value()->getTeam().getName() != "GRAPHIC") {
            this->_game->getCommandHandler().messageToGUI("pdi #" +
                std::to_string(optPlayer.value()->getId()) + "\n");
        }
        this->_game->removeFromTeam(pfd.fd);
        ::close(pfd.fd);
        return ClientState::DISCONNECTED;
    }
    return ClientState::UNDEFINED;
}

bool zappy::server::Server::_handleNewConnection(struct pollfd &pfd)
{
    std::lock_guard<std::mutex> lock(this->_socketLock);
    if (pfd.fd == this->_socket->getSocket()) {
        this->_fds.push_back(this->_socket->acceptConnection());
        return true;
    }
    return false;
}

void zappy::server::Server::pfdLoop()
{
    for (size_t i = 0; i < this->_fds.size();) {
        auto &pfd = this->_fds[i];

        if (pfd.revents & POLLIN) {
            try {
                if (this->_handleNewConnection(pfd)) {
                    ++i;
                    continue;
                }
                auto content = this->_getClientCommand(pfd);

                if (this->_handleClientDisconnection(content, pfd) ==
                    ClientState::DISCONNECTED) {
                    this->_fds.erase(this->_fds.begin() + i);
                    continue;
                }

                this->_handleClientCommand(content, pfd);

            } catch (const zappy::error::SocketError &e) {
                close(pfd.fd);
                this->_game->removeFromTeam(pfd.fd);
                this->_fds.erase(this->_fds.begin() + i);
                continue;
            }
        }

        ++i;
    }
}


void zappy::server::Server::runLoop()
{
    auto signalHandler = std::make_shared<zappy::utils::Signal>(*this, *_game);
    zappy::utils::Signal::initSignalHandling(signalHandler.get());

    while (this->_serverRun == RunningState::RUN) {
        this->_socket->getData(this->_fds);

        if (this->_game->getRunningState() == RunningState::STOP)
            this->setRunningState(RunningState::STOP);
        this->pfdLoop();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
