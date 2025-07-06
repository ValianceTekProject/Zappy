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
#include <string>
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

void zappy::game::CommandHandler::_sendExpulseMsg(
    ServerPlayer &player, ServerPlayer &pushingPlayer)
{
    int pushingDx = 0;
    int pushingDy = 0;
    this->_getDirectionVector(pushingPlayer, pushingDx, pushingDy);

    int ejectedDx = 0;
    int ejectedDy = 0;
    this->_getDirectionVector(player, ejectedDx, ejectedDy);

    int directionPushed = 0;
    if (pushingDx == 0 && pushingDy == -1)
        directionPushed = 1;
    if (pushingDx == 1 && pushingDy == 0)
        directionPushed = 3;
    if (pushingDx == 0 && pushingDy == 1)
        directionPushed = 5;
    if (pushingDx == -1 && pushingDy == 0)
        directionPushed = 7;
    std::string expulseMsg = "eject: " + std::to_string(directionPushed);
    player.getClient().sendMessage(expulseMsg);
}

void zappy::game::CommandHandler::ejectPlayerForward(ServerPlayer &player,
    Orientation &orientation, ServerPlayer &pushingPlayer)
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

    this->_sendExpulseMsg(player, pushingPlayer);
}

void zappy::game::CommandHandler::handleEject(
    zappy::game::ServerPlayer &player)
{
    if (this->_waitCommand(player, timeLimit::EJECT) == false)
        return;
    auto playerOrientation = player.orientation;

    auto playerList = this->_getPlayerOnTile(player.x, player.y);
    for (auto &playerOnTile : playerList) {
        auto playerOnTileUnlock = playerOnTile.lock();
        if (playerOnTileUnlock &&
            player.getId() != playerOnTileUnlock->getId()) {
            playerOnTileUnlock->interrupted = true;
            ejectPlayerForward(*playerOnTileUnlock, playerOrientation, player);

            std::string expulseMsgGui =
                "pex " + std::to_string(playerOnTileUnlock->getId());
            messageToGUI(expulseMsgGui);
        }
    }
}
