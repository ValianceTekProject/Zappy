/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** PlayerCommand
*/

#include "ClientCommand.hpp"

void zappy::game::CommandHandler::handleConnectNbr(
    zappy::game::ServerPlayer &player)
{
    auto playerTeam =
        dynamic_cast<zappy::game::TeamsPlayer *>(&player.getTeam());
    if (playerTeam) {
        int connectNbr =
            playerTeam->getClientNb() - playerTeam->getPlayerList().size();
        player.setInAction(false);
        player.getClient().sendMessage(std::to_string(connectNbr) + "\n");
    }
}

void zappy::game::CommandHandler::handleFork(zappy::game::ServerPlayer &player)
{
    this->messageToGUI("pfk #" + std::to_string(player.getId()) + "\n");
    this->_waitCommand(timeLimit::FORK);

    auto playerTeam =
        dynamic_cast<zappy::game::TeamsPlayer *>(&player.getTeam());
    if (playerTeam) {
        playerTeam->allowNewPlayer();
        std::lock_guard<std::mutex> eggLock (this->_map._eggMutex);
        auto eggId = this->_map.addNewEgg(playerTeam->getTeamId(), player.x, player.y);
        player.setInAction(false);
        this->messageToGUI(
            "enw #" + std::to_string(eggId) + " #" +
            std::to_string(player.getId()) + " " + std::to_string(player.x) +
            " " + std::to_string(player.y) + "\n");
        player.getClient().sendMessage("ok\n");
    }
}
