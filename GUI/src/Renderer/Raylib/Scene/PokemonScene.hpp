/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonScene.hpp
*/

#include "AScene.hpp"
#include "PokemonPlayerModel.hpp"
#include "PokemonEggModel.hpp"
#include "PokemonResourceModel.hpp"

#include <raylib.h>
#include <unordered_map>

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
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
