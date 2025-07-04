/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** MinecraftScene.hpp
*/

#pragma once

#include "AScene.hpp"

#include "AxolotlPlayerModel.hpp"
#include "BeePlayerModel.hpp"

#include "MinecraftEggModel.hpp"
#include "MinecraftResourceModel.hpp"

#include <raylib.h>
#include <map>
#include <functional>

namespace zappy {
    namespace gui {
        namespace raylib {
            class MinecraftScene : public AScene {
                public:
                    MinecraftScene(const std::shared_ptr<game::GameState> &gameState);
                    ~MinecraftScene() override = default;

                    void init() override;

                    bool shouldClose() const override;

                    void addEgg(const int &id) override;
                    void addPlayer(const int &id) override;

                    void endGame(const std::string &teamName) override;

                private:
                    const std::vector<std::function<std::unique_ptr<APlayerModel>(const int&)>> _playerModelsConstructors = {
                        [](const int &id) { return std::make_unique<AxolotlPlayerModel>(id); },
                        [](const int &id) { return std::make_unique<BeePlayerModel>(id); },
                    };

                    size_t _currentModelIndex = 0;

                    std::map<std::string, size_t> _playersModels;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
