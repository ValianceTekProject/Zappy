/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** ClientCommand
*/

#include "ClientCommand.hpp"
#include "Game.hpp"

void zappy::game::CommandHandler::messageToGUI(const std::string &msg)
{
    for (auto &team : _teamList) {
        if (team->getName() == "GRAPHIC") {
            for (auto &player : team->getPlayerList()) {
                if (player)
                    player->getClient().sendMessage(msg);
            }
        }
    }
}

void zappy::game::CommandHandler::initCommandMap()
{
    this->_commandMap = {
        {"Forward",
            [this](ServerPlayer &player, const std::string &) {
                handleForward(player);
            }},
        {"Right",
            [this](ServerPlayer &player, const std::string &) {
                handleRight(player);
            }},
        {"Left",
            [this](ServerPlayer &player, const std::string &) {
                handleLeft(player);
            }},
        {"Look",
            [this](ServerPlayer &player, const std::string &) {
                handleLook(player);
            }},
        {"Inventory",
            [this](ServerPlayer &player, const std::string &) {
                handleInventory(player);
            }},
        {"Broadcast",
            [this](ServerPlayer &player, const std::string &arg) {
                handleBroadcast(player, arg);
            }},
        {"Connect_nbr",
            [this](ServerPlayer &player, const std::string &) {
                handleConnectNbr(player);
            }},
        {"Fork",
            [this](ServerPlayer &player, const std::string &) {
                handleFork(player);
            }},
        {"Eject",
            [this](ServerPlayer &player, const std::string &) {
                handleEject(player);
            }},
        {"Take",
            [this](ServerPlayer &player, const std::string &arg) {
                handleTake(player, arg);
            }},
        {"Set",
            [this](ServerPlayer &player, const std::string &arg) {
                handleDrop(player, arg);
            }},
        {"Incantation", [this](ServerPlayer &player, const std::string &) {
             handleIncantation(player);
         }}};
}

bool zappy::game::CommandHandler::_waitCommand(ServerPlayer &player, timeLimit limit)
{
    auto commandTime = static_cast<double>(limit) / this->_freq;
    auto timeNeeded = std::chrono::duration<double>(commandTime);
    
    auto startTime = std::chrono::steady_clock::now();
    auto endTime = startTime + timeNeeded;
    
    while (std::chrono::steady_clock::now() < endTime) {
        auto tickDuration = std::chrono::duration<double>(1.0 / this->_freq);
        std::this_thread::sleep_for(tickDuration);

        if (player.interrupted) {
            player.stopPraying();
            player.setInAction(false);
            return false;
        }
    }
    return true;
}

void zappy::game::CommandHandler::_executeCommand(
    zappy::game::ServerPlayer &player,
    std::function<void(ServerPlayer &, const std::string &)> function,
    const std::string &args)
{
    std::thread commandThread([&player, function, args]() {
        if (player.isInAction()) {
            return;
        }
        std::lock_guard<std::mutex> lock(*(player.getClient().queueMutex));
        if (player.getClient().queueMessage.empty())
            return;
        player.getClient().queueMessage.pop();

        player.setInAction(true);
        player.startChrono();

        function(player, args);
        player.setInAction(false);
    });
    commandThread.detach();
}

void zappy::game::CommandHandler::processClientInput(
    std::string &input, zappy::game::ServerPlayer &player)
{
    if (this->_commandMap.empty())
        this->initCommandMap();

    auto spacePos = input.find(' ');
    std::string cmd = input.substr(0, spacePos);
    std::string args =
        (spacePos != std::string::npos) ? input.substr(spacePos + 1) : "";

    if (!args.empty() && args.back() == '\n')
        args.pop_back();

    auto it = this->_commandMap.find(cmd);
    if (it != this->_commandMap.end())
        return this->_executeCommand(player, it->second, args);
    if (player.isInAction() == false) {
        std::lock_guard<std::mutex> lock(player.msgMutex);
        player.getClient().queueMessage.pop();
        player.getClient().sendMessage("ko\n");
    }
}
