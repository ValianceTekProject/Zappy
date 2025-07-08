/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** BasicScene (minimal implementation)
*/

#pragma once

#include "AScene.hpp"
#include "AssetPaths.hpp"
#include "BasicPlayerModel.hpp"
#include "BasicEggModel.hpp"
#include "BasicResourceModel.hpp"

#include <raylib.h>
#include <unordered_map>
#include <rlgl.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            class BasicScene : public AScene {
                public:
                    BasicScene(const std::shared_ptr<game::GameState> &gameState);
                    ~BasicScene() override = default;

                    void init() override;

                    bool shouldClose() const override;

                    void addEgg(const int &id) override;
                    void addPlayer(const int &id) override;

                    void endGame(const std::string &teamName) override;

                private:
                    Color _getColor(const game::Player &player);

                    const std::vector<Color> _colors = {
                        WHITE,
                        GREEN,
                        BLUE,
                        RED,
                        YELLOW,
                        PURPLE,
                        ORANGE,
                        PINK,
                        BROWN,
                    };
                    std::vector<std::string> _teamNames;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
