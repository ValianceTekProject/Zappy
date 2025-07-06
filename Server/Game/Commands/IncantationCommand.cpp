/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IncantationCommand
*/

#include "ClientCommand.hpp"

std::vector<std::weak_ptr<zappy::game::ServerPlayer>>
zappy::game::CommandHandler::_getPlayersForIncant(int x, int y, size_t level)
{
    std::vector<std::weak_ptr<ServerPlayer>> players;

    for (auto &team : this->_teamList) {
        for (auto &player : team->getPlayerList()) {
            if (player->x == x && player->y == y && player->level == level) {
                players.push_back(player);
            }
        }
    }
    return players;
}

std::vector<std::weak_ptr<zappy::game::ServerPlayer>>
zappy::game::CommandHandler::_getPlayersIncanting(int x, int y, size_t level)
{
    std::vector<std::weak_ptr<ServerPlayer>> players;

    for (auto &team : this->_teamList) {
        for (auto &player : team->getPlayerList()) {
            if (player->x == x && player->y == y && player->isPraying() &&
                player->level == level) {
                players.push_back(player);
            }
        }
    }
    return players;
}

bool zappy::game::CommandHandler::_checkIncantationResources(
    size_t x, size_t y, size_t level)
{
    if (level < minLevel || level >= maxLevel)
        return false;

    auto &tile = this->_map.getTile(x, y);
    const auto &requirements = elevationRequirements[level - 1];

    return tile.getResourceQuantity(Resource::LINEMATE) >=
               requirements.linemate &&
           tile.getResourceQuantity(Resource::DERAUMERE) >=
               requirements.deraumere &&
           tile.getResourceQuantity(Resource::SIBUR) >= requirements.sibur &&
           tile.getResourceQuantity(Resource::MENDIANE) >=
               requirements.mendiane &&
           tile.getResourceQuantity(Resource::PHIRAS) >= requirements.phiras &&
           tile.getResourceQuantity(Resource::THYSTAME) >=
               requirements.thystame;
}

bool zappy::game::CommandHandler::_checkIncantationConditions(
    const zappy::game::ServerPlayer &player)
{
    const auto &requirements = elevationRequirements[player.level - 1];

    if (player.level >= maxLevel) {
        return false;
    }
    auto players =
        this->_getPlayersForIncant(player.x, player.y, player.level);

    if (players.size() < requirements.players) {
        return false;
    }
    return this->_checkIncantationResources(player.x, player.y, player.level);
}

void zappy::game::CommandHandler::_consumeElevationResources(
    size_t x, size_t y, size_t level)
{
    std::lock_guard<std::mutex> lock(this->_map._resourceMutex);
    auto &tile = this->_map.getTile(x, y);
    const auto &req = elevationRequirements[level - 1];

    for (size_t i = 0; i < req.linemate; i += 1)
        tile.removeResource(Resource::LINEMATE);
    for (size_t i = 0; i < req.deraumere; i += 1)
        tile.removeResource(Resource::DERAUMERE);
    for (size_t i = 0; i < req.sibur; i += 1)
        tile.removeResource(Resource::SIBUR);
    for (size_t i = 0; i < req.mendiane; i += 1)
        tile.removeResource(Resource::MENDIANE);
    for (size_t i = 0; i < req.phiras; i += 1)
        tile.removeResource(Resource::PHIRAS);
    for (size_t i = 0; i < req.thystame; i += 1)
        tile.removeResource(Resource::THYSTAME);
}

void zappy::game::CommandHandler::_setPrayer(zappy::game::ServerPlayer &player)
{
    auto playersOnTile =
        this->_getPlayersForIncant(player.x, player.y, player.level);
    std::for_each(playersOnTile.begin(), playersOnTile.end(),
        [](std::weak_ptr<ServerPlayer> playerOnTile) {
            auto sharedPlayer = playerOnTile.lock();
            if (!sharedPlayer)
                throw GameError("Unable to lock weak ptr", "Allowing praying");
            if (sharedPlayer->isInAction())
                return;
            sharedPlayer->setInAction(true);
            sharedPlayer->pray();
            sharedPlayer->getClient().sendMessage("Elevation underway\n");
        });
}

void zappy::game::CommandHandler::_elevatePlayer(zappy::game::ServerPlayer &player)
{
    auto playersOnTile = this->_getPlayersIncanting(player.x, player.y, player.level);
    
    for (auto &playerPtr : playersOnTile) {
        auto sharedPlayer = playerPtr.lock();
        if (!sharedPlayer || !sharedPlayer->isPraying()) {
            continue;
        }
        sharedPlayer->level += 1;
        sharedPlayer->stopPraying();
        sharedPlayer->setInAction(false);

        std::string msg = std::string("Current level: ") + 
                         std::to_string(sharedPlayer->level) + "\n";
        sharedPlayer->getClient().sendMessage(msg);
        
        std::cout << "Player " << sharedPlayer->getClient().getSocket() 
                 << " elevated to level " << sharedPlayer->level << std::endl;
    }
}

void zappy::game::CommandHandler::incantationPrinting(zappy::game::ServerPlayer &player)
{
    std::string guiMsg = "pic " + std::to_string(player.x) + " " +
                         std::to_string(player.y) + " " +
                         std::to_string(player.level);
    auto otherPlayers =
        this->_getPlayersForIncant(player.x, player.y, player.level);

    std::for_each(otherPlayers.begin(), otherPlayers.end(),
        [&guiMsg](std::weak_ptr<ServerPlayer> player) {
            auto sharedPlayer = player.lock();
            if (!sharedPlayer->isPraying())
                return;
            guiMsg += " #";
            guiMsg += sharedPlayer->getId();
        });
    guiMsg += "\n";
    this->messageToGUI(guiMsg);
    if (!this->_waitCommand(player, timeLimit::INCANTATION))
        return;
    if (!this->_checkIncantationConditions(player)) {
        this->messageToGUI(
            std::string("pie " + std::to_string(player.x) + " " +
                        std::to_string(player.y) + " 0\n"));
        player.setInAction(false);
        player.stopPraying();
        return player.getClient().sendMessage("ko\n");
    }
    player.pray();}

void zappy::game::CommandHandler::handleIncantation(
    zappy::game::ServerPlayer &player)
{
    std::cout << "Start incantation" << std::endl;
    if (!this->_checkIncantationConditions(player)) {
        std::cout << "Condition not good for incant" << std::endl;
        this->messageToGUI(
            std::string("pie " + std::to_string(player.x) + " " +
                        std::to_string(player.y) + " 0\n"));
        return player.getClient().sendMessage("ko\n");
    }
    this->_setPrayer(player);
    this->incantationPrinting(player);
    this->messageToGUI(std::string("pie " + std::to_string(player.x) + " " +
                                   std::to_string(player.y) + " 1\n"));
    this->messageToGUI(std::string("plv #" + std::to_string(player.getId()) + " " +
                                   std::to_string(player.level + 1) + "\n"));
    this->_consumeElevationResources(player.x, player.y, player.level);
    this->messageToGUI(std::string("pie " + std::to_string(player.x) + " " +
                                   std::to_string(player.y) + " 1\n"));
    this->_elevatePlayer(player);
    player.setInAction(false);
    player.stopPraying();
    std::cout << "SERV: Incantation Réussi !: " << player.isPraying() << player.isInAction() << std::endl;
}
