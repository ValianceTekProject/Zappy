/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AScene.cpp
*/

#include "AScene.hpp"

zappy::gui::raylib::AScene::AScene(const std::shared_ptr<game::GameState> &gameState) :
    _camera(Camera()),
    _gameState(gameState),
    _menu(std::make_unique<GameMenu>(gameState)),
    _skybox(),
    _mapRenderer(std::make_unique<MapRenderer>(this->_gameState->getMap()))
{}

void zappy::gui::raylib::AScene::init()
{
    constexpr Vector3 position = { 0, 10.0f, 10.0f };
    constexpr Vector3 target = { 0.0f, 0.0f, 0.0f };
    constexpr Vector3 up = { 0.0f, 0.45f, 0.0f };
    constexpr float fovy = 45.0f;

    this->_camera.position = position;
    this->_camera.target = target;
    this->_camera.up = up;
    this->_camera.fovy = fovy;
    this->_camera.projection = CAMERA_PERSPECTIVE;

    this->_menu->init();
    this->_mapRenderer->init();
}

void zappy::gui::raylib::AScene::setFrequency(const size_t &frequency)
{
    this->_menu->setFrequency(frequency);
}

void zappy::gui::raylib::AScene::update()
{
    this->_menu->update();
    this->_mapRenderer->update(this->_gameState->getFrequency());
    this->_skybox.update();
}

void zappy::gui::raylib::AScene::render() const
{
    BeginMode3D(getCamera());

    this->_skybox.render();
    this->_mapRenderer->render();

    EndMode3D();

    this->_menu->render();
}

void zappy::gui::raylib::AScene::handleInput(InputManager &inputManager)
{
    this->_menu->handleInput(inputManager);
}

void zappy::gui::raylib::AScene::addEgg(const int &id)
{
    // Egg supposed to be added to the map
    game::Egg egg = this->_gameState->getEggById(id);

    this->_mapRenderer->setEggPosition(id, egg.x, egg.y);
}

void zappy::gui::raylib::AScene::addPlayer(const int &id)
{
    game::Player &player = this->_gameState->getPlayerById(id);

    this->_menu->addPlayer(id);

    this->_mapRenderer->setPlayerPosition(id, player.x, player.y, player.orientation);
}

void zappy::gui::raylib::AScene::updatePlayerPosition(const int &id, const int &x, const int &y, const game::Orientation &orientation)
{
    game::Player player = this->_gameState->getPlayerById(id);

    if (player.orientation != orientation) {
        if (orientation == player.orientation - 1)
            this->_mapRenderer->playerLookLeft(player.getId());
        else if (orientation == player.orientation + 1)
            this->_mapRenderer->playerLookRight(player.getId());
        else
            this->_mapRenderer->playerLook(player.getId(), orientation);
    } else {
        // determine if the player go forward
        if (player.x == x && player.y == y)
            return;

        int mapWidth = (this->_gameState->getMap()->getWidth());
        int mapHeight = (this->_gameState->getMap()->getHeight());

        if (player.x == x) {
            if ((y == (player.y - 1 + mapHeight) % mapHeight && player.orientation == game::Orientation::NORTH) ||
                (y == (player.y + 1) % mapHeight && player.orientation == game::Orientation::SOUTH)) {
                    this->_mapRenderer->playerForward(player.getId(), x, y);
            }
        } else if (player.y == y) {
            if ((x == (player.x + 1) % mapWidth && player.orientation == game::Orientation::EAST) ||
                (x == (player.x - 1 + mapWidth) % mapWidth && player.orientation == game::Orientation::WEST)) {
                    this->_mapRenderer->playerForward(player.getId(), x, y);
            }
        } else {
            this->_mapRenderer->setPlayerPosition(player.getId(), x, y, orientation);
        }
    }
}

void zappy::gui::raylib::AScene::updatePlayerLevel(const int &id, const size_t &level)
{
    // Mettre à jour le niveau d'un joueur
    (void)id;
    (void)level;
}

void zappy::gui::raylib::AScene::updatePlayerInventory(const int &id, const game::Inventory &inventory)
{
    // Mettre à jour l'inventaire d'un joueur
    (void)id;
    (void)inventory;
}

void zappy::gui::raylib::AScene::playerExpulsion(const int &id)
{
    game::Player &playerThatExpelled = this->_gameState->getPlayerById(id);
    game::Orientation orientation = playerThatExpelled.orientation;

    auto expelledPlayers = this->_gameState->getPlayersByCoord(
        playerThatExpelled.x, playerThatExpelled.y
    );

    for (auto &player : expelledPlayers) {
        game::Player &p = player.get();
        if (player.get().getId() == id)
            continue;

        int newX = playerThatExpelled.x;
        int newY = playerThatExpelled.y;

        if (orientation == game::Orientation::EAST || orientation == game::Orientation::WEST)
            newX = (newX + (1 * (orientation == game::Orientation::WEST ? -1 : 1)))
                % this->_gameState->getMap()->getWidth();
        if (orientation == game::Orientation::NORTH || orientation == game::Orientation::SOUTH)
            newY = (newY + (1 * (orientation == game::Orientation::SOUTH ? -1 : 1)))
                % this->_gameState->getMap()->getHeight();

        this->_mapRenderer->playerExpulsion(p.getId(), newX, newY);
    }
}

void zappy::gui::raylib::AScene::playerBroadcast(const int &id, const std::string &message)
{
    this->_mapRenderer->playerBroadcast(id);

    std::string playerTeam = this->_gameState->getPlayerById(id).teamName;

    this->_menu->playerBroadcast(id, message, playerTeam);
}

void zappy::gui::raylib::AScene::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    this->_mapRenderer->startIncantation(x, y, playerIds);
    (void)level;
}

void zappy::gui::raylib::AScene::endIncantation(const int &x, const int &y, const bool &result)
{
    this->_mapRenderer->endIncantation(x, y, result);
}

void zappy::gui::raylib::AScene::hatchEgg(const int &id)
{
    (void)id;
}

void zappy::gui::raylib::AScene::removeEgg(const int &id)
{
    this->_mapRenderer->removeEgg(id);
}

void zappy::gui::raylib::AScene::removePlayer(const int &id)
{
    this->_menu->removePlayer(id);
    this->_mapRenderer->removePlayer(id);
}
