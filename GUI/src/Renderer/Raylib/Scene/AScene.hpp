/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AScene.hpp
*/

#pragma once

#include "IScene.hpp"
#include "MapRenderer.hpp"
#include "Skybox.hpp"
#include "MusicGame.hpp"

#include <memory>
#include <unordered_map>
#include <list>

namespace zappy {
    namespace gui {
        namespace raylib {
            class AScene : public IScene
            {
                public:
                    AScene(const std::shared_ptr<game::GameState> &gameState);
                    ~AScene() override { _music.unload(); }

                    virtual void init() override;

                    Camera &getCamera() override { return _camera; }
                    const Camera &getCamera() const override { return _camera; }

                    virtual void handleInput(InputManager &inputManager) override;
                    virtual void update() override;

                    virtual void render() const override;

                    virtual void addEgg(const int &id) override;
                    virtual void addPlayer(const int &id) override;

                    virtual void updatePlayerPosition(const int &id, const int &x, const int &y, const game::Orientation &orientation) override;

                    virtual void playerExpulsion(const int &id) override;

                    virtual void playerBroadcast(const int &id, const std::string &message) override;

                    virtual void startIncantation(
                        const int &x, const int &y,
                        const int &level,
                        const std::vector<int> &playerIds
                    ) override;
                    virtual void endIncantation(const int &x, const int &y, const bool &result) override;

                    virtual void hatchEgg(const int &id) override;

                    virtual void removeEgg(const int &id) override;
                    virtual void removePlayer(const int &id) override;

                    virtual void endGame(const std::string &teamName) override;

                protected:
                    Camera _camera;

                    MusicGame _music;
                    bool _isMusicPlaying;

                    const std::shared_ptr<game::GameState> _gameState;

                    Skybox _skybox;
                    const std::unique_ptr<MapRenderer> _mapRenderer;

                    bool _hasGameEnded;
                    std::string _wonTeamName;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
