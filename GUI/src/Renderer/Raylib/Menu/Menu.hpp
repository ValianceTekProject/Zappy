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
            constexpr float DEFAULT_SCREEN_WIDTH = 1920.f;
            constexpr float DEFAULT_SCREEN_HEIGHT = 1080.f;

            constexpr int DEFAULT_FONT_SIZE = 20;
            constexpr Color DEFAULT_TEXT_COLOR = WHITE;

            class Menu {
                public:
                    enum class MenuState {
                        FREQ,
                        HELP,
                        BROADCASTS
                    };

                    Menu();
                    ~Menu() = default;

                    void init(const int &frequency);

                    void setFrequency(const size_t &frequency) { this->_frequency = frequency; }
                    size_t getFrequency() const { return this->_frequency; }
                    bool hasFrequencyChanged() const { return this->_freqChanged; }

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

                    void _renderShowHideHelp(const int &screenWidth, const int &screenHeight) const;
                    void _renderBroadcasts(const int &screenWidth, const int &screenHeight) const;
                    void _renderHelp(const int &screenWidth, const int &screenHeight) const;
                    void _renderFreq(const int &screenWidth, const int &screenHeight) const;

                    size_t _frequency;
                    bool _freqChanged;

                    bool _displayAll;
                    std::map<MenuState, int> _menuStatesKeys;
                    std::map<MenuState, bool> _menuStates;
                    std::map<MenuState, void (Menu::*)(const int&, const int&) const> _menuRenderFunctions;

                    int _fontSize;
                    Color _textColor;

                    std::list<std::string> _broadcasts;

                    Vector2 _broadcastBoxSize;
                    Color _broadcastBoxColor;

                    const std::map<int, int> _frequencyKeyModifiers = {
                        { KEY_UP, 1 },
                        { KEY_DOWN, -1 },
                        { KEY_LEFT, -10 },
                        { KEY_RIGHT, 10 }
                    };
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy

