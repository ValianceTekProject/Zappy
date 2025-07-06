/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AScene.cpp
*/

#include "AScene.hpp"

zappy::gui::raylib::AScene::AScene(const std::shared_ptr<game::GameState> &gameState) :
    _camera(Camera()),
    _music(),
    _isMusicPlaying(true),
    _gameState(gameState),
    _skybox(),
    _mapRenderer(std::make_unique<MapRenderer>(this->_gameState->getMap()))
{}

/** @brief Initialise la scène.
 *
 * Configure la caméra en fonction de la taille de la carte
 * et initialise l'audio.
 */
void zappy::gui::raylib::AScene::init()
{
    constexpr Vector3 up = { 0.0f, 1.0f, 0.0f };
    constexpr float fovy = 60.0f;

    const float mapWidth = this->_gameState->getMap()->getWidth();
    const float mapHeight = this->_gameState->getMap()->getHeight();

    const float centerX = -mapWidth / 2.0f;
    const float centerZ = mapHeight / 2.0f;

    const float camHeight = std::max(centerX, centerZ);

    const float targetX = centerX / 2.5f;
    const float targetZ = centerZ / 2.5f;

    this->_camera.position = { centerX * 1.5f, camHeight, centerZ * 1.5f };
    this->_camera.target = { targetX, 0.0f, targetZ };
    this->_camera.up = up;
    this->_camera.fovy = fovy;
    this->_camera.projection = CAMERA_PERSPECTIVE;

    // Init audio
    InitAudioDevice();
    SetMasterVolume(0.5f);
}

/** @brief Gère les entrées utilisateur.
 *
 * @param inputManager Le gestionnaire d'entrée.
 */
void zappy::gui::raylib::AScene::handleInput(InputManager &inputManager)
{
    if (inputManager.isKeyPressed(KEY_M))
        this->_isMusicPlaying = !this->_isMusicPlaying;
}

/** @brief Met à jour les éléments de la scène.
 *
 * Met à jour le rendu de la carte, le ciel et la musique.
 */
void zappy::gui::raylib::AScene::update()
{
    this->_mapRenderer->update(this->_gameState->getFrequency());
    this->_skybox.update();

    if (this->_isMusicPlaying)
        this->_music.update();
}

/** @brief Dessine la scène à l'écran.
 *
 * Affiche la musique, le ciel et la carte en mode 3D.
 */
void zappy::gui::raylib::AScene::render() const
{
    if (this->_isMusicPlaying)
        this->_music.render();

    BeginMode3D(getCamera());

    this->_skybox.render();
    this->_mapRenderer->render();

    EndMode3D();
}

/** @brief Ajoute un œuf à la scène.
 *
 * @param id L'identifiant de l'œuf à ajouter.
 */
void zappy::gui::raylib::AScene::addEgg(const int &id)
{
    // Egg supposed to be added to the map
    game::Egg egg = this->_gameState->getEggById(id);

    this->_mapRenderer->setEggPosition(id, egg.x, egg.y);
}

/** @brief Ajoute un joueur à la scène.
 *
 * @param id L'identifiant du joueur à ajouter.
 */
void zappy::gui::raylib::AScene::addPlayer(const int &id)
{
    game::Player &player = this->_gameState->getPlayerById(id);

    this->_mapRenderer->setPlayerPosition(id, player.x, player.y, player.orientation);
}

/** @brief Met à jour la position et l’orientation d’un joueur.
 *
 * @param id L’identifiant du joueur.
 * @param x La nouvelle position en x.
 * @param y La nouvelle position en y.
 * @param orientation La nouvelle orientation du joueur.
 */
void zappy::gui::raylib::AScene::updatePlayerPosition(const int &id, const int &x, const int &y, const game::Orientation &orientation)
{
    game::Player player = this->_gameState->getPlayerById(id);

    if (player.orientation != orientation) {
        if (player.orientation == orientation + 1)
            this->_mapRenderer->playerLookLeft(player.getId(), orientation);
        else if (player.orientation == orientation - 1)
            this->_mapRenderer->playerLookRight(player.getId(), orientation);
        else {
            this->_mapRenderer->playerLook(player.getId(), orientation);
            this->_mapRenderer->removeAllRotations(player.getId());
        }
    }

    // determine if the player go forward
    if (player.x == x && player.y == y)
        return;

    const int mapWidth = this->_gameState->getMap()->getWidth();
    const int mapHeight = this->_gameState->getMap()->getHeight();

    if (player.x == x) {
        if ((y == (player.y - 1 + mapHeight) % mapHeight && player.orientation == game::Orientation::NORTH) ||
            (y == (player.y + 1) % mapHeight && player.orientation == game::Orientation::SOUTH)) {
                this->_mapRenderer->playerForward(player.getId(), x, y, orientation, mapWidth, mapHeight);
        } else  {
            this->_mapRenderer->setPlayerPosition(player.getId(), x, y, orientation);
            this->_mapRenderer->removeAllTranslations(player.getId());
        }
    } else if (player.y == y) {
        if ((x == (player.x + 1) % mapWidth && player.orientation == game::Orientation::EAST) ||
            (x == (player.x - 1 + mapWidth) % mapWidth && player.orientation == game::Orientation::WEST)) {
                this->_mapRenderer->playerForward(player.getId(), x, y, orientation, mapWidth, mapHeight);
        } else {
            this->_mapRenderer->setPlayerPosition(player.getId(), x, y, orientation);
            this->_mapRenderer->removeAllTranslations(player.getId());
        }
    } else {
        this->_mapRenderer->setPlayerPosition(player.getId(), x, y, orientation);
        this->_mapRenderer->removeAllTranslations(player.getId());
    }
}

/** @brief Déclenche l’expulsion des autres joueurs autour d’un joueur.
 *
 * @param id L’identifiant du joueur qui expulse.
 */
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

/** @brief Déclenche une animation de broadcast pour un joueur.
 *
 * @param id L’identifiant du joueur.
 * @param message Le message broadcasté (non utilisé ici).
 */
void zappy::gui::raylib::AScene::playerBroadcast(const int &id, const std::string &message)
{
    this->_mapRenderer->playerBroadcast(id);
    (void)message;
}

/** @brief Lance une incantation sur une case.
 *
 * @param x Coordonnée x de la case.
 * @param y Coordonnée y de la case.
 * @param level Le niveau de l'incantation.
 * @param playerIds Liste des identifiants des joueurs impliqués.
 */
void zappy::gui::raylib::AScene::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    this->_mapRenderer->startIncantation(x, y, playerIds);
    (void)level;
}

/** @brief Termine une incantation.
 *
 * @param x Coordonnée x de la case.
 * @param y Coordonnée y de la case.
 * @param result Résultat de l'incantation (réussie ou échouée).
 */
void zappy::gui::raylib::AScene::endIncantation(const int &x, const int &y, const bool &result)
{
    this->_mapRenderer->endIncantation(x, y, result);
}


/** @brief Fait éclore un œuf.
 *
 * @param id L'identifiant de l'œuf.
 */
void zappy::gui::raylib::AScene::hatchEgg(const int &id)
{
    this->removeEgg(id);
}

/** @brief Supprime un œuf de la scène.
 *
 * @param id L'identifiant de l'œuf à retirer.
 */
void zappy::gui::raylib::AScene::removeEgg(const int &id)
{
    this->_mapRenderer->removeEgg(id);
}


/** @brief Supprime un joueur de la scène.
 *
 * @param id L'identifiant du joueur à retirer.
 */
void zappy::gui::raylib::AScene::removePlayer(const int &id)
{
    this->_mapRenderer->removePlayer(id);
}
