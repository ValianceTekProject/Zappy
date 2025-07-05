//
// EPITECH PROJECT, 2025
// Zappy
// File description:
// Eject
//

#include "ClientCommand.hpp"
#include "Orientation.hpp"
#include "ServerPlayer.hpp"
#include <algorithm>
#include <memory>
#include <vector>

std::vector<std::weak_ptr<zappy::game::ServerPlayer>>
zappy::game::CommandHandler::_getPlayerOnTile(int x, int y)
{
    std::vector<std::weak_ptr<ServerPlayer>> playerList;

    auto retrievePlayerFromTeam = [&playerList, x, y](
                                      std::weak_ptr<ITeams> team) {
        auto teamUnlock = team.lock();
        if (teamUnlock && teamUnlock->getName() != "GRAPHIC") {
            for (auto player : teamUnlock->getPlayerList()) {
                if (player && player->x == x && player->y == y)
                    playerList.push_back(player);
            }
        }
    };
    std::for_each(this->_teamList.begin(), this->_teamList.end(),
        retrievePlayerFromTeam);

    return playerList;
}

void zappy::game::CommandHandler::ejectPlayerForward(ServerPlayer &player, Orientation &orientation)
{
    switch (orientation) {
        case Orientation::NORTH:
            player.y--;
            break;
        case Orientation::EAST:
            player.x++;
            break;
        case Orientation::SOUTH:
            player.y++;
            break;
        case Orientation::WEST:
            player.x--;
            break;
    }

    int width = this->_map.getWidth();
    int height = this->_map.getHeight();

    if (player.x >= width || player.x <= 0)
        player.x = (player.x + width) % width;
    if (player.y >= height || player.y <= 0)
        player.y = (player.y + height) % height;
}

void zappy::game::CommandHandler::handleEject(
    zappy::game::ServerPlayer &player)
{
    this->_waitCommand(timeLimit::EJECT);
    auto playerOrientation = player.orientation;

    auto playerList = this->_getPlayerOnTile(player.x, player.y);
    for (auto &playerOnTile: playerList) {
        auto playerOnTileUnlock = playerOnTile.lock();
        if (playerOnTileUnlock && player.getId() != playerOnTileUnlock->getId())
            ejectPlayerForward(*playerOnTileUnlock, playerOrientation);
    }
}
