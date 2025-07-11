/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** PlayerRotation.cpp
*/

#include "PlayerRotation.hpp"

zappy::gui::raylib::PlayerRotation::PlayerRotation(
    const int &playerId,
    const ActionType &actionType,
    const Rotation &movement,
    const game::Orientation &destination,
    const float &timeUnit,
    const float &elapsedTime
) :
    APlayerAction(playerId, actionType, timeUnit, elapsedTime),
    _movement(movement),
    _destination(destination)
{}

void zappy::gui::raylib::PlayerRotation::update(const float &deltaUnits, APlayerModel &player)
{
    if (player.getState() != APlayerModel::State::IDLE)
        player.idle();

    player.rotate(Vector3Scale(this->_movement.movementVector, deltaUnits));
    APlayerAction::update(deltaUnits, player);
}

void zappy::gui::raylib::PlayerRotation::finishAction(const float &deltaUnits, APlayerModel &player)
{
    APlayerAction::finishAction(deltaUnits, player);
    player.look(this->_destination);
}
