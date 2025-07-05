/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RaylibRenderer.cpp
*/

#include "RaylibRenderer.hpp"

zappy::gui::RaylibRenderer::RaylibRenderer() :
    ARenderer::ARenderer(),
    _sceneType(raylib::SceneType::BASIC),
    _scene(nullptr),
    _gameMenu(nullptr),
    _pauseMenu(nullptr),
    _inputManager()
{
    SetTraceLogLevel(LOG_NONE);
}

void zappy::gui::RaylibRenderer::init()
{
    static constexpr const char *title = "Zappy";

    InitWindow(1280, 720, title);
    SetExitKey(KEY_NULL);
    ToggleFullscreen();
    SetTargetFPS(60);
    DisableCursor();

    this->_pauseMenu = std::make_unique<raylib::PauseMenu>();
    this->_pauseMenu->init();

    this->_setScene(_pauseMenu->getSceneType());

    this->_gameMenu = std::make_unique<raylib::GameMenu>(this->_gameState);
    this->_gameMenu->init();

    // game::Orientation orientation = game::Orientation::SOUTH;
    // game::Player player(0, 2, 6, orientation);
    // this->addPlayer(player);

    // this->updatePlayerPosition(0, 2, 6, game::Orientation::WEST);
    // this->updatePlayerPosition(0, 2, 6, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 5, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 4, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 3, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 2, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 1, game::Orientation::NORTH);
    // this->updatePlayerPosition(0, 2, 0, game::Orientation::NORTH);

    // this->startIncantation(2, 2, 1, {0});
    // this->endIncantation(2, 2, true);

    ARenderer::init();
}

void zappy::gui::RaylibRenderer::setFrequency(const size_t &frequency)
{
    ARenderer::setFrequency(frequency);
    this->_gameMenu->setFrequency(frequency);
}

void zappy::gui::RaylibRenderer::handleInput()
{
    this->_inputManager.update();

    if (this->_inputManager.isKeyPressed(KEY_BACKSPACE)) {
        CloseWindow();
        return;
    }

    this->_scene->handleInput(this->_inputManager);

    this->_pauseMenu->handleInput(this->_inputManager);

    if (!this->_pauseMenu->isActive())
        this->_gameMenu->handleInput(this->_inputManager);
}

void zappy::gui::RaylibRenderer::update()
{
    ARenderer::update();

    if (!this->_pauseMenu->isActive() && !this->_checkUnwantedInput())
        UpdateCamera(&this->_scene->getCamera(), CAMERA_FREE);

    this->_scene->update();

    this->_gameMenu->update();

    this->_pauseMenu->update();

    if (this->_pauseMenu->shouldChangeScene())
        this->_setScene(this->_pauseMenu->getSceneType());
}

void zappy::gui::RaylibRenderer::render() const
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

    this->_scene->render();

    this->_gameMenu->render();

    this->_pauseMenu->render();

    EndDrawing();
}

bool zappy::gui::RaylibRenderer::shouldClose() const
{
    return WindowShouldClose();
}

void zappy::gui::RaylibRenderer::addEgg(const int &eggId,
    const int &fatherId,
    const int &x,
    const int &y
) {
    ARenderer::addEgg(eggId, fatherId, x, y);
    this->_scene->addEgg(eggId);
}

void zappy::gui::RaylibRenderer::addPlayer(const game::Player &player)
{
    ARenderer::addPlayer(player);

    const int id = player.getId();

    this->_scene->addPlayer(id);
    this->_gameMenu->addPlayer(id);
}

void zappy::gui::RaylibRenderer::updatePlayerPosition(const int &id,
    const int &x,
    const int &y,
    const game::Orientation &orientation
) {
    this->_scene->updatePlayerPosition(id, x, y, orientation);
    ARenderer::updatePlayerPosition(id, x, y, orientation);
}

void zappy::gui::RaylibRenderer::updatePlayerLevel(const int &id, const size_t &level)
{
    ARenderer::updatePlayerLevel(id, level);
}

void zappy::gui::RaylibRenderer::updatePlayerInventory(const int &id, const game::Inventory &inventory)
{
    ARenderer::updatePlayerInventory(id, inventory);
}

void zappy::gui::RaylibRenderer::playerExpulsion(const int &id)
{
    this->_scene->playerExpulsion(id);
    ARenderer::playerExpulsion(id);
}

void zappy::gui::RaylibRenderer::playerBroadcast(const int &id, const std::string &message)
{
    ARenderer::playerBroadcast(id, message);

    this->_scene->playerBroadcast(id, message);

    std::string playerTeam = this->_gameState->getPlayerById(id).teamName;

    this->_gameMenu->playerBroadcast(id, message, playerTeam);
}

void zappy::gui::RaylibRenderer::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    ARenderer::startIncantation(x, y, level, playerIds);
    this->_scene->startIncantation(x, y, level, playerIds);
}

void zappy::gui::RaylibRenderer::endIncantation(const int &x, const int &y, const bool &result)
{
    ARenderer::endIncantation(x, y, result);
    this->_scene->endIncantation(x, y, result);
}

void zappy::gui::RaylibRenderer::hatchEgg(const int &eggId)
{
    ARenderer::hatchEgg(eggId);
    this->_scene->hatchEgg(eggId);
}

void zappy::gui::RaylibRenderer::removeEgg(const int &eggId)
{
    ARenderer::removeEgg(eggId);
    this->_scene->removeEgg(eggId);
}

void zappy::gui::RaylibRenderer::removePlayer(const int &id)
{
    ARenderer::removePlayer(id);

    this->_scene->removePlayer(id);
    this->_gameMenu->removePlayer(id);
}

void zappy::gui::RaylibRenderer::endGame(const std::string &teamName)
{
    ARenderer::endGame(teamName);
    this->_scene->endGame(teamName);
}

void zappy::gui::RaylibRenderer::_setScene(const raylib::SceneType &sceneType)
{
    if (this->_scene)
        this->_scene.reset();

    this->_sceneType = sceneType;
    this->_scene = _scenesConstructors.at(sceneType)(this->_gameState);
    this->_scene->init();

    const auto &eggs = this->_gameState->getEggs();

    for (const auto &egg : eggs)
        this->_scene->addEgg(egg.getId());

    const auto &players = this->_gameState->getPlayers();

    for (const auto &player : players)
        this->_scene->addPlayer(player.getId());
}

bool zappy::gui::RaylibRenderer::_checkUnwantedInput() const
{
    const std::vector<int> unwantedKeys = {
        KEY_Q, KEY_E, KEY_UP, KEY_DOWN, KEY_LEFT, KEY_RIGHT
    };

    for (const auto &key : unwantedKeys) {
        raylib::InputManager::KeyState keyState = this->_inputManager.getKeyState(key);
        if (keyState == raylib::InputManager::KeyState::PRESSED ||
            keyState == raylib::InputManager::KeyState::HELD)
            return true;
    }
    return false;
}
