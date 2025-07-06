/*
** EPITECH PROJECT, 2024
** zappyGood
** File description:
** MapRenderer.cpp
*/

#include "MapRenderer.hpp"
#include "PlayerActions/PlayerIncantation.hpp"
#include "PokemonEggModel.hpp"
#include <memory>
#include <raylib.h>

zappy::gui::raylib::MapRenderer::MapRenderer(const std::shared_ptr<game::Map> map) :
    _map(map),
    _floor(nullptr),
    _broadcastType(EffectType::WAVE_BROADCAST),
    _broadcastColor(BLUE),
    _incantationType(EffectType::SPIRAL_INCANTATION),
    _incantationColor(BLUE),
    _eggs(),
    _players()
{}

/**
 * @brief Initialise le renderer avec une texture de sol.
 * @param tileTexturePath Chemin vers la texture des tuiles (par défaut : BASIC_FLOOR_PATH).
 */
void zappy::gui::raylib::MapRenderer::init(const std::string &tileTexturePath)
{
    // Init la carte
    this->_floor = std::make_shared<FlatFloor>(_map->getWidth(), _map->getHeight(), tileTexturePath, 1);
    this->_floor->init();

    this->_lastTime = std::chrono::steady_clock::now();
}

/**
 * @brief Met à jour l'état du renderer en fonction de la fréquence donnée.
 * @param frequency Fréquence de mise à jour.
 */
void zappy::gui::raylib::MapRenderer::update(const int &frequency)
{
    // Mettre à jour la carte
    this->_floor->update();

    auto now = std::chrono::steady_clock::now();
    float deltaSec = std::chrono::duration<float>(now - _lastTime).count();
    _lastTime = now;

    // Convertit en “unités d’action” : (secondes écoulées) * fréquence
    float deltaUnits = deltaSec * frequency;

    _updatePlayersAndEggs(deltaUnits);
    _updateActions(deltaUnits);
}

/**
 * @brief Rend la scène (joueurs, œufs, ressources, effets, etc.).
 */
void zappy::gui::raylib::MapRenderer::render()
{
    // Dessiner la carte
    this->_floor->render();

    _renderPlayersAndEggs();

    _renderResources();

    _renderAnimActions();
}

/**
 * @brief Définit le type d'effet de diffusion (broadcast).
 * @param type Type d'effet à appliquer.
 */
void zappy::gui::raylib::MapRenderer::setBroadcastType(const zappy::gui::raylib::EffectType &type)
{
    this->_broadcastType = type;
}

/**
 * @brief Définit la couleur de l'effet de diffusion (broadcast).
 * @param color Couleur de l'effet.
 */
void zappy::gui::raylib::MapRenderer::setIncantationType(const zappy::gui::raylib::EffectType &type)
{
    this->_incantationType = type;
}

/**
 * @brief Définit le type d'effet d'incantation.
 * @param type Type d'effet d'incantation.
 */
void zappy::gui::raylib::MapRenderer::setIncantationColor(const Color &color)
{
    this->_incantationColor = color;
}

/**
 * @brief Définit la couleur de l'effet d'incantation.
 * @param color Couleur de l'effet.
 */
void zappy::gui::raylib::MapRenderer::addEgg(std::unique_ptr<AEggModel> egg)
{
    egg->init();

    _eggs.push_back(std::move(egg));
}

/**
 * @brief Ajoute un œuf dans la scène.
 * @param egg Unique pointer vers le modèle d'œuf à ajouter.
 */
void zappy::gui::raylib::MapRenderer::addPlayer(std::unique_ptr<APlayerModel> player)
{
    player->init();

    this->_players.push_back(std::move(player));
}

/**
 * @brief Ajoute un joueur dans la scène.
 * @param player Unique pointer vers le modèle de joueur à ajouter.
 */
void zappy::gui::raylib::MapRenderer::addResourceModel(const zappy::game::Resource &type, std::unique_ptr<AResourceModel> model)
{
    if (model)
        model->init();
    _resourceModels[static_cast<size_t>(type)] = std::move(model);
}

/**
 * @brief Ajoute un modèle de ressource.
 * @param type Type de ressource.
 * @param model Unique pointer vers le modèle de ressource à ajouter.
 */
void zappy::gui::raylib::MapRenderer::setEggPosition(const int &id, const int &x, const int &y)
{
    if (_eggs.empty())
        return;

    constexpr float pokemonEggPosY = 0.15;
    auto &egg = this->_getEgg(id);
    PokemonEggModel *pokemonEgg = dynamic_cast<PokemonEggModel *>(&egg);
    Vector3 position3D = this->_floor->get3DCoords(x, y);

    if (pokemonEgg) {
        position3D.y = pokemonEggPosY;
    }

    egg.setPosition(position3D);
}

/**
 * @brief Positionne un œuf sur la carte.
 * @param id Identifiant de l'œuf.
 * @param x Position x.
 * @param y Position y.
 */
void zappy::gui::raylib::MapRenderer::setPlayerPosition(const int &id, const int &x, const int &y, const game::Orientation &orientation)
{
    if (this->_players.empty())
        return;

    auto &player = this->_getPlayer(id);
    Vector3 postion3D = this->_floor->get3DCoords(x, y);

    player.setPosition(postion3D);

    player.setGamePosition(Vector2{
        static_cast<float>(x),
        static_cast<float>(y)
    });
    player.look(orientation);
}

/**
 * @brief Positionne un joueur sur la carte avec une orientation donnée.
 * @param id Identifiant du joueur.
 * @param x Position x.
 * @param y Position y.
 * @param orientation Orientation du joueur.
 */
void zappy::gui::raylib::MapRenderer::playerLook(const int &id, const game::Orientation &orientation)
{
    if (this->_players.empty())
        return;

    auto &player = this->_getPlayer(id);

    player.look(orientation);
}

/**
 * @brief Change la direction du regard du joueur.
 * @param id Identifiant du joueur.
 * @param orientation Nouvelle orientation.
 */
void zappy::gui::raylib::MapRenderer::playerLookLeft(const int &id, const game::Orientation &orientation)
{
    if (this->_players.empty())
        return;

    auto &player = _getPlayer(id);

    if (orientation != player.getOrientation() - 1) {
        player.look(orientation);
        this->removeAllRotations(id);
        return;
    }

    player.lookLeft();

    constexpr float rotationAngle = 90.0f;

    _addRotation(player, rotationAngle, player.getOrientation());
}

/**
 * @brief Fait regarder le joueur vers la gauche.
 * @param id Identifiant du joueur.
 * @param orientation Orientation vers laquelle le joueur regarde.
 */

/**
 * @brief Fait regarder le joueur vers la droite.
 * @param id Identifiant du joueur.
 * @param orientation Orientation vers laquelle le joueur regarde.
 */
void zappy::gui::raylib::MapRenderer::playerLookRight(const int &id, const game::Orientation &orientation)
{
    if (this->_players.empty())
        return;

    auto &player = _getPlayer(id);

    if (orientation != player.getOrientation() + 1) {
        player.look(orientation);
        this->removeAllRotations(id);
        return;
    }

    player.lookRight();

    constexpr float rotationAngle = -90.0f;

    _addRotation(player, rotationAngle, player.getOrientation());
}

/**
 * @brief Déplace le joueur vers l'avant selon sa position et orientation, en tenant compte des dimensions de la carte.
 * @param id Identifiant du joueur.
 * @param x Position cible x.
 * @param y Position cible y.
 * @param orientation Orientation du joueur.
 * @param mapWidth Largeur de la carte.
 * @param mapHeight Hauteur de la carte.
 */
void zappy::gui::raylib::MapRenderer::playerForward(
    const int &id,
    const int &x,
    const int &y,
    const game::Orientation &orientation,
    const int &mapWidth,
    const int &mapHeight
) {
    if (this->_players.empty())
        return;

    APlayerModel &player = _getPlayer(id);

    const Vector2 gamePos = player.getGamePosition();
    const game::Orientation playerOrientation = player.getOrientation();

    bool valid = false;
    if (playerOrientation != orientation)
        valid = true;
    else if (playerOrientation == game::Orientation::NORTH && gamePos.y != (y + 1) % mapHeight)
        valid = true;
    else if (playerOrientation == game::Orientation::SOUTH && gamePos.y != (y - 1 + mapHeight) % mapHeight)
        valid = true;
    else if (playerOrientation == game::Orientation::WEST && gamePos.x != (x + 1) % mapWidth)
        valid = true;
    else if (playerOrientation == game::Orientation::EAST && gamePos.x != (x - 1 + mapWidth) % mapWidth)
        valid = true;

    if (valid) {
        this->removeAllTranslations(id);
        this->removeAllRotations(id);
        this->setPlayerPosition(id, x, y, orientation);
        return;
    }

    Translation translation = this->_floor->createTranslation(player, x, y, FORWARD_TIME);

    std::shared_ptr<IPlayerAction> action = PlayerActionFactory::createTranslation(
        id,
        translation,
        this->_floor,
        FORWARD_TIME
    );

    this->_playerActionQueues[id].push(std::move(action));

    player.setGamePosition(Vector2{ static_cast<float>(x), static_cast<float>(y) });
}

/**
 * @brief Gère l'expulsion d'un joueur à une position donnée.
 * @param id Identifiant du joueur expulsé.
 * @param x Position x.
 * @param y Position y.
 */
void zappy::gui::raylib::MapRenderer::playerExpulsion(const int &id, const int &x, const int &y)
{
    if (this->_players.empty())
        return;

    APlayerModel &player = _getPlayer(id);
    Translation translation = this->_floor->createTranslation(player, x, y, EXPULSION_TIME);

    std::shared_ptr<IPlayerAction> action = PlayerActionFactory::createTranslation(
        id,
        translation,
        this->_floor,
        EXPULSION_TIME
    );

    this->_playerActionQueues[id].push(std::move(action));

    player.setGamePosition(Vector2{ static_cast<float>(x), static_cast<float>(y) });
}

/**
 * @brief Déclenche la diffusion d'un message par un joueur.
 * @param id Identifiant du joueur qui diffuse.
 */
void zappy::gui::raylib::MapRenderer::playerBroadcast(const int &id)
{
    std::shared_ptr<IPlayerAction> action = PlayerActionFactory::createBroadcast(
        id,
        this->_broadcastType,
        this->_broadcastColor,
        BROADCAST_TIME
    );

    this->_playerActionQueues[id].push(action);
    this->_playerAnimAction.push_back(std::move(std::dynamic_pointer_cast<APlayerAnimAction>(action)));
}

/**
 * @brief Démarre une incantation sur une case donnée avec les joueurs concernés.
 * @param x Position x de l'incantation.
 * @param y Position y de l'incantation.
 * @param playerIds Identifiants des joueurs participants.
 */
void zappy::gui::raylib::MapRenderer::startIncantation(const int &x, const int &y, const std::vector<int> &playerIds)
{
    Vector2 incantationPos = Vector2{static_cast<float>(x), static_cast<float>(y)};

    std::shared_ptr<IPlayerAction> action = PlayerActionFactory::createIncantation(
        playerIds[0],
        this->_incantationType,
        this->_incantationColor,
        incantationPos,
        INCANTATION_TIME
    );

    for (auto id : playerIds)
        this->_playerActionQueues[id].push(action);

    this->_playerAnimAction.push_back(std::move(std::dynamic_pointer_cast<APlayerAnimAction>(action)));

    APlayerAnimAction &animAction = static_cast<APlayerAnimAction &>(*action);
    this->_incantationMap[animAction.getAnimationId()] = incantationPos;
}

/**
 * @brief Termine une incantation sur une case donnée avec le résultat.
 * @param x Position x de l'incantation.
 * @param y Position y de l'incantation.
 * @param result Résultat de l'incantation (true = succès, false = échec).
 */
void zappy::gui::raylib::MapRenderer::endIncantation(const int &x, const int &y, const bool &result)
{
    Vector2 targetPos = Vector2{static_cast<float>(x), static_cast<float>(y)};
    ssize_t animationIdToRemove = -1;
    (void)result;

    for (const auto &[animationId, pos] : _incantationMap) {
        if (pos.x == targetPos.x && pos.y == targetPos.y) {
            animationIdToRemove = animationId;
            break;
        }
    }

    if (animationIdToRemove == -1)
        return;

    std::shared_ptr<PlayerIncantation> incantation = nullptr;

    for (auto &action : _playerAnimAction) {
        if (action && action->getAnimationId() == animationIdToRemove) {
            // Recup shared ptr de playerIncantation
            incantation = std::dynamic_pointer_cast<PlayerIncantation>(action);
            break;
        }
    }

    // Supprimer l'action de la queue des joueurs
    int playerId = incantation->getPlayerId();

    for (auto &[id, queue] : _playerActionQueues)
        if (!queue.empty() && queue.front()->getPlayerId() == playerId &&
        queue.front()->getActionType() == ActionType::INCANTATION)
            queue.pop();

    // Appeller IncantationResult();
    incantation->incantationResult(result);

    _incantationMap.erase(animationIdToRemove);

    if (!incantation->hasActionStarted())
        incantation->startAction();
}

/**
 * @brief Supprime un joueur de la scène.
 * @param id Identifiant du joueur à supprimer.
 */
void zappy::gui::raylib::MapRenderer::removeEgg(const int &id)
{
    for (auto it = _eggs.begin(); it != _eggs.end(); it++) {
        if ((*it)->getId() == id) {
            _eggs.erase(it);
            break;
        }
    }
}

/**
 * @brief Supprime un œuf de la scène.
 * @param id Identifiant de l'œuf à supprimer.
 */
void zappy::gui::raylib::MapRenderer::removePlayer(const int &id)
{
    for (auto it = this->_players.begin(); it != this->_players.end(); it++) {
        if ((*it)->getId() == id) {
            this->_players.erase(it);
            this->removeAllActions(id);
            break;
        }
    }
}

void zappy::gui::raylib::MapRenderer::removeAllActions(const int &id)
{
    this->_playerActionQueues.erase(id);

    this->_playerAnimAction.erase(
        std::remove_if(
            this->_playerAnimAction.begin(),
            this->_playerAnimAction.end(),
            [id](const std::shared_ptr<APlayerAnimAction> &action) {
                return action->getPlayerId() == id;
            }
        ),
        this->_playerAnimAction.end()
    );
}

/**
 * @brief Supprime toutes les actions de translation pour un joueur donné.
 * @param id Identifiant du joueur.
 */
void zappy::gui::raylib::MapRenderer::removeAllTranslations(const int &id)
{
    std::queue<std::shared_ptr<IPlayerAction>> &queue = this->_playerActionQueues[id];
    std::queue<std::shared_ptr<IPlayerAction>> newQueue;

    while (!queue.empty()) {
        auto action = queue.front();
        queue.pop();
        if (action->getActionType() != ActionType::FORWARD &&
            action->getActionType() != ActionType::EXPULSION) {
            newQueue.push(action);
        }
    }
    queue = std::move(newQueue);
}

/**
 * @brief Supprime toutes les actions de rotation pour un joueur donné.
 * @param id Identifiant du joueur.
 */
void zappy::gui::raylib::MapRenderer::removeAllRotations(const int &id)
{
    std::queue<std::shared_ptr<IPlayerAction>> &queue = this->_playerActionQueues[id];
    std::queue<std::shared_ptr<IPlayerAction>> newQueue;

    while (!queue.empty()) {
        auto action = queue.front();
        queue.pop();
        if (action->getActionType() != ActionType::ROTATION) {
            newQueue.push(action);
        }
    }
    queue = std::move(newQueue);
}

/**
 * @brief Récupère une référence non-const au modèle joueur correspondant à l'identifiant.
 * @param id Identifiant du joueur.
 * @return Référence au modèle joueur.
 */
zappy::gui::raylib::APlayerModel &zappy::gui::raylib::MapRenderer::_getPlayer(const int &id)
{
    for (auto &player : this->_players) {
        if (player->getId() == id)
            return *player;
    }
    throw RendererError("Player " + std::to_string(id) + " not found", "MapRenderer");
}

/**
 * @brief Récupère une référence const au modèle joueur correspondant à l'identifiant.
 * @param id Identifiant du joueur.
 * @return Référence const au modèle joueur.
 */
const zappy::gui::raylib::APlayerModel &zappy::gui::raylib::MapRenderer::_getPlayer(const int &id) const
{
    for (const auto &player : this->_players) {
        if (player->getId() == id)
            return *player;
    }
    throw RendererError("Player " + std::to_string(id) + " not found", "MapRenderer");
}

/**
 * @brief Récupère une référence non-const au modèle œuf correspondant à l'identifiant.
 * @param id Identifiant de l'œuf.
 * @return Référence au modèle œuf.
 */
zappy::gui::raylib::AEggModel &zappy::gui::raylib::MapRenderer::_getEgg(const int &id)
{
    for (auto &egg : _eggs) {
        if (egg->getId() == id)
            return *egg;
    }
    throw RendererError("Egg " + std::to_string(id) + " not found", "MapRenderer");
}

/**
 * @brief Récupère une référence const au modèle œuf correspondant à l'identifiant.
 * @param id Identifiant de l'œuf.
 * @return Référence const au modèle œuf.
 */
const zappy::gui::raylib::AEggModel &zappy::gui::raylib::MapRenderer::_getEgg(const int &id) const
{
    for (const auto &egg : _eggs) {
        if (egg->getId() == id)
            return *egg;
    }
    throw RendererError("Egg " + std::to_string(id) + " not found", "MapRenderer");
}

/**
 * @brief Ajoute une rotation à un joueur avec un angle donné et une orientation.
 * @param player Référence au modèle joueur.
 * @param angle Angle de rotation en radians ou degrés.
 * @param orientation Nouvelle orientation du joueur.
 */
void zappy::gui::raylib::MapRenderer::_addRotation(
    const APlayerModel &player,
    const float &angle,
    const game::Orientation &orientation
) {
    Vector3 current = player.getOrientationRotation();
    Vector3 destination = {current.x, current.y + angle, current.z};

    Vector3 totalDelta = Vector3Subtract(destination, current);
    Vector3 perStep = Vector3Scale(totalDelta, 1.0f / static_cast<float>(ROTATION_TIME));

    Rotation rotation = {
        player.getId(),
        destination,
        perStep
    };

    auto action = PlayerActionFactory::createRotation(
        player.getId(),
        rotation,
        orientation,
        ROTATION_TIME
    );

    this->_playerActionQueues[player.getId()].push(std::move(action));
}

/**
 * @brief Met à jour les positions des joueurs et œufs en fonction du temps écoulé.
 * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
 */
void zappy::gui::raylib::MapRenderer::_updatePlayersAndEggs(const float &deltaUnits)
{
    for (auto &player : this->_players)
        player->update(deltaUnits);

    for (auto &egg : _eggs)
        egg->update(deltaUnits);
}

/**
 * @brief Met à jour les actions en cours des joueurs.
 * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
 */
void zappy::gui::raylib::MapRenderer::_updateActions(const float &deltaUnits)
{
    for (auto it = this->_playerActionQueues.begin(); it != this->_playerActionQueues.end(); ) {
        auto &queue = it->second;
        if (queue.empty()) {
            it = this->_playerActionQueues.erase(it);
            continue;
        }

        auto &action = queue.front();
        APlayerModel &player = _getPlayer(action->getPlayerId());

        if (!action->hasActionStarted())
            action->startAction();

        if (action->ActionWillEnd(deltaUnits)) {
            action->finishAction(deltaUnits, player);
            queue.pop();
            _updatePlayerAfterAction(player, queue);
        } else
            action->update(deltaUnits, player);
        ++it;
    }

    this->_updateAnimActions(deltaUnits);
}

/**
 * @brief Met à jour un joueur après l'exécution de ses actions.
 * @param player Référence au modèle joueur à mettre à jour.
 * @param actions Queue des actions du joueur.
 */
void zappy::gui::raylib::MapRenderer::_updatePlayerAfterAction(APlayerModel &player, const std::queue<std::shared_ptr<IPlayerAction>> &actions)
{
    if (actions.empty()) {
        player.idle();
        return;
    }

    std::shared_ptr<IPlayerAction> nextAction = actions.front();

    if (!std::dynamic_pointer_cast<PlayerTranslation>(nextAction))
        player.idle();
}

/**
 * @brief Met à jour les animations liées aux actions des joueurs.
 * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
 */
void zappy::gui::raylib::MapRenderer::_updateAnimActions(const float &deltaUnits)
{
    for (auto it = this->_playerAnimAction.begin(); it != this->_playerAnimAction.end(); ++it) {
        if (it->use_count() > 1)
            continue;

        auto action = (*it);
        APlayerModel &player = _getPlayer(action->getPlayerId());

        action->update(deltaUnits, player);
    }
}

/**
 * @brief Rend les joueurs et œufs sur la scène.
 */
void zappy::gui::raylib::MapRenderer::_renderPlayersAndEggs()
{
    for (auto &player : this->_players)
        player->render();

    for (auto &egg : _eggs)
        egg->render();
}

/**
 * @brief Rend les ressources sur la scène.
 */
void zappy::gui::raylib::MapRenderer::_renderResources()
{
    constexpr float uniformHeight = 0.1f;
    constexpr float spacing = 0.2f;

    for (size_t y = 0; y < _map->getHeight(); ++y) {
        for (size_t x = 0; x < _map->getWidth(); ++x) {
            const auto &tile = _map->getTile(x, y);
            const auto &resources = tile.getResources();
            Vector3 basePos = this->_floor->get3DCoords(x, y);
            int typeIndex = 0;

            for (size_t i = 0; i < zappy::game::RESOURCE_QUANTITY; ++i) {
                size_t quantity = resources[i];
                if (quantity == 0 || !_resourceModels[i])
                    continue;

                for (size_t q = 0; q < quantity; ++q) {
                    Vector3 pos = {
                        basePos.x + (q % 2) * spacing + (typeIndex % 3) * spacing,
                        uniformHeight,
                        basePos.z + (q / 2) * spacing + (typeIndex / 3) * spacing
                    };

                    _resourceModels[i]->setPosition(pos);
                    _resourceModels[i]->render();
                }
                typeIndex++;
            }
        }
    }
}

/**
 * @brief Rend les animations liées aux actions des joueurs.
 */
void zappy::gui::raylib::MapRenderer::_renderAnimActions()
{
    auto it = _playerAnimAction.begin();
    while (it != _playerAnimAction.end()) {
        const auto &action = *it;
        if (!action || action->hasEffectEnded()) {
            it = _playerAnimAction.erase(it);
            continue;
        }

        if (!action->hasActionStarted()) {
            ++it;
            continue;
        }

        auto &player = _getPlayer(action->getPlayerId());
        action->render(player.getPosition());
        ++it;
    }
}
