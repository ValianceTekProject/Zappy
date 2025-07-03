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

namespace zappy {
    namespace gui {
        namespace raylib {
            class PauseMenu
            {
                public:
                    PauseMenu();
                    ~PauseMenu() = default;

                    void init();

                    void handleInput(const InputManager &inputManager);

                    void update();

                    void render() const;

                    bool isActive() const { return _display; }

                private:
                    int _fontSize;
                    Color _textColor;
                    Color _boxColor;
                    Color _selectedColor;
                    Color _normalColor;

                    bool _display;

                    int _key;
                    int _selectedButton;
            };
        }
    }
}