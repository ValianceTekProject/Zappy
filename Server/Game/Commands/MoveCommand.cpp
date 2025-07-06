/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MoveCommand
*/

#include "ClientCommand.hpp"

void zappy::game::CommandHandler::handleForward(
    zappy::game::ServerPlayer &player)
{
    if (!this->_waitCommand(player, timeLimit::FORWARD))
        return;
    player.stepForward(this->_widthMap, this->_heightMap);
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    std::string msg =
        "ppo #" + std::to_string(player.getId()) + " " +
        std::to_string(player.x) + " " + std::to_string(player.y) + " " +
        std::to_string(static_cast<int>(player.orientation) + 1) + "\n";
    this->messageToGUI(msg);
}

void zappy::game::CommandHandler::handleRight(
    zappy::game::ServerPlayer &player)
{
    if (!this->_waitCommand(player, timeLimit::RIGHT))
        return;
    player.lookRight();
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    std::string msg =
        "ppo #" + std::to_string(player.getId()) + " " +
        std::to_string(player.x) + " " + std::to_string(player.y) + " " +
        std::to_string(static_cast<int>(player.orientation) + 1) + "\n";
    this->messageToGUI(msg);
}

void zappy::game::CommandHandler::handleLeft(zappy::game::ServerPlayer &player)
{
    if (!this->_waitCommand(player, timeLimit::LEFT))
        return;
    player.lookLeft();
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    std::string msg =
        "ppo #" + std::to_string(player.getId()) + " " +
        std::to_string(player.x) + " " + std::to_string(player.y) + " " +
        std::to_string(static_cast<int>(player.orientation) + 1) + "\n";
    this->messageToGUI(msg);
}
