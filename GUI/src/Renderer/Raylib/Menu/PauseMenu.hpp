/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.hpp
*/

#pragma once
#include "Menu/InputManager.hpp"
#include <raylib.h>
#include "Menu/MenuMacros.hpp"
#include "AssetPaths.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class PauseMenu
            {
                public:
                    PauseMenu();
                    ~PauseMenu();

                    void init();

                    void handleInput(const InputManager &inputManager);
                    void update();

                    bool shouldChangeScene() const { return _shouldChangeScene; }
                    SceneType getSceneType() const { return _sceneType; }

                    void render() const;

                    bool isActive() const { return _display; }

                private:
                    int _fontSize;
                    Color _textColor;
                    Color _boxColor;
                    Color _selectedColor;
                    Color _normalColor;

                    bool _display;

                    bool _shouldChangeScene;
                    SceneType _sceneType;

                    int _key;
                    int _selectedButton;

                    Texture2D _classicTexture;
                    Texture2D _pokemonTexture;
                    int _selectedTheme;
            };
        }
    }
}
