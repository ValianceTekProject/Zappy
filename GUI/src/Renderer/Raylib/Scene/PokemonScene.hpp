/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonScene.hpp
*/

#pragma once

#include "AScene.hpp"

#include "BulbasaurPlayerModel.hpp"
#include "FlyingPikachuPlayerModel.hpp"

#include "PokemonEggModel.hpp"
#include "PokemonResourceModel.hpp"

#include <raylib.h>
#include <map>
#include <functional>

namespace zappy {
    namespace gui {
        namespace raylib {
            class PokemonScene : public AScene {
                public:
                    PokemonScene(const std::shared_ptr<game::GameState> &gameState);
                    ~PokemonScene() override = default;

                    void init() override;

                    bool shouldClose() const override;

                    void addEgg(const int &id) override;
                    void addPlayer(const int &id) override;

                    void endGame(const std::string &teamName) override;

                private:
                    // player models constructors
                    const std::vector<float> _constructorProbabilities = {
                        0.7f, // Bulbasaur
                        0.3f, // FlyingPikachu
                    };

                    const std::map<int, std::function<std::unique_ptr<APlayerModel>(const int&)>> _playerModelsConstructors = {
                        { 0, [](const int &id) { return std::make_unique<BulbasaurPlayerModel>(id); } },
                        { 1, [](const int &id) { return std::make_unique<FlyingPikachuPlayerModel>(id); } },
                    };
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
