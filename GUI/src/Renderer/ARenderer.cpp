/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** ARenderer.cpp
*/

#include "ARenderer.hpp"

zappy::gui::ARenderer::ARenderer() :
    _gameState(nullptr),
    _elapsedTime(0.0f)
{}

/**
 * @brief Initialise le temps interne du renderer.
 */
void zappy::gui::ARenderer::init()
{
    this->_lastTime = std::chrono::steady_clock::now();
}

/**
 * @brief Définit l'état de jeu à utiliser par le renderer.
 * 
 * @param gameState L'état de jeu partagé.
 */
void zappy::gui::ARenderer::setGameState(std::shared_ptr<game::GameState> &gameState)
{
    this->_gameState = gameState;
}

/**
 * @brief Définit les requêtes du protocole à utiliser.
 * 
 * @param protocolRequests Map de fonctions associées aux requêtes réseau.
 */
void zappy::gui::ARenderer::setProtocolRequests(const ProtocolRequest &protocolRequests)
{
    this->_protocolRequests = protocolRequests;
}

/**
 * @brief Définit la fréquence de mise à jour du jeu.
 * 
 * @param frequency La fréquence en unités de temps.
 */
void zappy::gui::ARenderer::setFrequency(const size_t &frequency)
{
    _checkGameState();
    this->_gameState->setFrequency(frequency);
}

/**
 * @brief Met à jour le renderer et déclenche des actions régulières comme les requêtes de contenu de carte.
 */
void zappy::gui::ARenderer::update()
{
    if (!_gameState)
        return;

    auto now = std::chrono::steady_clock::now();
    float deltaSec = std::chrono::duration<float>(now - this->_lastTime).count();
    this->_lastTime = now;

    float deltaUnits = deltaSec * static_cast<float>(this->_gameState->getFrequency());
    this->_elapsedTime += deltaUnits;

    if (_elapsedTime >= RequestMapContentTimeUnit) {
        auto it = this->_protocolRequests.find(network::GP::MAP_CONTENT);
        if (it != this->_protocolRequests.end())
            it->second(0, 0);
        this->_elapsedTime = 0.0f;
    }
}

/**
 * @brief Ajoute un œuf à l'état de jeu.
 * 
 * @param eggId Identifiant de l'œuf.
 * @param fatherId Identifiant du joueur père.
 * @param x Coordonnée X de l'œuf.
 * @param y Coordonnée Y de l'œuf.
 */
void zappy::gui::ARenderer::addEgg(const int &eggId, const int &fatherId, const int &x, const int &y)
{
    _checkGameState();
    this->_gameState->addEgg(eggId, fatherId, x, y);
}

/**
 * @brief Ajoute un joueur à l'état de jeu.
 * 
 * @param player Référence au joueur à ajouter.
 */
void zappy::gui::ARenderer::addPlayer(const game::Player &player)
{
    _checkGameState();
    this->_gameState->addPlayer(player);
}

/**
 * @brief Met à jour la position et l’orientation d’un joueur.
 * 
 * @param id Identifiant du joueur.
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @param orientation Orientation du joueur.
 */
void zappy::gui::ARenderer::updatePlayerPosition(
    const int &id,
    const int &x,
    const int &y,
    const game::Orientation &orientation
) {
    _checkGameState();
    this->_gameState->updatePlayerPosition(id, x, y, orientation);
}

/**
 * @brief Met à jour le niveau d’un joueur.
 * 
 * @param id Identifiant du joueur.
 * @param level Nouveau niveau.
 */
void zappy::gui::ARenderer::updatePlayerLevel(const int &id, const size_t &level)
{
    _checkGameState();
    this->_gameState->updatePlayerLevel(id, level);
}

/**
 * @brief Met à jour l’inventaire d’un joueur.
 * 
 * @param id Identifiant du joueur.
 * @param inventory Inventaire à appliquer.
 */
void zappy::gui::ARenderer::updatePlayerInventory(const int &id, const game::Inventory &inventory)
{
    _checkGameState();
    this->_gameState->updatePlayerInventory(id, inventory);
}

/**
 * @brief Notifie l’expulsion d’un joueur.
 * 
 * @param id Identifiant du joueur expulsé.
 */
void zappy::gui::ARenderer::playerExpulsion(const int &id)
{
    _checkGameState();
    this->_gameState->playerExpulsion(id);
}

/**
 * @brief Notifie un message de broadcast émis par un joueur.
 * 
 * @param id Identifiant du joueur.
 * @param message Message diffusé.
 */
void zappy::gui::ARenderer::playerBroadcast(const int &id, const std::string &message)
{
    _checkGameState();
    this->_gameState->playerBroadcast(id, message);
}

/**
 * @brief Démarre une incantation.
 * 
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @param level Niveau de l’incantation.
 * @param playerIds Liste des identifiants des joueurs impliqués.
 */
void zappy::gui::ARenderer::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    _checkGameState();
    this->_gameState->startIncantation(x, y, level, playerIds);
}

/**
 * @brief Termine une incantation.
 * 
 * @param x Coordonnée X.
 * @param y Coordonnée Y.
 * @param result Résultat de l’incantation (succès ou échec).
 */
void zappy::gui::ARenderer::endIncantation(const int &x, const int &y, const bool &result)
{
    _checkGameState();
    this->_gameState->endIncantation(x, y, result);
}

/**
 * @brief Fait éclore un œuf.
 * 
 * @param eggId Identifiant de l'œuf.
 */
void zappy::gui::ARenderer::hatchEgg(const int &eggId)
{
    _checkGameState();
    this->_gameState->hatchEgg(eggId);
}

/**
 * @brief Supprime un œuf de l’état de jeu.
 * 
 * @param eggId Identifiant de l'œuf.
 */
void zappy::gui::ARenderer::removeEgg(const int &eggId)
{
    _checkGameState();
    this->_gameState->removeEgg(eggId);
}

/**
 * @brief Supprime un joueur de l’état de jeu.
 * 
 * @param id Identifiant du joueur.
 */
void zappy::gui::ARenderer::removePlayer(const int &id)
{
    _checkGameState();
    this->_gameState->removePlayer(id);
}

/**
 * @brief Termine la partie et affiche le nom de l’équipe gagnante.
 * 
 * @param teamName Nom de l’équipe gagnante.
 */
void zappy::gui::ARenderer::endGame(const std::string &teamName)
{
    _checkGameState();
    this->_gameState->endGame(teamName);
}

/**
 * @brief Vérifie que l'état de jeu est bien défini.
 * 
 * @throw GuiError si l’état de jeu n’est pas défini.
 */
void zappy::gui::ARenderer::_checkGameState() const
{
    if (!this->_gameState)
        throw GuiError("Game state is not set", "ARenderer");
}
