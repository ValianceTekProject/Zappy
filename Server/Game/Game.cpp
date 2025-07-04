/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Game
*/

#include "Game.hpp"
#include "Client.hpp"
#include "Server.hpp"
#include "ServerPlayer.hpp"
#include <algorithm>
#include <thread>

const constexpr int nbOrientation = 4;

void zappy::game::Game::_addPlayerToTeam(
    std::shared_ptr<zappy::game::ITeams> team, int clientSocket)
{
    std::srand(std::time({}));
    int randVal = std::rand() % nbOrientation;
    zappy::game::Orientation orientation =
        static_cast<zappy::game::Orientation>(randVal);
    zappy::server::Client user(clientSocket);
    auto itPlayerTeam = std::dynamic_pointer_cast<TeamsPlayer>(team);
    if (itPlayerTeam) {
        zappy::game::Egg egg = this->_map.popEgg();
        this->_commandHandler.messageToGUI(
            "ebo #" + std::to_string(egg.getId()) + "\n");
        this->_commandHandler.messageToGUI(
            "edi #" + std::to_string(egg.getId()) + "\n");
        user.setState(zappy::server::ClientState::CONNECTED);
        auto newPlayer =
            std::make_shared<zappy::game::ServerPlayer>(std::move(user),
                _idPlayerTot, egg.x, egg.y, orientation, *team, 1);
        newPlayer->teamName = team->getName();
        this->_idPlayerTot += 1;
        team->addPlayer(newPlayer);
        this->_playerList.push_back(newPlayer);
        if (auto lastPlayer = this->_playerList.back().lock(); lastPlayer) {
            if (lastPlayer->teamName == "GRAPHIC")
                return;
            for (auto &team : this->_teamList) {
                if (team->getName() == "GRAPHIC") {
                    for (auto &guiPlayer : team->getPlayerList()) {
                        guiPlayer->getClient().sendMessage(
                            "pnw #" + std::to_string(newPlayer->getId()) + " " +
                            std::to_string(newPlayer->x) + " " +
                            std::to_string(newPlayer->y) + " " +
                            std::to_string(static_cast<int>(newPlayer->orientation + 1)) +
                            " " + std::to_string(newPlayer->level) + " " +
                            newPlayer->teamName + "\n");
                    }
                }
            }
        }
        return;
    }
    user.setState(zappy::server::ClientState::CONNECTED);
    auto newPlayer = std::make_shared<zappy::game::ServerPlayer>(
        std::move(user), -1, -1, -1, orientation, *team, 1);
    newPlayer->teamName = team->getName();
    team->addPlayer(newPlayer);
    this->_playerList.push_back(newPlayer);
}

bool zappy::game::Game::checkWin()
{
    constexpr int nbPlayerWinLevel = 6;
    constexpr int winLevel = 8;

    for (auto &team : this->getTeamList()) {
        if (team->getName() == "GRAPHIC")
            continue;
        int nbMaxLevel = 0;
        for (auto &player : team->getPlayerList()) {
            if (player->level >= winLevel)
                nbMaxLevel += 1;
        }
        if (nbMaxLevel >= nbPlayerWinLevel) {
            this->getCommandHandler().messageToGUI("seg " +
                team->getName() + "\n");
            std::cout << "Team " << team->getName() << " has won !" << std::endl;
            return true;
        }
    }
    return false;
}

bool zappy::game::Game::_checkAlreadyInTeam(int clientSocket)
{
    for (const auto &team : this->_teamList) {
        for (const auto &player : team->getPlayerList()) {
            const auto &socket = player->getClient();
            if (socket.getSocket() == clientSocket) {
                return true;
            }
        }
    }
    return false;
}

bool zappy::game::Game::handleTeamJoin(
    int clientSocket, const std::string &teamName)
{
    auto it = std::find_if(this->_teamList.begin(), this->_teamList.end(),
        [&teamName](const std::shared_ptr<zappy::game::ITeams>(team)) {
            return team->getName() == teamName;
        });

    auto itPlayerTeam = std::dynamic_pointer_cast<TeamsPlayer>(*it);
    if (itPlayerTeam) {
        if (it == this->_teamList.end() ||
            static_cast<int>(itPlayerTeam->getPlayerList().size()) >=
                itPlayerTeam->getClientNb()) {
            return false;
        }
    }
    if (this->_checkAlreadyInTeam(clientSocket) == true)
        return false;

    this->_addPlayerToTeam(*it, clientSocket);

    return true;
}

void zappy::game::Game::removeFromTeam(int clientSocket)
{
    for (size_t i = 0; i < this->_teamList.size(); i += 1) {
        auto &team = this->_teamList[i];
        for (auto &player : team->getPlayerList()) {
            if (player->getClient().getSocket() == clientSocket) {
                team->removePlayer(clientSocket);
                return;
            }
        }
    }
}

void zappy::game::Game::foodManager(std::shared_ptr<ITeams> &team)
{
    constexpr int unitLose = 126;
    double secondsBetweenFoodLoss =
        static_cast<double>(unitLose) / this->_baseFreqMs;
    auto loseFood = std::chrono::milliseconds(
        static_cast<int>(secondsBetweenFoodLoss * 1000));
    std::chrono::duration<double> loseFoodSeconds =
        std::chrono::duration<double>(loseFood.count() / 1000.0);

    auto itPlayerTeam = std::dynamic_pointer_cast<TeamsPlayer>(team);

    if (itPlayerTeam) {
        for (auto &player : itPlayerTeam->getPlayerList()) {
            if (!player || player->getClient().getState() == server::ClientState::DISCONNECTED)
                continue;
            if (player->getLifeChrono() >= loseFoodSeconds) {
                if (player->getInventory().getResourceQuantity(
                        zappy::game::Resource::FOOD) > 0) {
                    player->dropRessource(zappy::game::Resource::FOOD);
                    for (auto &teams : _teamList) {
                        if (teams->getName() == "GRAPHIC") {
                            for (auto players : teams->getPlayerList())
                                this->getCommandHandlerGui().handlePin(
                                    *players, std::to_string(player->getId()));
                        }
                    }
                    player->resetLifeChrono();
                } else {
                    std::cout << "Death of player: " << player->getId() << std::endl;
                    player->getClient().sendMessage("dead\n");
                    player->getTeam().removePlayer(player->getClient().getSocket());
                    this->_commandHandler.messageToGUI(
                        "pdi #" + std::to_string(player->getId()) + "\n");
                    itPlayerTeam->removePlayer(
                        player->getClient().getSocket());
                }
            }
        }
    }
}

void zappy::game::Game::runGame()
{
    this->_isRunning = RunningState::RUN;
    auto lastUpdate = std::chrono::steady_clock::now();
    auto lastResourceRespawn = std::chrono::steady_clock::now();

    while (this->_isRunning != RunningState::STOP) {
        auto respawnInterval = std::chrono::duration<double>(
            static_cast<double>(TIME_BEFORE_RESPAWN) / this->_baseFreqMs);
        auto now = std::chrono::steady_clock::now();
        auto elapsedTurn = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastUpdate);
        auto elapsedRespawn = std::chrono::duration_cast<std::chrono::duration<double>>(now - lastResourceRespawn);

        if (this->checkWin())
            this->setRunningState(zappy::RunningState::STOP);
        for (auto &team : this->getTeamList()) {
            foodManager(team);
            for (auto &player : team->getPlayerList()) {
                if (!player->getClient().queueMessage.empty()) {
                    if (player->teamName == "GRAPHIC") {
                        this->_commandHandlerGui.processClientInput(
                            player->getClient().queueMessage.front(), *player);
                    } else {
                        this->_commandHandler.processClientInput(
                            player->getClient().queueMessage.front(), *player);
                    }
                }
            }
        }

        if (elapsedTurn >= std::chrono::milliseconds(this->_baseFreqMs)) {
            this->_playTurn();
            lastUpdate = now;
        }

        if (elapsedRespawn >= respawnInterval) {
            this->_map.replaceResources();
            lastResourceRespawn = now;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
}
