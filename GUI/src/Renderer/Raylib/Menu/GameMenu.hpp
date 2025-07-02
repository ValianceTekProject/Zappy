/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameMenu.hpp
*/

#pragma once

#include "InputManager.hpp"
#include "GameState.hpp"
#include "RendererError.hpp"
#include "MenuMacros.hpp"

#include <memory>
#include <map>
#include <list>

namespace zappy {
    namespace gui{
        namespace raylib {
            class GameMenu {
                public:
                    GameMenu(const std::shared_ptr<game::GameState> &gameState);
                    ~GameMenu() = default;

                    void init();

                    void setFrequency(const size_t &frequency) { this->_frequency = frequency; }
                    size_t getFrequency() const { return this->_frequency; }
                    bool hasFrequencyChanged();

                    void handleInput(const InputManager &inputManager);
                    void update();

                    void render() const;

                    void addPlayer(const int &id);

                    void playerBroadcast(
                        const int &id,
                        const std::string &message,
                        const std::string &playerTeam
                    );

                    void removePlayer(const int &id);

                private:
                    MenuPlayerInfo &_getPlayerInfo(const int &id);
                    const MenuPlayerInfo &_getPlayerInfo(const int &id) const;

                    std::string _decryptBroadcast(
                        const int &id,
                        const std::string &message,
                        const std::string &playerTeam
                    );

                    void _handleFreqInput(const InputManager &inputManager);
                    void _handlePlayersInput(const InputManager &inputManager);

                    void _renderShowHideHelp(const int &screenWidth, const int &screenHeight) const;
                    void _renderBroadcasts(const int &screenWidth, const int &screenHeight) const;
                    void _renderHelp(const int &screenWidth, const int &screenHeight) const;
                    void _renderFreq(const int &screenWidth, const int &screenHeight) const;
                    void _renderPlayersInfos(const int &screenWidth, const int &screenHeight) const;
                    void _renderPlayerInfo(
                        const MenuPlayerInfo &playerInfo,
                        const int &x, const int &y,
                        const int &textSize,
                        const int &spacingY,
                        const int &boxWidth,
                        const int &boxHeight
                    ) const;

                    const std::shared_ptr<game::GameState> _gameState;

                    int _fontSize;
                    Color _textColor;
                    Color _boxColor;

                    size_t _frequency;
                    bool _freqChanged;

                    std::vector<MenuPlayerInfo> _playersInfos;
                    int _displayedPlayersIndex;
                    int _numberPlayerDisplayed;

                    int _playerBoxWidth;

                    std::list<std::string> _broadcasts;

                    MenuModifiedSection _modifiedSection;
                    std::map<MenuModifiedSection, void (GameMenu::*)(const InputManager&)> _modifiedSectionHandlers;

                    bool _displayAll;
                    std::map<MenuState, int> _menuStatesKeys;
                    std::map<MenuState, bool> _menuStates;
                    std::map<MenuState, void (GameMenu::*)(const int&, const int&) const> _menuRenderFunctions;

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
