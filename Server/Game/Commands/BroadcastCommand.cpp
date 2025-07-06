/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** BroadcastCommand
*/

#include "ClientCommand.hpp"

std::pair<int, int> zappy::game::CommandHandler::_computeBroadcastDistance(
    int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dx_round = 0;
    if (dx > 0)
        dx_round = dx - static_cast<int>(this->_widthMap);
    else
        dx_round = dx + static_cast<int>(this->_widthMap);
    if (std::abs(dx_round) < std::abs(dx))
        dx = dx_round;

    int dy = y2 - y1;
    int dy_round = 0;
    if (dy > 0)
        dy_round = dy - static_cast<int>(this->_heightMap);
    else
        dy_round = dy + static_cast<int>(this->_heightMap);
    if (std::abs(dy_round) < std::abs(dy))
        dy = dy_round;

    return {dx, dy};
}

int zappy::game::CommandHandler::_getSoundCardinalPoint(
    int relativeX, int relativeY)
{
    if (relativeY < 0) {
        if (relativeX < 0)
            return static_cast<int>(SoundDirection::NORTHWEST);
        else if (relativeX > 0)
            return static_cast<int>(SoundDirection::NORTHEAST);
        else
            return static_cast<int>(SoundDirection::NORTH);
    } else if (relativeY > 0) {
        if (relativeX < 0)
            return static_cast<int>(SoundDirection::SOUTHWEST);
        else if (relativeX > 0)
            return static_cast<int>(SoundDirection::SOUTHEAST);
        else
            return static_cast<int>(SoundDirection::SOUTH);
    } else {
        if (relativeX < 0)
            return static_cast<int>(SoundDirection::WEST);
    }
    return static_cast<int>(SoundDirection::EAST);
}

int zappy::game::CommandHandler::_computeSoundDirection(
    const ServerPlayer &player, const ServerPlayer &receiver)
{
    if (player.x == receiver.x && player.y == receiver.y)
        return static_cast<int>(SoundDirection::SAME_POSITION);

    auto [dx, dy] = this->_computeBroadcastDistance(
        static_cast<int>(receiver.x), static_cast<int>(receiver.y),
        static_cast<int>(player.x), static_cast<int>(player.y));

    int relativeX = 0;
    int relativeY = 0;

    switch (receiver.orientation) {
        case Orientation::NORTH:
            relativeX = dx;
            relativeY = dy;
            break;
        case Orientation::EAST:
            relativeX = -dy;
            relativeY = dx;
            break;
        case Orientation::SOUTH:
            relativeX = -dx;
            relativeY = -dy;
            break;
        case Orientation::WEST:
            relativeX = dy;
            relativeY = -dx;
            break;
    }
    return this->_getSoundCardinalPoint(relativeX, relativeY);
}

void zappy::game::CommandHandler::handleBroadcast(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    if (this->_waitCommand(player, timeLimit::BROADCAST) == false)
        return;

    for (auto &team : this->_teamList) {
        for (auto &teamPlayer : team->getPlayerList()) {
            if (teamPlayer->getClient().getSocket() !=
                player.getClient().getSocket()) {
                int direction =
                    this->_computeSoundDirection(player, *teamPlayer);
                std::string broadcastMsg =
                    "message " + std::to_string(direction) + ", " + arg + "\n";
                teamPlayer->getClient().sendMessage(broadcastMsg);
            }
        }
    }
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    this->messageToGUI(std::string(
        "pbc #" + std::to_string(player.getId()) + " " + arg + "\n"));
}
