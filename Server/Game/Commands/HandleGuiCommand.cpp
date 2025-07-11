/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** handleGuiCommand
*/

#include "GuiCommand.hpp"

void zappy::game::CommandHandlerGui::handleMsz(
    zappy::game::ServerPlayer &player)
{
    std::string msg = "msz " + std::to_string(this->_widthMap) + " " +
                      std::to_string(this->_heightMap) + "\n";
    player.getClient().sendMessage(msg);
}

void zappy::game::CommandHandlerGui::handlePnw(zappy::game::ServerPlayer &gui)
{
    for (auto &team : this->_teamList) {
        auto teamsPlayer =
            std::dynamic_pointer_cast<zappy::game::TeamsPlayer>(team);

        if (teamsPlayer && !teamsPlayer->getPlayerList().empty()) {
            for (auto &player : teamsPlayer->getPlayerList()) {

                gui.getClient().sendMessage(
                    "pnw #" + std::to_string(player->getId()) + " " +
                    std::to_string(player->x) + " " +
                    std::to_string(player->y) + " " +
                    std::to_string(static_cast<int>(player->orientation + 1)) +
                    " " + std::to_string(player->level) + " " +
                    player->teamName + "\n");
            }
        }
    }
}

void zappy::game::CommandHandlerGui::handleBct(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    std::istringstream iss(arg);
    int x;
    int y;
    std::string leftover;
    std::string msg = "bct ";

    if (iss >> x >> y && !(iss >> leftover) &&
        (x <= this->_widthMap && x >= 0) &&
        (y <= this->_heightMap && y >= 0)) {
        msg += std::to_string(x) + " " + std::to_string(y);
        for (auto resource : this->_map.getTile(x, y).getResources())
            msg += " " + std::to_string(resource);
        msg += "\n";
        player.getClient().sendMessage(msg);
    } else
        player.getClient().sendMessage("ko\n");
}

void zappy::game::CommandHandlerGui::handleMct(
    zappy::game::ServerPlayer &player)
{
    for (int x = 0; x < this->_widthMap; x += 1) {
        for (int y = 0; y < this->_heightMap; y += 1)
            this->handleBct(player,
                std::string(std::to_string(x) + " " + std::to_string(y)));
    }
}

void zappy::game::CommandHandlerGui::handleTna(
    zappy::game::ServerPlayer &player)
{
    for (auto &team : this->_teamList) {
        if (team->getName() != "GRAPHIC")
            player.getClient().sendMessage(
                std::string("tna " + team->getName() + "\n"));
    }
}

void zappy::game::CommandHandlerGui::handlePpo(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    std::stringstream stream;
    int playerId;
    std::string msg = "ppo #";

    stream << arg;
    stream >> playerId;

    for (auto &team : this->_teamList) {
        for (auto &p : team->getPlayerList()) {
            if (p->getId() == playerId) {
                std::string msg =
                    msg + std::to_string(playerId) + " " +
                    std::to_string(p->x) + " " + std::to_string(p->y) + " " +
                    std::to_string(static_cast<int>(player.orientation) + 1) +
                    "\n";

                player.getClient().sendMessage(msg);
                return;
            }
        }
    }
    player.getClient().sendMessage("ko\n");
}

void zappy::game::CommandHandlerGui::handlePlv(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    std::stringstream stream;
    int playerId;
    std::string msg = "plv #";

    stream << arg;
    stream >> playerId;

    for (auto &team : this->_teamList) {
        for (auto &p : team->getPlayerList()) {
            if (p->getId() == playerId) {
                msg += std::to_string(playerId) + " " +
                       std::to_string(p->level) + "\n";
                player.getClient().sendMessage(msg);
                std::cout << "LEVEL : " << p->level << std::endl;
                return;
            }
        }
    }
    player.getClient().sendMessage("ko\n");
}

void zappy::game::CommandHandlerGui::handlePin(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    std::stringstream stream;
    int playerId;
    std::string msg = "pin #";

    stream << arg;
    stream >> playerId;

    for (auto &team : this->_teamList) {
        for (auto &p : team->getPlayerList()) {
            if (p->getId() == playerId) {
                msg += std::to_string(playerId) + " " + std::to_string(p->x) +
                       " " + std::to_string(p->y) + " ";
                zappy::game::Inventory playerInv = p->getInventory();
                for (auto foodName : names)
                    msg += std::to_string(playerInv.getResourceQuantity(
                               getResource(foodName))) +
                           " ";
                msg.pop_back();
                msg += "\n";
                player.getClient().sendMessage(msg);
                return;
            }
        }
    }
    player.getClient().sendMessage("ko\n");
}

void zappy::game::CommandHandlerGui::handleSgt(
    zappy::game::ServerPlayer &player)
{
    player.getClient().sendMessage(
        std::string("sgt ") + std::to_string(this->_freq) + "\n");
}

void zappy::game::CommandHandlerGui::handleSst(
    zappy::game::ServerPlayer &player, const std::string &arg)
{
    std::stringstream stream;
    int freq;
    std::string msg = "sgt ";

    stream << arg;
    stream >> freq;

    this->_freq = freq;

    player.getClient().sendMessage(msg + std::to_string(this->_freq) + "\n");
}

