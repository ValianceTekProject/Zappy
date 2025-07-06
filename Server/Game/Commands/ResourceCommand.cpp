/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ResourceCommand
*/

#include "ClientCommand.hpp"

void zappy::game::CommandHandler::handleInventory(
    zappy::game::ServerPlayer &player)
{
    if (!this->_waitCommand(player, timeLimit::INVENTORY))
        return;
    zappy::game::Inventory playerInv = player.getInventory();
    std::string msg = "[";
    for (auto foodName : names)
        msg += foodName + " " +
               std::to_string(
                   playerInv.getResourceQuantity(getResource(foodName))) +
               ",";
    msg.pop_back();
    msg += "]\n";
    player.setInAction(false);
    player.getClient().sendMessage(msg);
}

void zappy::game::CommandHandler::resourceSendGui(zappy::game::ServerPlayer &player)
{
    for (auto &team : this->_teamList) {
        if (team->getName() == "GRAPHIC") {
            for (auto &players : team->getPlayerList()) {
                handlePin(*players, std::to_string(player.getId()));
                handleBct(*players, std::string(std::to_string(player.x)) +
                " " + std::string(std::to_string(player.y)));
            }
        }
    }
}

void zappy::game::CommandHandler::handleTake(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    if (!this->_waitCommand(player, timeLimit::TAKE))
        return;
    auto objectTake = std::find_if(names.begin(), names.end(),
        [&arg](const std::string &name) { return name == arg; });

    if (objectTake == names.end())
        return player.getClient().sendMessage("ko\n");

    std::lock_guard<std::mutex> lock(this->_map._resourceMutex);
    zappy::game::Resource resource = getResource(arg);

    auto &tile = this->_map.getTile(player.x, player.y);
    if (tile.getResourceQuantity(resource) == 0)
        return player.getClient().sendMessage("ko\n");

    player.collectRessource(resource);
    tile.removeResource(resource);
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    this->messageToGUI("pgt #" + std::to_string(player.getId()) + " " +
                       std::to_string(castResource(resource)) + "\n");
    this->resourceSendGui(player);
}

void zappy::game::CommandHandler::handleDrop(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    if (!this->_waitCommand(player, timeLimit::SET))
        return;
    auto objectDrop = std::find_if(names.begin(), names.end(),
        [&arg](const std::string &name) { return name == arg; });

    if (objectDrop == names.end())
        return player.getClient().sendMessage("ko\n");

    std::lock_guard<std::mutex> lock(this->_map._resourceMutex);
    zappy::game::Resource resource = getResource(arg);

    auto &inventory = player.getInventory();
    if (inventory.getResourceQuantity(resource) == 0)
        return player.getClient().sendMessage("ko\n");

    this->_map.getTile(player.x, player.y).addSingleResource(resource);
    player.dropRessource(resource);
    player.setInAction(false);
    player.getClient().sendMessage("ok\n");
    this->messageToGUI("pdr #" + std::to_string(player.getId()) + " " +
                       std::to_string(castResource(resource)) + "\n");
    this->resourceSendGui(player);
}
