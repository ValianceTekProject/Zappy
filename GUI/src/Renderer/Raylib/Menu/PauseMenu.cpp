/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.cpp
*/

#include "PauseMenu.hpp"

zappy::gui::raylib::PauseMenu::PauseMenu() :
    _fontSize(DEFAULT_FONT_SIZE),
    _textColor(DEFAULT_TEXT_COLOR),
    _display(false),
    _key(KEY_ESCAPE),
    _selectedButton(0) {}

void zappy::gui::raylib::PauseMenu::init()
{
    _boxColor = {0, 0, 0, 180};
    _selectedColor = {100, 100, 100, 200};
    _normalColor = {50, 50, 50, 150};
}

void zappy::gui::raylib::PauseMenu::handleInput(const InputManager &inputManager)
{
    if (inputManager.isKeyPressed(_key)) {
        _display = !_display;
        return;
    }

    if (_display) {
        if (inputManager.isKeyPressed(KEY_UP)) {
            _selectedButton = (_selectedButton - 1 + 3) % 3;
        }
        if (inputManager.isKeyPressed(KEY_DOWN)) {
            _selectedButton = (_selectedButton + 1) % 3;
        }

        if (inputManager.isKeyPressed(KEY_SPACE) || inputManager.isKeyPressed(KEY_ENTER)) {
            switch (_selectedButton) {
                case 0:
                    _display = false;
                    break;
                case 1:

                case 2:
                    CloseWindow();
                    break;
            }
        }

        if (inputManager.isKeyPressed(KEY_ESCAPE)) {
            _display = false;
        }
    }
}

void zappy::gui::raylib::PauseMenu::update() {}

void zappy::gui::raylib::PauseMenu::render() const
{
    if (!_display)
        return;

    const int screenWidth = GetRenderWidth();
    const int screenHeight = GetRenderHeight();

    const float scaleX = static_cast<float>(screenWidth) / DEFAULT_SCREEN_WIDTH;
    const float scaleY = static_cast<float>(screenHeight) / DEFAULT_SCREEN_HEIGHT;

    DrawRectangle(0, 0, screenWidth, screenHeight, {0, 0, 0, 100});

    const int menuWidth = static_cast<int>(400 * scaleX);
    const int menuHeight = static_cast<int>(400 * scaleY);
    const int menuX = (screenWidth - menuWidth) / 2;
    const int menuY = (screenHeight - menuHeight) / 2;

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, _boxColor);
    DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, WHITE);

    const int titleFontSize = static_cast<int>(40 * scaleY);
    const std::string title = "PAUSE";
    const int titleWidth = MeasureText(title.c_str(), titleFontSize);
    const int titleX = menuX + (menuWidth - titleWidth) / 2;
    const int titleY = menuY + static_cast<int>(40 * scaleY);

    DrawText(title.c_str(), titleX, titleY, titleFontSize, WHITE);

    const int buttonFontSize = static_cast<int>(24 * scaleY);
    const int padding = static_cast<int>(40 * scaleX);

    const std::string resumeText = "RESUME";
    const std::string themeText = "CHANGE THEME";
    const std::string quitText = "QUIT";

    int maxTextWidth = MeasureText(themeText.c_str(), buttonFontSize);
    maxTextWidth = std::max(maxTextWidth, MeasureText(resumeText.c_str(), buttonFontSize));
    maxTextWidth = std::max(maxTextWidth, MeasureText(quitText.c_str(), buttonFontSize));

    const int buttonWidth = maxTextWidth + padding;
    const int buttonHeight = static_cast<int>(50 * scaleY);
    const int buttonX = menuX + (menuWidth - buttonWidth) / 2;
    const int buttonSpacing = static_cast<int>(20 * scaleY);

    const int resumeY = titleY + titleFontSize + static_cast<int>(50 * scaleY);
    Color resumeColor = (_selectedButton == 0) ? _selectedColor : _normalColor;
    DrawRectangle(buttonX, resumeY, buttonWidth, buttonHeight, resumeColor);
    DrawRectangleLines(buttonX, resumeY, buttonWidth, buttonHeight, WHITE);
    const int resumeTextX = buttonX + (buttonWidth - MeasureText(resumeText.c_str(), buttonFontSize)) / 2;
    const int resumeTextY = resumeY + (buttonHeight - buttonFontSize) / 2;
    DrawText(resumeText.c_str(), resumeTextX, resumeTextY, buttonFontSize, WHITE);

    const int themeY = resumeY + buttonHeight + buttonSpacing;
    Color themeColor = (_selectedButton == 1) ? _selectedColor : _normalColor;
    DrawRectangle(buttonX, themeY, buttonWidth, buttonHeight, themeColor);
    DrawRectangleLines(buttonX, themeY, buttonWidth, buttonHeight, WHITE);
    const int themeTextX = buttonX + (buttonWidth - MeasureText(themeText.c_str(), buttonFontSize)) / 2;
    const int themeTextY = themeY + (buttonHeight - buttonFontSize) / 2;
    DrawText(themeText.c_str(), themeTextX, themeTextY, buttonFontSize, WHITE);

    const int quitY = themeY + buttonHeight + buttonSpacing;
    Color quitColor = (_selectedButton == 2) ? _selectedColor : _normalColor;
    DrawRectangle(buttonX, quitY, buttonWidth, buttonHeight, quitColor);
    DrawRectangleLines(buttonX, quitY, buttonWidth, buttonHeight, WHITE);
    const int quitTextX = buttonX + (buttonWidth - MeasureText(quitText.c_str(), buttonFontSize)) / 2;
    const int quitTextY = quitY + (buttonHeight - buttonFontSize) / 2;
    DrawText(quitText.c_str(), quitTextX, quitTextY, buttonFontSize, WHITE);

    const int instructionFontSize = static_cast<int>(16 * scaleY);
    const std::string instructions = "Use UP/DOWN arrows to navigate, ENTER/SPACE to select, ESC to close";
    const int instructionWidth = MeasureText(instructions.c_str(), instructionFontSize);
    const int instructionX = (screenWidth - instructionWidth) / 2;
    const int instructionY = menuY + menuHeight + static_cast<int>(20 * scaleY);
    DrawText(instructions.c_str(), instructionX, instructionY, instructionFontSize, LIGHTGRAY);
}
