/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Menu.cpp
*/

#include "Menu.hpp"

zappy::gui::raylib::Menu::Menu() :
    _fontSize(20),
    _textColor(WHITE)
{}

void zappy::gui::raylib::Menu::init()
{
    constexpr Vector2 broadcastBoxSize = {400, 100};
    constexpr Color broadcastBoxColor = {0, 200, 0, 100};

    this->_broadcastBoxSize = broadcastBoxSize;
    this->_broadcastBoxColor = broadcastBoxColor;

    this->_menuStatesKeys[MenuState::BROADCASTS] = KEY_B;
    this->_menuStates[MenuState::BROADCASTS] = false;
    this->_menuRenderFunctions[MenuState::BROADCASTS] = &Menu::_renderBroadcasts;

    this->_menuStatesKeys[MenuState::HELP] = KEY_H;
    this->_menuStates[MenuState::HELP] = false;
    this->_menuRenderFunctions[MenuState::HELP] = &Menu::_renderHelp;

    playerBroadcast(1, "Hello World!", "Team 1");
}

void zappy::gui::raylib::Menu::handleInput(InputManager &inputManager)
{
    if (inputManager.isKeyPressed(KEY_F11))
        ToggleFullscreen();

    for (const auto &[state, key] : _menuStatesKeys) {
        if (inputManager.isKeyPressed(key))
            this->_menuStates[state] = !this->_menuStates[state];
    }
}

void zappy::gui::raylib::Menu::update()
{}

void zappy::gui::raylib::Menu::render() const
{
    for (const auto &[state, isActive] : _menuStates) {
        if (!isActive)
            continue;
        auto it = _menuRenderFunctions.find(state);
        if (it != _menuRenderFunctions.end()) {
            (this->*it->second)();
        }
    }

    _renderShowHideHelp();
}

void zappy::gui::raylib::Menu::playerBroadcast(
    const int &id,
    const std::string &message,
    const std::string &playerTeam
) {
    this->_broadcasts.push_back(_decryptBroadcast(id, message, playerTeam));

    constexpr size_t maxBroadcastSize = 10;

    if (_broadcasts.size() > maxBroadcastSize)
        this->_broadcasts.pop_front();
}

std::string zappy::gui::raylib::Menu::_decryptBroadcast(
    const int &id,
    const std::string &message,
    const std::string &playerTeam
) {
    std::string decryptedMessage = message;

    (void)playerTeam;

    decryptedMessage = "Player " + std::to_string(id) + ": " + decryptedMessage;

    return decryptedMessage;
}

void zappy::gui::raylib::Menu::_renderShowHideHelp() const
{
    constexpr int padding = 10;
    constexpr int boxWidth = 300;
    constexpr int titleHeight = 30;

    int screenX = padding;
    int screenY = padding;

    // Dessine le bouton [H] Help
    DrawRectangle(screenX, screenY, boxWidth, titleHeight, _broadcastBoxColor);
    DrawRectangleLines(screenX, screenY, boxWidth, titleHeight, WHITE);
    DrawText("[H] Help", screenX + padding, screenY + 5, 20, _textColor);
}

void zappy::gui::raylib::Menu::_renderHelp() const
{
    constexpr int padding = 10;
    constexpr int lineHeight = 20;
    constexpr int titleHeight = 30;

    int screenX = padding;
    int screenY = padding;

    std::vector<std::string> lines = {
        "z/q/s/d to move",
        "mouse to look around",
        "[B] Broadcasts messages",
        "[F11] Toggle fullscreen"
        // Tu peux en ajouter plus ici si besoin
    };

    int y = screenY + titleHeight + padding;
    for (const auto &line : lines) {
        DrawTextEx(GetFontDefault(), line.c_str(),
            {static_cast<float>(screenX + padding), static_cast<float>(y)},
            18, 1, _textColor);
        y += lineHeight;
    }
}

void zappy::gui::raylib::Menu::_renderBroadcasts() const
{
    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();

    const int padding = 10;
    const int boxWidth = 400;
    const int lineHeight = 20;
    const int titleHeight = 30;

    int boxHeight = titleHeight + (lineHeight + padding) * static_cast<int>(_broadcasts.size());

    int boxX = screenWidth - boxWidth - padding;
    int boxY = screenHeight - boxHeight - padding;

    // Draw box background
    DrawRectangle(boxX, boxY, boxWidth, boxHeight, _broadcastBoxColor);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);

    // Draw title
    DrawText("Broadcasts", boxX + padding, boxY + 5, 20, WHITE);

    // Draw each broadcast
    int y = boxY + titleHeight;
    for (const auto &broadcast : _broadcasts) {
        DrawTextEx(GetFontDefault(), broadcast.c_str(), {static_cast<float>(boxX + padding), static_cast<float>(y)}, 18, 1, _textColor);
        y += lineHeight + padding;
    }
}
