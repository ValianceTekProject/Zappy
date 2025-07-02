/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RaylibRenderer.hpp
*/

#pragma once

#include "ARenderer.hpp"

#include "BasicScene.hpp"

namespace zappy {
    namespace gui {
        class RaylibRenderer : public ARenderer {
            public:
                RaylibRenderer();
                ~RaylibRenderer() override = default;

                void init() override;

                void setFrequency(const size_t &frequency) override;
                size_t getFrequency() const override { return this->_scene->getFrequency(); }
                bool hasFrequencyChanged() const override { return this->_scene->hasFrequencyChanged(); }

                void handleInput() override;
                void update() override;

                void render() const override;

                bool shouldClose() const override;

                void addEgg(
                    const int &eggId,
                    const int &fatherId,
                    const int &x,
                    const int &y
                ) override;
                void addPlayer(const game::Player &player) override;

                void updatePlayerPosition(const int &id,
                    const int &x,
                    const int &y,
                    const game::Orientation &orientation
                ) override;
                void updatePlayerLevel(const int &id, const size_t &level) override;
                void updatePlayerInventory(const int &id, const game::Inventory &inventory) override;

                void playerExpulsion(const int &id) override;

                void playerBroadcast(const int &id, const std::string &message) override;

                void startIncantation(
                    const int &x, const int &y,
                    const int &level,
                    const std::vector<int> &playerIds
                ) override;
                void endIncantation(const int &x, const int &y, const bool &result) override;

                void hatchEgg(const int &eggId) override;

                void removeEgg(const int &eggId) override;
                void removePlayer(const int &id) override;

                void endGame(const std::string &teamName) override;

            private:
                std::unique_ptr<raylib::IScene> _scene;

                raylib::InputManager _inputManager;
        };
    }
}
