/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ARenderer.hpp
*/

#pragma once

#include "IRenderer.hpp"
#include "GuiError.hpp"

#include <memory>
#include <chrono>

namespace zappy {
    namespace gui {
        constexpr float RequestMapContentTimeUnit = 20.0f;
        constexpr float RequestPlayersInventoryTimeUnit = 126.0f;

        class ARenderer : public IRenderer {
            public:
                ARenderer();
                virtual ~ARenderer() override = default;

                virtual void init() override;

                void setGameState(std::shared_ptr<game::GameState> &gameState) override;

                void setProtocolRequests(const ProtocolRequest &protocolRequests) override;

                virtual void setFrequency(const size_t &frequency) override;

                virtual void handleInput() override = 0;
                virtual void update() override;

                virtual void render() const override = 0;

                virtual bool shouldClose() const override = 0;

                virtual void addEgg(
                    const int &eggId,
                    const int &fatherId,
                    const int &x,
                    const int &y
                ) override;
                virtual void addPlayer(const game::Player &player) override;

                virtual void updatePlayerPosition(
                    const int &id,
                    const int &x,
                    const int &y,
                    const game::Orientation &orientation
                ) override;
                virtual void updatePlayerLevel(const int &id, const size_t &level) override;
                virtual void updatePlayerInventory(const int &id, const game::Inventory &inventory) override;

                virtual void playerExpulsion(const int &id) override;

                virtual void playerBroadcast(const int &id, const std::string &message) override;

                virtual void startIncantation(
                    const int &x, const int &y,
                    const int &level,
                    const std::vector<int> &playerIds
                ) override;
                virtual void endIncantation(const int &x, const int &y, const bool &result) override;

                virtual void hatchEgg(const int &eggId) override;

                virtual void removeEgg(const int &eggId) override;
                virtual void removePlayer(const int &id) override;

                virtual void endGame(const std::string &teamName) override;

            protected:
                void _checkGameState() const;

                std::shared_ptr<game::GameState> _gameState;

                ProtocolRequest _protocolRequests;

                std::chrono::steady_clock::time_point _lastTime;
                float _elapsedTime;
        };
    }
}
