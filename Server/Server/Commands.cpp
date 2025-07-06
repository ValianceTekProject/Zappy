//
// EPITECH PROJECT, 2025
// Commands
// File description:
// Handle commands for server
//

#include "Error.hpp"
#include "Server.hpp"
#include "my_macros.hpp"

std::string zappy::server::Server::_getClientCommand(const struct pollfd &pfd)
{
    constexpr short buffSize = 1024;

    char buffer[buffSize] = {0};

    auto readValue = read(pfd.fd, buffer, sizeof(buffer));
    if (readValue == -1)
        throw error::SocketError("Unable to read client command");
    std::string content(buffer);
    content.erase(content.find_last_not_of(endSequence) + 1);
    return content;
}

void zappy::server::Server::_playerConnect(std::shared_ptr<zappy::game::ITeams> &team,
    struct pollfd &pfd)
{
    auto teamsPlayer =
        std::dynamic_pointer_cast<zappy::game::TeamsPlayer>(team);
    if (teamsPlayer) {
        std::string msg =
            std::to_string(teamsPlayer->getClientNb() -
                            teamsPlayer->getPlayerList().size()) +
            "\n";
        msg += std::to_string(this->_width) + " " +
                std::to_string(this->_height);
        this->_socket->sendMessage(pfd.fd, msg);
        return;
    }
}

void zappy::server::Server::_initialCommandGui(std::shared_ptr<zappy::game::TeamsGui> &teamsGui)
{
    this->_game->getCommandHandlerGui().handleMsz(
        *teamsGui->getPlayerList().back());
    this->_game->getCommandHandlerGui().handleSgt(
        *teamsGui->getPlayerList().back());
    this->_game->getCommandHandlerGui().handleMct(
        *teamsGui->getPlayerList().back());
    this->_game->getCommandHandlerGui().handleTna(
        *teamsGui->getPlayerList().back());
    this->_game->getCommandHandlerGui().handlePnw(
        *teamsGui->getPlayerList().back());
    for (auto &teams : this->_game->getTeamList()) {
        if (teams->getName() != "GRAPHIC") {
            for (auto &players : teams->getPlayerList()) {
                this->_game->getCommandHandlerGui().handlePin(
                    *teamsGui->getPlayerList().back(),
                    std::to_string(players->getId()));
                this->_game->getCommandHandlerGui().handlePlv(
                    *teamsGui->getPlayerList().back(),
                    std::to_string(players->getId()));
            }
        }
    }
}

void zappy::server::Server::_guiConnect(std::shared_ptr<zappy::game::ITeams> &team)
{
    auto teamsGui =
        std::dynamic_pointer_cast<zappy::game::TeamsGui>(team);
    if (teamsGui) {
        _initialCommandGui(teamsGui);
        std::lock_guard<std::mutex> lock(
            this->_game->getMap()._eggMutex);
        for (auto &eggs : this->_game->getMap().getEggList())
            this->sendMessage(
                std::string("enw #" +
                            std::to_string(eggs.getId()) + " -1 " +
                            std::to_string(eggs.x) + " " +
                            std::to_string(eggs.y) + "\n"),
                teamsGui->getPlayerList()
                    .back()
                    ->getClient()
                    .getSocket());
        return;
    }
}

void zappy::server::Server::_handleClientCommand(
    const std::string &command, struct pollfd &pfd)
{
    for (auto &team : this->_game->getTeamList()) {
        if (command.compare(team->getName()) == 0) {
            bool hasJoin =
                this->_game->handleTeamJoin(pfd.fd, team->getName());
            if (hasJoin) {
                this->_playerConnect(team, pfd);
                this->_guiConnect(team);
                return;
            }
            this->_socket->sendMessage(pfd.fd, "Invalid team");
            return;
        }
    }
    handleClientMessage(pfd.fd, command);
}
