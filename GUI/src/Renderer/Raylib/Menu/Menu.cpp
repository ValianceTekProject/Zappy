/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Menu.cpp
*/

#include "Menu.hpp"

zappy::gui::raylib::Menu::Menu() :
    _frequency(0),
    _freqChanged(false),
    _displayAll(true),
    _fontSize(DEFAULT_FONT_SIZE),
    _textColor(DEFAULT_TEXT_COLOR)
{}

void zappy::gui::raylib::Menu::init(const int &frequency)
{
    this->_frequency = frequency;

    constexpr Vector2 broadcastBoxSize = {400, 100};
    constexpr Color broadcastBoxColor = {0, 200, 0, 100};

    this->_broadcastBoxSize = broadcastBoxSize;
    this->_broadcastBoxColor = broadcastBoxColor;

    this->_menuStatesKeys[MenuState::FREQ] = KEY_F;
    this->_menuStates[MenuState::FREQ] = true;
    this->_menuRenderFunctions[MenuState::FREQ] = &Menu::_renderShowHideHelp;

    this->_menuStatesKeys[MenuState::HELP] = KEY_H;
    this->_menuStates[MenuState::HELP] = false;
    this->_menuRenderFunctions[MenuState::HELP] = &Menu::_renderHelp;

    this->_menuStatesKeys[MenuState::BROADCASTS] = KEY_B;
    this->_menuStates[MenuState::BROADCASTS] = false;
    this->_menuRenderFunctions[MenuState::BROADCASTS] = &Menu::_renderBroadcasts;

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

    if (inputManager.isKeyPressed(KEY_F1))
        _displayAll = !_displayAll;

    for (const auto &[key, modifier]: _frequencyKeyModifiers) {
        if (inputManager.isKeyPressed(key)) {
            if (modifier < 0 && this->_frequency == 0)
                continue;
            else if (modifier > 0 && this->_frequency == 1000)
                continue;
            else if (modifier + static_cast<long>(this->_frequency) < 0)
                this->_frequency = 0;
            else if (modifier + static_cast<long>(this->_frequency) > 1000)
                this->_frequency = 1000;
            else
                this->_frequency += modifier;
            this->_freqChanged = true;
        }
    }
}

void zappy::gui::raylib::Menu::update()
{}

void zappy::gui::raylib::Menu::render() const
{
    if (!_displayAll)
        return;

    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();

    for (const auto &[state, isActive] : _menuStates) {
        if (!isActive)
            continue;
        auto it = _menuRenderFunctions.find(state);
        if (it != _menuRenderFunctions.end()) {
            (this->*it->second)(screenWidth, screenHeight);
        }
    }

    _renderShowHideHelp(screenWidth, screenHeight);
    _renderFreq(screenWidth, screenHeight);
}

void zappy::gui::raylib::Menu::playerBroadcast(
    const int &id,
    const std::string &message,
    const std::string &playerTeam
) {
    this->_broadcasts.push_back(_decryptBroadcast(id, message, playerTeam));

    constexpr size_t maxBroadcastSize = 10;

    if (this->_broadcasts.size() > maxBroadcastSize)
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

void zappy::gui::raylib::Menu::_renderShowHideHelp(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = static_cast<int>(10 * scaleX);
    const int paddingY = static_cast<int>(10 * scaleY);
    const int boxWidth = static_cast<int>(300 * scaleX);
    const int titleHeight = static_cast<int>(30 * scaleY);
    const int fontSize = static_cast<int>(20 * scaleY);

    int screenX = paddingX;
    int screenY = paddingY;

    DrawRectangle(screenX, screenY, boxWidth, titleHeight, _broadcastBoxColor);
    DrawRectangleLines(screenX, screenY, boxWidth, titleHeight, WHITE);
    DrawText("[H] Help", screenX + paddingX, screenY + paddingY / 2, fontSize, this->_textColor);
}

void zappy::gui::raylib::Menu::_renderBroadcasts(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = static_cast<int>(10 * scaleX);
    const int paddingY = static_cast<int>(10 * scaleY);
    const int boxWidth = static_cast<int>(400 * scaleX);
    const int lineHeight = static_cast<int>(20 * scaleY);
    const int titleHeight = static_cast<int>(30 * scaleY);
    const int fontSize = static_cast<int>(this->_fontSize * scaleY);
    const int textSize = static_cast<int>(18 * scaleY);

    int boxHeight = titleHeight + (_broadcasts.size() * (lineHeight + paddingY));
    int boxX = screenWidth - boxWidth - paddingX;
    int boxY = screenHeight - boxHeight - paddingY;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, _broadcastBoxColor);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);
    DrawText("Broadcasts", boxX + paddingX, boxY + paddingY / 2, fontSize, WHITE);

    int y = boxY + titleHeight;
    for (const auto &broadcast : _broadcasts) {
        DrawTextEx(GetFontDefault(), broadcast.c_str(),
            {static_cast<float>(boxX + paddingX), static_cast<float>(y)},
            static_cast<float>(textSize), 1, _textColor);
        y += lineHeight + paddingY;
    }
}

void zappy::gui::raylib::Menu::_renderHelp(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = static_cast<int>(10 * scaleX);
    const int paddingY = static_cast<int>(10 * scaleY);
    const int lineHeight = static_cast<int>(20 * scaleY);
    const int titleHeight = static_cast<int>(30 * scaleY);
    const int fontSize = static_cast<int>(this->_fontSize * scaleY);

    int screenX = paddingX;
    int screenY = paddingY;

    const std::vector<std::string> lines = {
        "[F1] Toggle all menus",
        "[F11] Toggle fullscreen",
        "z/q/s/d to move",
        "mouse to look around",
        "Arrow keys to modify frequency",
        "[B] Broadcasts messages",
    };

    int y = screenY + titleHeight + paddingY;
    for (const auto &line : lines) {
        DrawTextEx(GetFontDefault(), line.c_str(),
            {static_cast<float>(screenX + paddingX), static_cast<float>(y)},
            static_cast<float>(fontSize), 1, this->_textColor);
        y += lineHeight + paddingY;
    }
}

void zappy::gui::raylib::Menu::_renderFreq(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingY = static_cast<int>(10 * scaleY);
    const int boxWidth = static_cast<int>(300 * scaleX);
    const int boxHeight = static_cast<int>(40 * scaleY);
    const int fontSize = static_cast<int>(this->_fontSize * scaleY);

    const int boxX = (screenWidth - boxWidth) / 2;
    const int boxY = paddingY;

    const std::string freqText = "Frequency: " + std::to_string(this->_frequency);

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, _broadcastBoxColor);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);

    const int textWidth = MeasureText(freqText.c_str(), fontSize);
    const int textX = boxX + (boxWidth - textWidth) / 2;
    const int textY = boxY + (boxHeight - fontSize) / 2;

    DrawText(freqText.c_str(), textX, textY, fontSize, this->_textColor);
}
