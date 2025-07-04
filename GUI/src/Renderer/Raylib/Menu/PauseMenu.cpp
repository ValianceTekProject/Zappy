/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.cpp
*/

#include "PauseMenu.hpp"

using namespace zappy::gui::raylib;

PauseMenu::PauseMenu() :
    _fontSize(DEFAULT_FONT_SIZE),
    _textColor(DEFAULT_TEXT_COLOR),
    _display(false),
    _shouldChangeScene(false),
    _sceneType(SceneType::BASIC),
    _key(KEY_ESCAPE),
    _selectedButton(0),
    _selectedTheme(0),
    _menuState(PauseMenuState::MAIN_MENU) {}

PauseMenu::~PauseMenu()
{
    for (auto& theme : _themes)
        UnloadTexture(theme.texture);
}

void PauseMenu::init()
{
    _boxColor = {0, 0, 0, 180};
    _selectedColor = {100, 100, 100, 200};
    _normalColor = {50, 50, 50, 150};

    _themes = {
        {"CLASSIC", LoadTexture(assets::BASIC_THEME_IMAGE_PATH.c_str())},
        {"POKEMON", LoadTexture(assets::POKEMON_THEME_IMAGE_PATH.c_str())}
    };
}

void PauseMenu::handleInput(const InputManager &inputManager)
{
    if (inputManager.isKeyPressed(_key)) {
        _display = !_display;
        return;
    }

    if (!_display) return;

    if (_menuState == PauseMenuState::MAIN_MENU) {
        if (inputManager.isKeyPressed(KEY_UP))
            _selectedButton = (_selectedButton - 1 + 3) % 3;

        if (inputManager.isKeyPressed(KEY_DOWN))
            _selectedButton = (_selectedButton + 1) % 3;

        if (inputManager.isKeyPressed(KEY_ENTER) || inputManager.isKeyPressed(KEY_SPACE))
            switch (_selectedButton) {
                case 0: _display = false; break;
                case 1: _menuState = PauseMenuState::THEME_MENU; break;
                case 2: CloseWindow(); break;
            }
    } else if (_menuState == PauseMenuState::THEME_MENU) {
        if (inputManager.isKeyPressed(KEY_LEFT))

            _selectedTheme = (_selectedTheme - 1 + _themes.size()) % _themes.size();
        if (inputManager.isKeyPressed(KEY_RIGHT))

            _selectedTheme = (_selectedTheme + 1) % _themes.size();

        if (inputManager.isKeyPressed(KEY_ESCAPE))
            _menuState = PauseMenuState::MAIN_MENU;

        if (inputManager.isKeyPressed(KEY_ENTER) || inputManager.isKeyPressed(KEY_SPACE))
            _menuState = PauseMenuState::MAIN_MENU;
    }

    if (inputManager.isKeyPressed(KEY_ESCAPE))
        _display = false;
}

void PauseMenu::update() {}

void PauseMenu::render() const
{
    if (!_display) return;

    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();
    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.1f));

    const int menuWidth = static_cast<int>(500 * scaleX);
    const int menuHeight = static_cast<int>(500 * scaleY);
    const int menuX = (screenWidth - menuWidth) / 2;
    const int menuY = (screenHeight - menuHeight) / 2;

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, _boxColor);
    DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, WHITE);

    if (_menuState == PauseMenuState::MAIN_MENU)
        _renderPauseMenu(scaleX, scaleY, menuX, menuY, menuWidth, menuHeight);
    else
        _renderThemeMenu(scaleX, scaleY, menuX, menuY, menuWidth, menuHeight);
}

void PauseMenu::_renderPauseMenu(float scaleX, float scaleY, int x, int y, int w, int h) const
{
    const int fontSize = static_cast<int>(40 * scaleY);
    const int buttonFontSize = static_cast<int>(24 * scaleY);
    const int buttonWidth = static_cast<int>(200 * scaleX);
    const int buttonHeight = static_cast<int>(50 * scaleY);
    const int buttonX = x + (w - buttonWidth) / 2;
    const int buttonSpacing = static_cast<int>(20 * scaleY);

    DrawText("PAUSE", x + (w - MeasureText("PAUSE", fontSize)) / 2, y + 40, fontSize, WHITE);

    std::vector<std::string> labels = {"RESUME", "CHANGE THEME", "LEAVE"};
    for (int i = 0; i < 3; ++i) {
        int btnY = y + fontSize + 80 + i * (buttonHeight + buttonSpacing);
        Color color = (_selectedButton == i) ? _selectedColor : _normalColor;
        DrawRectangle(buttonX, btnY, buttonWidth, buttonHeight, color);
        DrawRectangleLines(buttonX, btnY, buttonWidth, buttonHeight, WHITE);
        DrawText(labels[i].c_str(), buttonX + (buttonWidth - MeasureText(labels[i].c_str(), buttonFontSize)) / 2,
        btnY + (buttonHeight - buttonFontSize) / 2, buttonFontSize, WHITE);
    }
}

void PauseMenu::_renderThemeMenu(float scaleX, float scaleY, int x, int y, int w, int h) const
{
    const int titleSize = static_cast<int>(32 * scaleY);
    DrawText("SELECT THEME", x + (w - MeasureText("SELECT THEME", titleSize)) / 2, y + 30, titleSize, WHITE);

    const int previewSize = static_cast<int>(100 * scaleY);
    const int spacing = static_cast<int>(120 * scaleX);
    int startX = x + (w - (_themes.size() * spacing)) / 2;
    const int yPos = y + 100;

    for (size_t i = 0; i < _themes.size(); ++i)
        _renderTheme(_themes[i], startX + i * spacing, yPos, previewSize, previewSize, static_cast<int>(i) == _selectedTheme);

    const std::string backText = "ESC to return";
    int instrY = y + h - 30;
    DrawText(backText.c_str(), x + (w - MeasureText(backText.c_str(), 16)) / 2, instrY, 16, LIGHTGRAY);
}

void PauseMenu::_renderTheme(const Theme& theme, int x, int y, int width, int height, bool selected) const
{
    DrawTextureEx(theme.texture, {(float)x, (float)y}, 0.0f, (float)width / theme.texture.width, WHITE);

    Color boxColor = selected ? _selectedColor : _normalColor;
    DrawRectangle(x, y + height + 10, width, 30, boxColor);
    DrawRectangleLines(x, y + height + 10, width, 30, WHITE);

    DrawText(theme.name.c_str(), x + (width - MeasureText(theme.name.c_str(), 16)) / 2, y + height + 18, 16, WHITE);
}
