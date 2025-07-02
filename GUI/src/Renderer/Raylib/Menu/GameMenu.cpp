/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameMenu.cpp
*/

#include "GameMenu.hpp"

zappy::gui::raylib::GameMenu::GameMenu(const std::shared_ptr<game::GameState> &gameState) :
    _gameState(gameState),
    _fontSize(DEFAULT_FONT_SIZE),
    _textColor(DEFAULT_TEXT_COLOR),
    _frequency(0),
    _freqChanged(false),
    _displayedPlayersIndex(-1),
    _numberPlayerDisplayed(0),
    _displayAll(true)
{}

void zappy::gui::raylib::GameMenu::init()
{
    this->_frequency = this->_gameState->getFrequency();

    constexpr Color broadcastBoxColor = {0, 175, 0, 100};

    this->_boxColor = broadcastBoxColor;

    this->_modifiedSection = MenuModifiedSection::FREQ;

    this->_modifiedSectionHandlers[MenuModifiedSection::FREQ] = &GameMenu::_handleFreqInput;
    this->_modifiedSectionHandlers[MenuModifiedSection::PLAYERS] = &GameMenu::_handlePlayersInput;

    this->_menuStatesKeys[MenuState::HELP] = KEY_H;
    this->_menuStates[MenuState::HELP] = false;
    this->_menuRenderFunctions[MenuState::HELP] = &GameMenu::_renderHelp;

    this->_menuStatesKeys[MenuState::BROADCASTS] = KEY_B;
    this->_menuStates[MenuState::BROADCASTS] = false;
    this->_menuRenderFunctions[MenuState::BROADCASTS] = &GameMenu::_renderBroadcasts;

    this->_menuStatesKeys[MenuState::PLAYERS] = KEY_P;
    this->_menuStates[MenuState::PLAYERS] = false;
    this->_menuRenderFunctions[MenuState::PLAYERS] = &GameMenu::_renderPlayersInfos;
}

bool zappy::gui::raylib::GameMenu::hasFrequencyChanged()
{
    if (this->_freqChanged) {
        this->_freqChanged = false;
        return true;
    }
    return false;
}

void zappy::gui::raylib::GameMenu::handleInput(const InputManager &inputManager)
{
    if (inputManager.isKeyPressed(KEY_F11))
        ToggleFullscreen();

    for (const auto &[state, key] : _menuStatesKeys) {
        if (inputManager.isKeyPressed(key))
            this->_menuStates[state] = !this->_menuStates[state];
    }

    if (inputManager.isKeyPressed(KEY_F1))
        _displayAll = !_displayAll;

    if (inputManager.isKeyPressed(KEY_F))
        this->_modifiedSection++;

    (this->*_modifiedSectionHandlers[this->_modifiedSection])(inputManager);
}

void zappy::gui::raylib::GameMenu::update()
{}

void zappy::gui::raylib::GameMenu::render() const
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

void zappy::gui::raylib::GameMenu::addPlayer(const int &id)
{
    if (this->_playersInfos.empty()) {
        this->_displayedPlayersIndex = 0;
        this->_numberPlayerDisplayed = 1;
    }

    Texture2D icon = LoadTexture("Assets/grass.jpg");

    MenuPlayerInfo playerInfo(id, true, icon);

    this->_playersInfos.push_back(playerInfo);
}

void zappy::gui::raylib::GameMenu::playerBroadcast(
    const int &id,
    const std::string &message,
    const std::string &playerTeam
) {
    this->_broadcasts.push_back(_decryptBroadcast(id, message, playerTeam));

    constexpr size_t maxBroadcastSize = 10;

    if (this->_broadcasts.size() > maxBroadcastSize)
        this->_broadcasts.pop_front();
}

void zappy::gui::raylib::GameMenu::removePlayer(const int &id)
{
    for (auto it = this->_playersInfos.begin(); it != this->_playersInfos.end(); ++it) {
        if (it->id == id) {
            this->_playersInfos.erase(it);
            break;
        }
    }

    if (this->_displayedPlayersIndex >= static_cast<int>(_playersInfos.size()))
        this->_displayedPlayersIndex = this->_displayedPlayersIndex % static_cast<int>(_playersInfos.size());

    if (this->_playersInfos.empty()) {
        this->_displayedPlayersIndex = -1;
        this->_numberPlayerDisplayed = 0;
    }
}

zappy::gui::raylib::MenuPlayerInfo &zappy::gui::raylib::GameMenu::_getPlayerInfo(const int &id)
{
    for (auto &playerInfo : this->_playersInfos) {
        if (playerInfo.id == id)
            return playerInfo;
    }
    throw RendererError("Player" + std::to_string(id) + " Infos not found", "GameMenu::_getPlayerInfo");
}

const zappy::gui::raylib::MenuPlayerInfo &zappy::gui::raylib::GameMenu::_getPlayerInfo(const int &id) const
{
    for (const auto &playerInfo : this->_playersInfos) {
        std::cout << "MenuPlayerInfo.id: " << playerInfo.id << std::endl;
        if (playerInfo.id == id)
            return playerInfo;
    }
    throw RendererError("Player" + std::to_string(id) + " Infos not found", "GameMenu::_getPlayerInfo");
}

std::string zappy::gui::raylib::GameMenu::_decryptBroadcast(
    const int &id,
    const std::string &message,
    const std::string &playerTeam
) {
    std::string decryptedMessage = message;

    (void)playerTeam;

    decryptedMessage = "Player " + std::to_string(id) + ": " + decryptedMessage;

    return decryptedMessage;
}

void zappy::gui::raylib::GameMenu::_handleFreqInput(const InputManager &inputManager)
{
    for (const auto &[key, modifier]: _frequencyKeyModifiers) {
        if (inputManager.isKeyPressed(key)) {
            if (modifier < 0 && this->_frequency == MIN_FREQ)
                continue;
            else if (modifier > 0 && this->_frequency == MAX_FREQ)
                continue;
            else if (modifier + static_cast<long>(this->_frequency) < MIN_FREQ)
                this->_frequency = MIN_FREQ;
            else if (modifier + static_cast<long>(this->_frequency) > MAX_FREQ)
                this->_frequency = MAX_FREQ;
            else
                this->_frequency += modifier;
            this->_freqChanged = true;
        }
    }
}

void zappy::gui::raylib::GameMenu::_handlePlayersInput(const InputManager &inputManager)
{
    if (this->_playersInfos.size() == 0)
        return;

    if (inputManager.isKeyPressed(KEY_UP)) {
        this->_numberPlayerDisplayed++;
        if (this->_numberPlayerDisplayed > MAX_PLAYERS_DISPLAYED)
            this->_numberPlayerDisplayed = MAX_PLAYERS_DISPLAYED;
    } else if (inputManager.isKeyPressed(KEY_DOWN)) {
        this->_numberPlayerDisplayed--;
        if (this->_numberPlayerDisplayed < MIN_PLAYERS_DISPLAYED)
            this->_numberPlayerDisplayed = MIN_PLAYERS_DISPLAYED;
    } else if (inputManager.isKeyPressed(KEY_LEFT)) {
        this->_displayedPlayersIndex -= this->_numberPlayerDisplayed;
        if (this->_displayedPlayersIndex < 0)
            this->_displayedPlayersIndex = this->_displayedPlayersIndex % this->_playersInfos.size();
    } else if (inputManager.isKeyPressed(KEY_RIGHT)) {
        this->_displayedPlayersIndex += this->_numberPlayerDisplayed;
        if (this->_displayedPlayersIndex >= static_cast<int>(this->_playersInfos.size()))
            this->_displayedPlayersIndex %= this->_playersInfos.size();
    }
}

void zappy::gui::raylib::GameMenu::_renderShowHideHelp(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = static_cast<int>(10 * scaleX);
    const int paddingY = static_cast<int>(10 * scaleY);
    const int fontSize = static_cast<int>(20 * scaleY);

    const std::string text = "[H] Help";
    const int textWidth = MeasureText(text.c_str(), fontSize);
    const int boxWidth = textWidth + 2 * paddingX;
    const int boxHeight = fontSize + 2 * paddingY;

    const int screenX = paddingX;
    const int screenY = paddingY;

    DrawRectangle(screenX, screenY, boxWidth, boxHeight, _boxColor);
    DrawRectangleLines(screenX, screenY, boxWidth, boxHeight, WHITE);
    DrawText(text.c_str(), screenX + paddingX, screenY + paddingY, fontSize, this->_textColor);
}

void zappy::gui::raylib::GameMenu::_renderBroadcasts(const int &screenWidth, const int &screenHeight) const
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
    int boxX = paddingX;
    int boxY = screenHeight - boxHeight - paddingY;

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, _boxColor);
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

void zappy::gui::raylib::GameMenu::_renderHelp(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = static_cast<int>(10 * scaleX);
    const int paddingY = static_cast<int>(10 * scaleY);
    const int lineHeight = static_cast<int>(20 * scaleY);
    const int titleHeight = static_cast<int>(35 * scaleY);
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
        "[P] Players infos",
    };

    int y = screenY + titleHeight + paddingY;
    for (const auto &line : lines) {
        DrawTextEx(GetFontDefault(), line.c_str(),
            {static_cast<float>(screenX + paddingX), static_cast<float>(y)},
            static_cast<float>(fontSize), 1, this->_textColor);
        y += lineHeight + paddingY;
    }
}

void zappy::gui::raylib::GameMenu::_renderFreq(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingY = static_cast<int>(10 * scaleY);
    const int boxWidth = static_cast<int>(200 * scaleX);
    const int boxHeight = static_cast<int>(40 * scaleY);
    const int fontSize = static_cast<int>(this->_fontSize * scaleY);

    const int boxX = (screenWidth - boxWidth) / 2;
    const int boxY = paddingY;

    const std::string freqText = "Frequency: " + std::to_string(this->_gameState->getFrequency());

    DrawRectangle(boxX, boxY, boxWidth, boxHeight, _boxColor);
    DrawRectangleLines(boxX, boxY, boxWidth, boxHeight, WHITE);

    const int textWidth = MeasureText(freqText.c_str(), fontSize);
    const int textX = boxX + (boxWidth - textWidth) / 2;
    const int textY = boxY + (boxHeight - fontSize) / 2;

    DrawText(freqText.c_str(), textX, textY, fontSize, this->_textColor);
}

void zappy::gui::raylib::GameMenu::_renderPlayersInfos(const int &screenWidth, const int &screenHeight) const
{
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    const int paddingX = 20 * scaleX;
    const int paddingY = 20 * scaleY;
    const int spacingX = 5 * scaleX;
    const int spacingY = 5 * scaleY;
    const int textSize = static_cast<int>(this->_fontSize * scaleY);
    const int boxWidth = textSize * 10;
    const int totalLines = 4 + game::RESOURCE_QUANTITY;
    const int boxHeight = totalLines * textSize + (totalLines - 1) * spacingY;

    const int x = screenWidth - paddingX;
    const int y = paddingY;

    if (this->_numberPlayerDisplayed <= 0) {
        const std::string text = "There is actually no player";
        const int textWidth = MeasureText(text.c_str(), textSize);
        DrawText(text.c_str(), x - textWidth, y, textSize, WHITE);
        return;
    }

    int available = static_cast<int>(_playersInfos.size()) - _displayedPlayersIndex;
    int toDraw = std::min(_numberPlayerDisplayed, available);

    const int columns = 2;

    for (int i = 0; i < toDraw; ++i) {
        int dataIdx = _displayedPlayersIndex + i;
        int col = i % columns;
        int row = i / columns;

        int x = screenWidth
              - paddingX
              - (columns - 1 - col) * (boxWidth + spacingX);

        int y = paddingY + row * (boxHeight + spacingY);

        _renderPlayerInfo(
            _playersInfos[dataIdx],
            x, y,
            textSize,
            spacingY,
            boxWidth,
            boxHeight
        );
    }
}

void zappy::gui::raylib::GameMenu::_renderPlayerInfo(
    const MenuPlayerInfo &playerInfo,
    const int &screenWidth, const int &y,
    const int &textSize,
    const int &spacingY,
    const int &boxWidth,
    const int &boxHeight
) const {
    const auto &player = this->_gameState->getPlayerById(playerInfo.id);
    const auto &inv = player.getInventory();

    const int boxX = screenWidth - boxWidth;

    DrawRectangle(boxX, y - 10, boxWidth, boxHeight + 20, Fade(GRAY, 0.5f));

    const int centerX = boxX + boxWidth / 2;
    int currentY = y;

    auto drawCentered = [&](const std::string &str) {
        int textWidth = MeasureText(str.c_str(), textSize);
        DrawText(str.c_str(), centerX - textWidth / 2, currentY, textSize, WHITE);
        currentY += textSize + spacingY;
    };

    drawCentered("ID: " + std::to_string(playerInfo.id));
    drawCentered("Team: " + player.teamName);
    drawCentered("Level: " + std::to_string(player.level));
    drawCentered("Position: " + std::to_string(player.x) + ", " + std::to_string(player.y));

    for (size_t i = 0; i < game::RESOURCE_QUANTITY; ++i) {
        game::Resource res = static_cast<game::Resource>(i);
        std::string resStr = game::getName(res) + ": " + std::to_string(inv.getResourceQuantity(res));
        drawCentered(resStr);
    }
}
