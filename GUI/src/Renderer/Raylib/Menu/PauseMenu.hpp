/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.hpp
*/

#pragma once
#include "InputManager.hpp"
#include "MenuMacros.hpp"
#include "AssetPaths.hpp"

#include <raylib.h>

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
                    struct Theme {
                        std::string name;
                        Texture2D texture;
                    };

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
                    int _selectedTheme;

                    PauseMenuState _menuState;
                    std::vector<Theme> _themes;

                    void _renderPauseMenu(float scaleX, float scaleY, int menuX, int menuY, int menuWidth, int menuHeight) const;
                    void _renderThemeMenu(float scaleX, float scaleY, int menuX, int menuY, int menuWidth, int menuHeight) const;
                    void _renderTheme(const Theme& theme, int x, int y, int width, int height, bool selected) const;
            };
        }
    }
}
