/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.cpp
*/

#include "PauseMenu.hpp"
#include "Menu/MenuMacros.hpp"

using namespace zappy::gui::raylib;

constexpr const char *LABEL_RESUME = "RESUME";
constexpr const char *LABEL_CHANGE_THEME = "CHANGE THEME";
constexpr const char *LABEL_LEAVE = "LEAVE";
constexpr const char *LABEL_PAUSE = "PAUSE";
constexpr const char *LABEL_SELECT_THEME = "SELECT THEME";
constexpr const char *LABEL_BACK = "ESCAPE to return";
constexpr const char *THEME_CLASSIC = "CLASSIC";
constexpr const char *THEME_POKEMON = "POKEMON";

zappy::gui::raylib::PauseMenu::PauseMenu::PauseMenu() :
    _fontSize(DEFAULT_FONT_SIZE),
    _textColor(DEFAULT_TEXT_COLOR),
    _display(false),
    _shouldChangeScene(false),
    _sceneType(SceneType::BASIC),
    _key(KEY_ESCAPE),
    _selectedButton(0),
    _selectedTheme(0),
    _menuState(PauseMenuState::MAIN_MENU),
    _isThemeMenu(false) {}

zappy::gui::raylib::PauseMenu::PauseMenu::~PauseMenu()
{
    for (auto& theme : this->_themes)
        UnloadTexture(theme.texture);
}

void zappy::gui::raylib::PauseMenu::PauseMenu::init()
{
    constexpr Color boxColor = {0, 0, 0, 180};
    constexpr Color selectedColor = {100, 100, 100, 200};
    constexpr Color normalColor = {50, 50, 50, 150};

    this->_boxColor = boxColor;
    this->_selectedColor = selectedColor;
    this->_normalColor = normalColor;

    this->_themes = {
        {THEME_CLASSIC, LoadTexture(assets::BASIC_THEME_IMAGE_PATH.c_str())},
        {THEME_POKEMON, LoadTexture(assets::POKEMON_THEME_IMAGE_PATH.c_str())},
    };
}

void zappy::gui::raylib::PauseMenu::PauseMenu::handleInput(const InputManager &inputManager)
{
    if (!this->_display) {
        if (inputManager.getKeyState(this->_key) == InputManager::KeyState::RELEASED)
            this->_display = true;
        return;
    }

    if (this->_menuState == PauseMenuState::MAIN_MENU) {
        if (inputManager.isKeyReleased(KEY_UP))
            this->_selectedButton = (this->_selectedButton - 1 + 3) % 3;

        if (inputManager.isKeyReleased(KEY_DOWN))
            this->_selectedButton = (this->_selectedButton + 1) % 3;

        if (inputManager.isKeyReleased(KEY_ENTER) || inputManager.isKeyReleased(KEY_SPACE))
            switch (this->_selectedButton) {
                case 0:
                    this->_display = false;
                    break;
                case 1:
                    this->_menuState = PauseMenuState::THEME_MENU;
                    this->_selectedButton = 0;
                    break;
                case 2:
                    CloseWindow();
                    break;
            }

        if (inputManager.getKeyState(this->_key) == InputManager::KeyState::RELEASED) {
            this->_selectedButton = 0;
            this->_display = false;
        }
    } else if (_menuState == PauseMenuState::THEME_MENU) {
        if (inputManager.isKeyReleased(KEY_LEFT))
            this->_selectedTheme = (this->_selectedTheme - 1 + this->_themes.size()) % this->_themes.size();

        if (inputManager.isKeyReleased(KEY_RIGHT))
            this->_selectedTheme = (this->_selectedTheme + 1) % this->_themes.size();

        if (inputManager.isKeyReleased(KEY_ENTER)) {
            this->_shouldChangeScene = true;
            this->_menuState = PauseMenuState::MAIN_MENU;

            if (this->_themes[this->_selectedTheme].name == THEME_CLASSIC)
                this->_sceneType = SceneType::BASIC;
            else if (this->_themes[this->_selectedTheme].name == THEME_POKEMON)
                this->_sceneType = SceneType::POKEMON;
        }

        if (inputManager.getKeyState(this->_key) == InputManager::KeyState::RELEASED)
            this->_menuState = PauseMenuState::MAIN_MENU;
    }
}

void zappy::gui::raylib::PauseMenu::PauseMenu::update() {}

void zappy::gui::raylib::PauseMenu::PauseMenu::render() const
{
    if (!this->_display)
        return;

    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();
    const float scale = _getUniformScale();

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.1f));

    const int menuSize = static_cast<int>(500 * scale);
    const int menuX = (screenWidth - menuSize) / 2;
    const int menuY = (screenHeight - menuSize) / 2;

    DrawRectangle(menuX, menuY, menuSize, menuSize, this->_boxColor);
    DrawRectangleLines(menuX, menuY, menuSize, menuSize, WHITE);

    if (this->_menuState == PauseMenuState::MAIN_MENU)
        _renderPauseMenu(scale, menuX, menuY, menuSize, menuSize);
    else
        _renderThemeMenu(scale, menuX, menuY, menuSize, menuSize);
}

void zappy::gui::raylib::PauseMenu::PauseMenu::_renderPauseMenu(float scale, int x, int y, int w, int h) const
{
    const int titleFontSize = static_cast<int>(40 * scale);
    const int buttonWidth = static_cast<int>(300 * scale);
    const int buttonHeight = static_cast<int>(70 * scale);
    const int buttonSpacing = static_cast<int>(30 * scale);
    const int buttonX = x + (w - buttonWidth) / 2;

    const std::vector<std::string> labels = {LABEL_RESUME, LABEL_CHANGE_THEME, LABEL_LEAVE};
    const int totalButtonsHeight = static_cast<int>(labels.size()) * buttonHeight +
                                   static_cast<int>(labels.size() - 1) * buttonSpacing;

    const int startY = y + (h - totalButtonsHeight) / 2;

    DrawText(
        LABEL_PAUSE,
        x + (w - MeasureText(LABEL_PAUSE, titleFontSize)) / 2,
        y + static_cast<int>(30 * scale),
        titleFontSize,
        WHITE
    );

    for (int i = 0; i < static_cast<int>(labels.size()); ++i) {
        int btnY = startY + i * (buttonHeight + buttonSpacing);
        Color color = (this->_selectedButton == i) ? this->_selectedColor : this->_normalColor;

        DrawRectangle(buttonX, btnY, buttonWidth, buttonHeight, color);
        DrawRectangleLines(buttonX, btnY, buttonWidth, buttonHeight, WHITE);

        int textSize = static_cast<int>(buttonHeight * 0.5f);
        while (MeasureText(labels[i].c_str(), textSize) > buttonWidth - 20 && textSize > 5)
            textSize--;

        DrawText(
            labels[i].c_str(),
            buttonX + (buttonWidth - MeasureText(labels[i].c_str(), textSize)) / 2,
            btnY + (buttonHeight - textSize) / 2,
            textSize,
            WHITE
        );
    }
}

void zappy::gui::raylib::PauseMenu::PauseMenu::_renderThemeMenu(float scale, int x, int y, int w, int h) const
{
    const int titleFontSize = static_cast<int>(40 * scale);
    const int spacingY = static_cast<int>(30 * scale);
    const int paddingTop = static_cast<int>(40 * scale);

    DrawText(LABEL_SELECT_THEME, x + (w - MeasureText(LABEL_SELECT_THEME, titleFontSize)) / 2,
        y + paddingTop, titleFontSize, WHITE);

    const int yPos = y + paddingTop + titleFontSize + spacingY;

    if (this->_themes.size() <= 2) {
        const int itemWidth = static_cast<int>(w * 0.4f);
        const int itemHeight = static_cast<int>(h * 0.4f);
        const int spacing = static_cast<int>(30 * scale);

        int totalWidth = static_cast<int>(this->_themes.size()) * itemWidth + (this->_themes.size() - 1) * spacing;
        int startX = x + (w - totalWidth) / 2;

        for (size_t i = 0; i < this->_themes.size(); ++i) {
            int posX = startX + i * (itemWidth + spacing);
            _renderTheme(this->_themes[i], posX, yPos, itemWidth, itemHeight, static_cast<int>(i) == this->_selectedTheme);
        }

    } else {
        const int centerIndex = this->_selectedTheme;
        const int previewWidth = static_cast<int>(w * 0.3f);
        const int previewHeight = static_cast<int>(h * 0.4f);
        const int centerX = x + w / 2;
        const int spacing = static_cast<int>(40 * scale);

        for (int offset = -1; offset <= 1; ++offset) {
            int index = (centerIndex + offset + this->_themes.size()) % this->_themes.size();
            float itemScale = (offset == 0) ? 1.0f : 0.8f;

            int itemW = static_cast<int>(previewWidth * itemScale);
            int itemH = static_cast<int>(previewHeight * itemScale);
            int posX = centerX + offset * (previewWidth + spacing) - itemW / 2;

            _renderTheme(_themes[index], posX, yPos, itemW, itemH, offset == 0);
        }
    }

    int instrY = y + h - static_cast<int>(30 * scale);
    int textSize = static_cast<int>(16 * scale);
    DrawText(LABEL_BACK, x + (w - MeasureText(LABEL_BACK, textSize)) / 2, instrY, textSize, LIGHTGRAY);
}

void zappy::gui::raylib::PauseMenu::PauseMenu::_renderTheme(const Theme& theme, int x, int y, int width, int height, bool selected) const
{
    float textureScale = static_cast<float>(width) / theme.texture.width;
    DrawTextureEx(theme.texture, {(float)x, (float)y}, 0.0f, textureScale, WHITE);

    const int boxHeight = static_cast<int>(height * 0.2f);
    const int boxY = y + height + static_cast<int>(10 * textureScale);

    Color boxColor = selected ? _selectedColor : this->_normalColor;
    DrawRectangle(x, boxY, width, boxHeight, boxColor);
    DrawRectangleLines(x, boxY, width, boxHeight, WHITE);

    int textSize = static_cast<int>(boxHeight * 0.6f);
    while (MeasureText(theme.name.c_str(), textSize) > width - 10 && textSize > 5)
        textSize--;

    DrawText(theme.name.c_str(),
        x + (width - MeasureText(theme.name.c_str(), textSize)) / 2,
        boxY + (boxHeight - textSize) / 2,
        textSize,
        WHITE);
}

float zappy::gui::raylib::PauseMenu::_getUniformScale() const
{
    return std::min(
        static_cast<float>(GetRenderWidth()) / DEFAULT_SCREEN_WIDTH,
        static_cast<float>(GetRenderHeight()) / DEFAULT_SCREEN_HEIGHT
    );
}

bool zappy::gui::raylib::PauseMenu::shouldChangeScene()
{
    if (this->_shouldChangeScene) {
        this->_shouldChangeScene = false;
        return true;
    }
    return false;
}
