/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** GuiCommand
*/

#include "GuiCommand.hpp"

void zappy::game::CommandHandlerGui::initCommandMap()
{
    this->_commandMapGui = {
        {"msz",
            [this](ServerPlayer &player, const std::string &) {
                handleMsz(player);
            }},
        {"bct",
            [this](ServerPlayer &player, const std::string &arg) {
                handleBct(player, arg);
            }},
        {"mct",
            [this](ServerPlayer &player, const std::string &) {
                handleMct(player);
            }},
        {"tna",
            [this](ServerPlayer &player, const std::string &) {
                handleTna(player);
            }},
        {"ppo",
            [this](ServerPlayer &player, const std::string &arg) {
                handlePpo(player, arg);
            }},
        {"plv",
            [this](ServerPlayer &player, const std::string &arg) {
                handlePlv(player, arg);
            }},
        {"pin",
            [this](ServerPlayer &player, const std::string &arg) {
                handlePin(player, arg);
            }},
        {"sgt",
            [this](ServerPlayer &player, const std::string &) {
                handleSgt(player);
            }},
        {"sst", [this](ServerPlayer &player, const std::string &arg) {
             handleSst(player, arg);
         }}};
}

void zappy::game::CommandHandlerGui::processClientInput(
    std::string &input, zappy::game::ServerPlayer &player)
{
    if (this->_commandMapGui.empty())
        this->initCommandMap();

    size_t pos = input.find('#');

    while (pos != std::string::npos) {
        input.erase(pos, 1);
        pos = input.find('#');
    }

    auto spacePos = input.find(' ');
    std::string cmd = input.substr(0, spacePos);
    std::string args =
        (spacePos != std::string::npos) ? input.substr(spacePos + 1) : "";

    if (!args.empty() && args.back() == '\n')
        args.pop_back();

    auto it = this->_commandMapGui.find(cmd);
    std::lock_guard<std::mutex> lock(*(player.getClient().queueMutex));
    if (it != this->_commandMapGui.end()) {
        it->second(player, args);
        if (!player.getClient().queueMessage.empty())
            player.getClient().queueMessage.pop();
        return;
    }
    player.getClient().sendMessage("suc\n");
    if (!player.getClient().queueMessage.empty())
        player.getClient().queueMessage.pop();
}
