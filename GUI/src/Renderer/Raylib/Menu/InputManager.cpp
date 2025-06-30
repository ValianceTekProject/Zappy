/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** InputManager.cpp
*/

#include "InputManager.hpp"

/**
 * @brief Updates the state of keyboard keys in the input manager
 *
 * This method iterates through existing key states and updates their status based on current key press conditions.
 * It handles key state transitions between PRESSED, HELD, RELEASED, and NONE states.
 * Additionally, it detects newly pressed keys not currently tracked and adds them to the key states map.
 */
void zappy::gui::raylib::InputManager::update()
{
    _updateState(_keyStates, IsKeyDown);
    _updateState(_mouseButtonStates, IsMouseButtonDown);

    int key = GetKeyPressed();
    while (key != KEY_NULL) {
        if (_keyStates.find(key) == _keyStates.end())
            _keyStates[key] = KeyState::PRESSED;
        key = GetKeyPressed();
    }

    Vector2 currentMousePos = GetMousePosition();
    _mouseDelta = {
        currentMousePos.x - _lastMousePosition.x,
        currentMousePos.y - _lastMousePosition.y
    };
    _lastMousePosition = currentMousePos;
}

/**
 * @brief Get the state of a key
 *
 * @param key
 * @return zappy::gui::raylib::InputManager::KeyState
 */
zappy::gui::raylib::InputManager::KeyState zappy::gui::raylib::InputManager::getKeyState(int key) const
{
    if (_keyStates.find(key) != _keyStates.end())
        return _keyStates.at(key);
    return KeyState::NONE;
}

bool zappy::gui::raylib::InputManager::isKeyPressed(int key) const
{
    return getKeyState(key) == KeyState::PRESSED;
}

/**
 * @brief Get the state of a mouse button
 *
 * @param button
 * @return zappy::gui::raylib::InputManager::KeyState
 */
zappy::gui::raylib::InputManager::KeyState zappy::gui::raylib::InputManager::getMouseButtonState(int button) const
{
    if (_mouseButtonStates.find(button) != _mouseButtonStates.end())
        return _mouseButtonStates.at(button);
    return KeyState::NONE;
}

/**
 * @brief Check if a mouse button is pressed
 *
 * @param button
 * @return true if the button is pressed else false
 */
bool zappy::gui::raylib::InputManager::isMouseButtonPressed(int button) const
{
    return getMouseButtonState(button) == KeyState::PRESSED;
}

void zappy::gui::raylib::InputManager::_updateState(
    std::unordered_map<int, zappy::gui::raylib::InputManager::KeyState> &states,
    const std::function<bool(int)> &isDownFn
) {
    for (auto &[key, state] : states) {
        if (isDownFn(key)) {
            state = (state == KeyState::PRESSED || state == KeyState::HELD)
                ? KeyState::HELD : KeyState::PRESSED;
        } else {
            state = (state == KeyState::PRESSED || state == KeyState::HELD)
                ? KeyState::RELEASED : KeyState::NONE;
        }
    }
}
