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
    _selectedButton(0),
    _selectedTheme(0) {}

void zappy::gui::raylib::PauseMenu::init()
{
    _boxColor = {0, 0, 0, 180};
    _selectedColor = {100, 100, 100, 200};
    _normalColor = {50, 50, 50, 150};

    _classicTexture = LoadTexture(zappy::gui::raylib::assets::BASIC_THEME_IMAGE_PATH.c_str());
    _pokemonTexture = LoadTexture(zappy::gui::raylib::assets::POKEMON_THEME_IMAGE_PATH.c_str());
}

void zappy::gui::raylib::PauseMenu::handleInput(const InputManager &inputManager)
{
    if (inputManager.isKeyPressed(_key)) {
        _display = !_display;
        return;
    }

    if (!_display)
        return;

    if (inputManager.isKeyPressed(KEY_UP)) {
        _selectedButton = (_selectedButton - 1 + 3) % 3;
    }
    if (inputManager.isKeyPressed(KEY_DOWN)) {
        _selectedButton = (_selectedButton + 1) % 3;
    }

    if (_selectedButton == 1) {
        if (inputManager.isKeyPressed(KEY_LEFT)) {
            _selectedTheme = (_selectedTheme - 1 + 2) % 2;
        }
        if (inputManager.isKeyPressed(KEY_RIGHT)) {
            _selectedTheme = (_selectedTheme + 1) % 2;
        }
    }

    if (inputManager.isKeyPressed(KEY_SPACE) || inputManager.isKeyPressed(KEY_ENTER)) {
        switch (_selectedButton) {
            case 0:
                _display = false;
                break;
            case 1:
                if (_selectedTheme == 0) {
                    // thème classic
                } else {
                    // thème pokemon
                }
                break;
            case 2:
                CloseWindow();
                break;
        }
    }

    if (inputManager.isKeyPressed(KEY_ESCAPE)) {
        _display = false;
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

    DrawRectangle(0, 0, screenWidth, screenHeight, Fade(BLACK, 0.1f));

    const int menuWidth = static_cast<int>(500 * scaleX);
    const int menuHeight = static_cast<int>(500 * scaleY);
    const int menuX = (screenWidth - menuWidth) / 2;
    const int menuY = (screenHeight - menuHeight) / 2;

    DrawRectangle(menuX, menuY, menuWidth, menuHeight, _boxColor);
    DrawRectangleLines(menuX, menuY, menuWidth, menuHeight, WHITE);

    const int titleFontSize = static_cast<int>(40 * scaleY);
    const std::string title = "PAUSE";
    const int titleX = menuX + (menuWidth - MeasureText(title.c_str(), titleFontSize)) / 2;
    const int titleY = menuY + static_cast<int>(40 * scaleY);

    DrawText(title.c_str(), titleX, titleY, titleFontSize, WHITE);

    const int buttonFontSize = static_cast<int>(24 * scaleY);

    const int buttonWidth = static_cast<int>(200 * scaleX);
    const int buttonHeight = static_cast<int>(50 * scaleY);
    const int buttonX = menuX + (menuWidth - buttonWidth) / 2;
    const int buttonSpacing = static_cast<int>(20 * scaleY);

    const int resumeY = titleY + titleFontSize + static_cast<int>(50 * scaleY);
    Color resumeColor = (_selectedButton == 0) ? _selectedColor : _normalColor;
    DrawRectangle(buttonX, resumeY, buttonWidth, buttonHeight, resumeColor);
    DrawRectangleLines(buttonX, resumeY, buttonWidth, buttonHeight, WHITE);
    DrawText("RESUME", buttonX + (buttonWidth - MeasureText("RESUME", buttonFontSize)) / 2,
    resumeY + (buttonHeight - buttonFontSize) / 2, buttonFontSize, WHITE);

    const int themeY = resumeY + buttonHeight + buttonSpacing;
    const int previewSize = static_cast<int>(100 * scaleY);
    const int imageSpacing = static_cast<int>(100 * scaleX);

    const int image1X = menuX + (menuWidth / 2) - previewSize - (imageSpacing / 2);
    const int image2X = menuX + (menuWidth / 2) + (imageSpacing / 2);
    const int imageY = themeY;

    DrawTextureEx(_classicTexture, {(float)image1X, (float)imageY}, 0.0f, (float)previewSize / _classicTexture.width, WHITE);
    DrawTextureEx(_pokemonTexture, {(float)image2X, (float)imageY}, 0.0f, (float)previewSize / _pokemonTexture.width, WHITE);

    const int themeButtonY = imageY + previewSize + static_cast<int>(10 * scaleY);
    const int themeButtonWidth = static_cast<int>(180 * scaleX); // largeur augmentée ici
    const int themeButtonHeight = static_cast<int>(40 * scaleY);

    Color theme1Color = (_selectedButton == 1 && _selectedTheme == 0) ? _selectedColor : _normalColor;
    Color theme2Color = (_selectedButton == 1 && _selectedTheme == 1) ? _selectedColor : _normalColor;

    DrawRectangle(image1X - (themeButtonWidth - previewSize) / 2, themeButtonY, themeButtonWidth, themeButtonHeight, theme1Color);
    DrawRectangleLines(image1X - (themeButtonWidth - previewSize) / 2, themeButtonY, themeButtonWidth, themeButtonHeight, WHITE);
    DrawText("CLASSIC",
            image1X - (themeButtonWidth - previewSize) / 2 + (themeButtonWidth - MeasureText("CLASSIC", buttonFontSize)) / 2,
            themeButtonY + (themeButtonHeight - buttonFontSize) / 2,
            buttonFontSize, WHITE);

    DrawRectangle(image2X - (themeButtonWidth - previewSize) / 2, themeButtonY, themeButtonWidth, themeButtonHeight, theme2Color);
    DrawRectangleLines(image2X - (themeButtonWidth - previewSize) / 2, themeButtonY, themeButtonWidth, themeButtonHeight, WHITE);
    DrawText("POKEMON",
            image2X - (themeButtonWidth - previewSize) / 2 + (themeButtonWidth - MeasureText("POKEMON", buttonFontSize)) / 2,
            themeButtonY + (themeButtonHeight - buttonFontSize) / 2,
            buttonFontSize, WHITE);

    const int quitY = themeButtonY + themeButtonHeight + buttonSpacing;
    Color quitColor = (_selectedButton == 2) ? _selectedColor : _normalColor;
    DrawRectangle(buttonX, quitY, buttonWidth, buttonHeight, quitColor);
    DrawRectangleLines(buttonX, quitY, buttonWidth, buttonHeight, WHITE);
    DrawText("QUIT", buttonX + (buttonWidth - MeasureText("QUIT", buttonFontSize)) / 2,
quitY + (buttonHeight - buttonFontSize) / 2, buttonFontSize, WHITE);

    const int instructionFontSize = static_cast<int>(16 * scaleY);
    const std::string instructions = "Use UP/DOWN to navigate, LEFT/RIGHT to choose theme, ENTER to select";
    const int instructionWidth = MeasureText(instructions.c_str(), instructionFontSize);
    const int instructionX = (screenWidth - instructionWidth) / 2;
    const int instructionY = menuY + menuHeight + static_cast<int>(20 * scaleY);
    DrawText(instructions.c_str(), instructionX, instructionY, instructionFontSize, LIGHTGRAY);
}

zappy::gui::raylib::PauseMenu::~PauseMenu()
{
    UnloadTexture(_classicTexture);
    UnloadTexture(_pokemonTexture);
}
