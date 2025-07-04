/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.cpp
*/

#include "PauseMenu.hpp"
#include "Menu/MenuMacros.hpp"

using namespace zappy::gui::raylib;

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
    this->_boxColor = {0, 0, 0, 180};
    this->_selectedColor = {100, 100, 100, 200};
    this->_normalColor = {50, 50, 50, 150};

    this->_themes = {
        {"CLASSIC", LoadTexture(assets::BASIC_THEME_IMAGE_PATH.c_str())},
        {"POKEMON", LoadTexture(assets::POKEMON_THEME_IMAGE_PATH.c_str())},
    };
}

void zappy::gui::raylib::PauseMenu::PauseMenu::handleInput(const InputManager &inputManager)
{
    if (!this->_display) {
        if (inputManager.isKeyPressed(this->_key)) {
            this->_display = true;
            return;
        }
    }

    if (this->_menuState == PauseMenuState::MAIN_MENU) {
        if (inputManager.isKeyPressed(KEY_UP))
            this->_selectedButton = (this->_selectedButton - 1 + 3) % 3;

        if (inputManager.isKeyPressed(KEY_DOWN))
            this->_selectedButton = (this->_selectedButton + 1) % 3;

        if (inputManager.isKeyPressed(KEY_ENTER) || inputManager.isKeyPressed(KEY_SPACE))
            switch (this->_selectedButton) {
                case 0:
                    this->_display = false;
                    break;
                case 1:
                    this->_menuState = PauseMenuState::THEME_MENU;
                    break;
                case 2:
                    CloseWindow();
                    break;
            }

        if (inputManager.isKeyPressed(this->_key))
            this->_display = false;

    } else if (_menuState == PauseMenuState::THEME_MENU) {
        if (inputManager.isKeyPressed(KEY_LEFT))
            this->_selectedTheme = (this->_selectedTheme - 1 + this->_themes.size()) % this->_themes.size();

        if (inputManager.isKeyPressed(KEY_RIGHT))
            this->_selectedTheme = (this->_selectedTheme + 1) % this->_themes.size();

        if (inputManager.isKeyPressed(KEY_ENTER)) {
            this->_shouldChangeScene = !this->_shouldChangeScene;
            this->_sceneType = zappy::gui::raylib::SceneType::POKEMON;
        }

        if (inputManager.isKeyPressed(this->_key))
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
    const int buttonWidth = static_cast<int>(200 * scale);
    const int buttonHeight = static_cast<int>(50 * scale);
    const int buttonSpacing = static_cast<int>(20 * scale);
    const int buttonX = x + (w - buttonWidth) / 2;

    DrawText("PAUSE", x + (w - MeasureText("PAUSE", titleFontSize)) / 2, y + static_cast<int>(30 * scale), titleFontSize, WHITE);

    std::vector<std::string> labels = {"RESUME", "CHANGE THEME", "LEAVE"};
    for (int i = 0; i < 3; ++i) {
        int btnY = y + titleFontSize + static_cast<int>(50 * scale) + i * (buttonHeight + buttonSpacing);
        Color color = (this->_selectedButton == i) ? this->_selectedColor : this->_normalColor;
        DrawRectangle(buttonX, btnY, buttonWidth, buttonHeight, color);
        DrawRectangleLines(buttonX, btnY, buttonWidth, buttonHeight, WHITE);

        int textSize = static_cast<int>(buttonHeight * 0.4f);
        while (MeasureText(labels[i].c_str(), textSize) > buttonWidth - 20 && textSize > 5)
            textSize--;

        DrawText(labels[i].c_str(),
            buttonX + (buttonWidth - MeasureText(labels[i].c_str(), textSize)) / 2,
            btnY + (buttonHeight - textSize) / 2,
            textSize,
            WHITE);
    }
}

void zappy::gui::raylib::PauseMenu::PauseMenu::_renderThemeMenu(float scale, int x, int y, int w, int h) const
{
    const int titleFontSize = static_cast<int>(40 * scale);
    const int spacingY = static_cast<int>(30 * scale);
    const int paddingTop = static_cast<int>(40 * scale);

    const std::string title = "SELECT THEME";
    DrawText(title.c_str(), x + (w - MeasureText(title.c_str(), titleFontSize)) / 2,
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

    const std::string backText = "BACKSPACE to return";
    int instrY = y + h - static_cast<int>(30 * scale);
    int textSize = static_cast<int>(16 * scale);
    DrawText(backText.c_str(), x + (w - MeasureText(backText.c_str(), textSize)) / 2, instrY, textSize, LIGHTGRAY);
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
        this->_shouldChangeScene = !this->_shouldChangeScene;
        return true;
    }
    return false;
}
