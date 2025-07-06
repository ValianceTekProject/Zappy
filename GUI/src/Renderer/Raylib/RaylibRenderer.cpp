/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** RaylibRenderer.cpp
*/

#include "RaylibRenderer.hpp"

/// @brief Constructeur du renderer Raylib.
/// Initialise les pointeurs de menu et la scène par défaut.
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

/// @brief Initialise la fenêtre, les menus et la scène.
/// Lance la fenêtre Raylib en plein écran et configure les éléments graphiques du jeu.
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

    ARenderer::init();
}

/// @brief Met à jour la fréquence du jeu.
/// @param frequency Nouvelle fréquence du serveur (unités de temps).
void zappy::gui::RaylibRenderer::setFrequency(const size_t &frequency)
{
    ARenderer::setFrequency(frequency);
    this->_gameMenu->setFrequency(frequency);
}

/// @brief Gère les entrées clavier de l’utilisateur.
/// Redirige les entrées vers la scène active ou les menus si nécessaire.
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

/// @brief Met à jour la scène, les menus et la caméra.
/// Gère les actions utilisateur, les changements de fréquence et les transitions de scène.
void zappy::gui::RaylibRenderer::update()
{
    ARenderer::update();

    if (!this->_pauseMenu->isActive() && !this->_checkUnwantedInput())
        UpdateCamera(&this->_scene->getCamera(), CAMERA_FREE);

    this->_scene->update();

    this->_gameMenu->update();

    if (this->_gameMenu->hasFrequencyChanged()) {
        auto it = this->_protocolRequests.find(network::GP::TIME_UNIT_MODIFICATION);
        if (it != this->_protocolRequests.end())
            it->second(this->_gameMenu->getFrequency(), 0);
    }

    this->_pauseMenu->update();

    if (this->_pauseMenu->shouldChangeScene())
        this->_setScene(this->_pauseMenu->getSceneType());
}

/// @brief Effectue le rendu complet de la scène actuelle.
/// Dessine la scène, le menu de jeu et le menu pause à l’écran.
void zappy::gui::RaylibRenderer::render() const
{
    BeginDrawing();
    ClearBackground(SKYBLUE);

    this->_scene->render();
    this->_gameMenu->render();
    this->_pauseMenu->render();

    EndDrawing();
}

/// @brief Vérifie si la fenêtre doit être fermée.
/// @return true si l’utilisateur a demandé à fermer la fenêtre.
bool zappy::gui::RaylibRenderer::shouldClose() const
{
    return WindowShouldClose();
}

/// @brief Ajoute un œuf à la scène.
/// @param eggId Identifiant de l’œuf.
/// @param fatherId Identifiant du joueur ayant pondu l’œuf.
/// @param x Position X.
/// @param y Position Y.
void zappy::gui::RaylibRenderer::addEgg(const int &eggId,
    const int &fatherId,
    const int &x,
    const int &y
) {
    ARenderer::addEgg(eggId, fatherId, x, y);
    this->_scene->addEgg(eggId);
}

/// @brief Ajoute un joueur à la scène et au menu de jeu.
/// @param player Référence vers le joueur à ajouter.
void zappy::gui::RaylibRenderer::addPlayer(const game::Player &player)
{
    ARenderer::addPlayer(player);

    const int id = player.getId();

    this->_scene->addPlayer(id);
    this->_gameMenu->addPlayer(id);
}

/// @brief Met à jour la position et l’orientation d’un joueur.
/// @param id Identifiant du joueur.
/// @param x Nouvelle position X.
/// @param y Nouvelle position Y.
/// @param orientation Orientation du joueur.
void zappy::gui::RaylibRenderer::updatePlayerPosition(const int &id,
    const int &x,
    const int &y,
    const game::Orientation &orientation
) {
    this->_scene->updatePlayerPosition(id, x, y, orientation);
    ARenderer::updatePlayerPosition(id, x, y, orientation);
}

/// @brief Met à jour le niveau d’un joueur.
/// @param id Identifiant du joueur.
/// @param level Nouveau niveau.
void zappy::gui::RaylibRenderer::updatePlayerLevel(const int &id, const size_t &level)
{
    ARenderer::updatePlayerLevel(id, level);
}

/// @brief Met à jour l’inventaire d’un joueur.
/// @param id Identifiant du joueur.
/// @param inventory Nouvel inventaire du joueur.
void zappy::gui::RaylibRenderer::updatePlayerInventory(const int &id, const game::Inventory &inventory)
{
    ARenderer::updatePlayerInventory(id, inventory);
}

/// @brief Joue l’animation d’expulsion d’un joueur.
/// @param id Identifiant du joueur expulsé.
void zappy::gui::RaylibRenderer::playerExpulsion(const int &id)
{
    this->_scene->playerExpulsion(id);
    ARenderer::playerExpulsion(id);
}

/// @brief Affiche un message de broadcast d’un joueur.
/// @param id Identifiant du joueur.
/// @param message Message envoyé.
void zappy::gui::RaylibRenderer::playerBroadcast(const int &id, const std::string &message)
{
    ARenderer::playerBroadcast(id, message);
    this->_scene->playerBroadcast(id, message);

    std::string playerTeam = this->_gameState->getPlayerById(id).teamName;

    this->_gameMenu->playerBroadcast(id, message, playerTeam);
}

/// @brief Démarre une incantation.
/// @param x Position X.
/// @param y Position Y.
/// @param level Niveau de l’incantation.
/// @param playerIds Liste des identifiants des joueurs impliqués.
void zappy::gui::RaylibRenderer::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    ARenderer::startIncantation(x, y, level, playerIds);
    this->_scene->startIncantation(x, y, level, playerIds);
}

/// @brief Termine une incantation.
/// @param x Position X.
/// @param y Position Y.
/// @param result Résultat de l’incantation (réussite ou échec).
void zappy::gui::RaylibRenderer::endIncantation(const int &x, const int &y, const bool &result)
{
    ARenderer::endIncantation(x, y, result);
    this->_scene->endIncantation(x, y, result);
}

/// @brief Fait éclore un œuf.
/// @param eggId Identifiant de l’œuf.
void zappy::gui::RaylibRenderer::hatchEgg(const int &eggId)
{
    ARenderer::hatchEgg(eggId);
    this->_scene->hatchEgg(eggId);
}

/// @brief Supprime un œuf de la scène.
/// @param eggId Identifiant de l’œuf.
void zappy::gui::RaylibRenderer::removeEgg(const int &eggId)
{
    ARenderer::removeEgg(eggId);
    this->_scene->removeEgg(eggId);
}

/// @brief Supprime un joueur de la scène et du menu.
/// @param id Identifiant du joueur.
void zappy::gui::RaylibRenderer::removePlayer(const int &id)
{
    ARenderer::removePlayer(id);
    this->_scene->removePlayer(id);
    this->_gameMenu->removePlayer(id);
}

/// @brief Termine la partie et affiche l’équipe gagnante.
/// @param teamName Nom de l’équipe gagnante.
void zappy::gui::RaylibRenderer::endGame(const std::string &teamName)
{
    ARenderer::endGame(teamName);
    this->_scene->endGame(teamName);
}

/// @brief Change la scène active selon le type donné.
/// Réinitialise l’ancienne scène et charge tous les œufs et joueurs depuis l’état du jeu.
/// @param sceneType Type de la nouvelle scène à activer.
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

/// @brief Vérifie si une touche non autorisée est pressée.
/// Empêche certaines touches d’interférer avec la navigation.
/// @return true si une touche non désirée est pressée.
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
