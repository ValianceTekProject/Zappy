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

void zappy::gui::ARenderer::init()
{
    this->_lastTime = std::chrono::steady_clock::now();
}

void zappy::gui::ARenderer::setGameState(std::shared_ptr<game::GameState> &gameState)
{
    this->_gameState = gameState;
}

void zappy::gui::ARenderer::setProtocolRequests(const ProtocolRequest &protocolRequests)
{
    this->_protocolRequests = protocolRequests;
}

void zappy::gui::ARenderer::setFrequency(const size_t &frequency)
{
    _checkGameState();
    this->_gameState->setFrequency(frequency);
}

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

void zappy::gui::ARenderer::addEgg(
    const int &eggId,
    const int &fatherId,
    const int &x,
    const int &y
) {
    _checkGameState();
    this->_gameState->addEgg(eggId, fatherId, x, y);
}

void zappy::gui::ARenderer::addPlayer(const game::Player &player)
{
    _checkGameState();
    this->_gameState->addPlayer(player);
}

void zappy::gui::ARenderer::updatePlayerPosition(
    const int &id,
    const int &x,
    const int &y,
    const game::Orientation &orientation
) {
    _checkGameState();
    this->_gameState->updatePlayerPosition(id, x, y, orientation);
}

void zappy::gui::ARenderer::updatePlayerLevel(const int &id, const size_t &level)
{
    _checkGameState();
    this->_gameState->updatePlayerLevel(id, level);
}

void zappy::gui::ARenderer::updatePlayerInventory(const int &id, const game::Inventory &inventory)
{
    _checkGameState();
    this->_gameState->updatePlayerInventory(id, inventory);
}

void zappy::gui::ARenderer::playerExpulsion(const int &id)
{
    _checkGameState();
    this->_gameState->playerExpulsion(id);
}

void zappy::gui::ARenderer::playerBroadcast(const int &id, const std::string &message)
{
    _checkGameState();
    this->_gameState->playerBroadcast(id, message);
}

void zappy::gui::ARenderer::startIncantation(
    const int &x, const int &y,
    const int &level,
    const std::vector<int> &playerIds
) {
    _checkGameState();
    this->_gameState->startIncantation(x, y, level, playerIds);
}

void zappy::gui::ARenderer::endIncantation(const int &x, const int &y, const bool &result)
{
    _checkGameState();
    this->_gameState->endIncantation(x, y, result);
}

void zappy::gui::ARenderer::hatchEgg(const int &eggId)
{
    _checkGameState();
    this->_gameState->hatchEgg(eggId);
}

void zappy::gui::ARenderer::removeEgg(const int &eggId)
{
    _checkGameState();
    this->_gameState->removeEgg(eggId);
}

void zappy::gui::ARenderer::removePlayer(const int &id)
{
    _checkGameState();
    this->_gameState->removePlayer(id);
}

void zappy::gui::ARenderer::endGame(const std::string &teamName)
{
    _checkGameState();
    this->_gameState->endGame(teamName);
}

void zappy::gui::ARenderer::_checkGameState() const
{
    if (!this->_gameState)
        throw GuiError("Game state is not set", "ARenderer");
}
