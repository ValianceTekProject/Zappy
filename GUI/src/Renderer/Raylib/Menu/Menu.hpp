/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Menu.hpp
*/

#pragma once

#include "InputManager.hpp"

#include <iostream>
#include <memory>
#include <map>
#include <list>

namespace zappy {
    namespace gui{
        namespace raylib {
            class Menu {
                public:
                    enum class MenuState {
                        HELP,
                        BROADCASTS
                    };

                    Menu();
                    ~Menu() = default;

                    void init();

                    void handleInput(InputManager &inputManager);
                    void update();

                    void render() const;

                    void playerBroadcast(
                        const int &id,
                        const std::string &message,
                        const std::string &playerTeam
                    );

                private:
                    std::string _decryptBroadcast(
                        const int &id,
                        const std::string &message,
                        const std::string &playerTeam
                    );

                    void _renderShowHideHelp() const;
                    void _renderHelp() const;
                    void _renderBroadcasts() const;

                    std::map<MenuState, int> _menuStatesKeys;
                    std::map<MenuState, bool> _menuStates;
                    std::map<MenuState, void (Menu::*)() const> _menuRenderFunctions;

                    int _fontSize;
                    Color _textColor;

                    std::list<std::string> _broadcasts;

                    Vector2 _broadcastBoxSize;
                    Color _broadcastBoxColor;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy

