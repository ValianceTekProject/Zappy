/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IRenderer.hpp
*/

#pragma once

#include "GameState.hpp"
#include "GuiProtocol.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

namespace zappy {
    namespace gui {
        using ProtocolRequest = std::unordered_map<network::GuiProtocol, std::function<void(const int &f, const int &d)>>;

        class IRenderer
        {
            public:
                virtual ~IRenderer() = default;

                virtual void init() = 0;

                virtual void setGameState(std::shared_ptr<game::GameState> &gameState) = 0;

                virtual void setProtocolRequests(const ProtocolRequest &protocolRequests) = 0;

                virtual void setFrequency(const size_t &frequency) = 0;

                virtual void handleInput() = 0;
                virtual void update() = 0;

                virtual void render() const = 0;

                virtual bool shouldClose() const = 0;

                virtual void addEgg(
                    const int &eggId,
                    const int &fatherId,
                    const int &x,
                    const int &y
                ) = 0;
                virtual void addPlayer(const game::Player &player) = 0;

                virtual void updatePlayerPosition(
                    const int &id,
                    const int &x,
                    const int &y,
                    const game::Orientation &orientation
                ) = 0;
                virtual void updatePlayerLevel(const int &id, const size_t &level) = 0;
                virtual void updatePlayerInventory(const int &id, const game::Inventory &inventory) = 0;

                virtual void playerExpulsion(const int &id) = 0;

                virtual void playerBroadcast(const int &id, const std::string &message) = 0;

                virtual void startIncantation(
                    const int &x, const int &y,
                    const int &level,
                    const std::vector<int> &playerIds
                ) = 0;
                virtual void endIncantation(const int &x, const int &y, const bool &result) = 0;

                virtual void hatchEgg(const int &eggId) = 0;

                virtual void removeEgg(const int &eggId) = 0;
                virtual void removePlayer(const int &id) = 0;

                virtual void endGame(const std::string &teamName) = 0;
        };
    }
}
