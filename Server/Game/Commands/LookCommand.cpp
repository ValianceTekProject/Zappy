/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** LookCommand
*/

#include "ClientCommand.hpp"

std::pair<size_t, size_t> zappy::game::CommandHandler::_normalizeCoords(
    size_t x, size_t y)
{
    x = ((x % this->_widthMap) + this->_widthMap) % this->_widthMap;
    y = ((y % this->_heightMap) + this->_heightMap) % this->_heightMap;
    return {static_cast<size_t>(x), static_cast<size_t>(y)};
}

void zappy::game::CommandHandler::_getDirectionVector(
    const Player &player, int &dx, int &dy)
{
    Orientation orientation = player.orientation;

    switch (orientation) {
        case Orientation::NORTH:
            dx = 0;
            dy = -1;
            break;
        case Orientation::EAST:
            dx = 1;
            dy = 0;
            break;
        case Orientation::SOUTH:
            dx = 0;
            dy = 1;
            break;
        case Orientation::WEST:
            dx = -1;
            dy = 0;
            break;
    }
}

std::pair<int, int> zappy::game::CommandHandler::_computeLookTarget(
    zappy::game::ServerPlayer &player, size_t line, int offset)
{
    Orientation orientation = player.orientation;
    size_t playerX = player.x;
    size_t playerY = player.y;
    int dx = 0;
    int dy = 0;

    this->_getDirectionVector(player, dx, dy);

    int targetX =
        static_cast<int>(playerX) + dx * static_cast<int>(line) +
        (orientation == Orientation::NORTH || orientation == Orientation::SOUTH
                ? offset
                : (dy * offset));

    int targetY =
        static_cast<int>(playerY) + dy * static_cast<int>(line) +
        (orientation == Orientation::EAST || orientation == Orientation::WEST
                ? offset
                : (-dx * offset));

    return {targetX, targetY};
}

std::string zappy::game::CommandHandler::_getTileContent(
    size_t x, size_t y, bool isPlayerTile)
{
    std::string content = "";
    auto &tile = this->_map.getTile(x, y);
    bool hasContent = false;

    auto playerList = this->_getPlayerOnTile(x, y);
    for (auto it: playerList) {
        content += " player";
    }
    for (const auto &resourceName : names) {
        zappy::game::Resource resource = getResource(resourceName);
        size_t quantity = tile.getResourceQuantity(resource);

        for (size_t i = 0; i < quantity; ++i) {
            if (isPlayerTile) {
                content += " " + resourceName;
                continue;
            }
            if (hasContent)
                content += " ";
            content += resourceName;
            hasContent = true;
        }
    }

    return content;
}

bool zappy::game::CommandHandler::_checkLastTileInLook(
    size_t playerLevel, size_t line, int offset)
{
    return (line == playerLevel && offset == static_cast<int>(line));
}

std::string zappy::game::CommandHandler::_lookLine(
    zappy::game::ServerPlayer &player, size_t line)
{
    std::string lineMsg = "";

    for (int offset = static_cast<int>(line) * -1;
        offset <= static_cast<int>(line); offset += 1) {
        auto [targetX, targetY] =
            this->_computeLookTarget(player, line, offset);
        auto [normalizedX, normalizedY] =
            this->_normalizeCoords(targetX, targetY);

        std::string tileContent = this->_getTileContent(
            normalizedX, normalizedY, line == 0 && offset == 0);
        lineMsg += tileContent;

        if (!this->_checkLastTileInLook(player.level, line, offset))
            lineMsg += ",";
    }

    return lineMsg;
}

std::string zappy::game::CommandHandler::_buildLookMessage(
    zappy::game::ServerPlayer &player)
{
    std::string msg = "[";
    size_t playerLevel = player.level;

    for (size_t line = 0; line <= playerLevel; line += 1) {
        std::string lineMsg = this->_lookLine(player, line);
        msg += lineMsg;
    }

    msg += "]\n";
    return msg;
}

void zappy::game::CommandHandler::handleLook(zappy::game::ServerPlayer &player)
{
    if (!this->_waitCommand(player, timeLimit::LOOK))
        return;
    std::string msg = this->_buildLookMessage(player);

    player.setInAction(false);
    player.getClient().sendMessage(msg);
}
