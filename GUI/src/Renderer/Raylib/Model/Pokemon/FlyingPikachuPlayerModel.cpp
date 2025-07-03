/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** FlyingPikachuPlayerModel.cpp
*/

#include "FlyingPikachuPlayerModel.hpp"

#include <raylib.h>
#include <stdio.h>

zappy::gui::raylib::FlyingPikachuPlayerModel::FlyingPikachuPlayerModel(const int &id) :
    APlayerModel::APlayerModel(id)
{
    constexpr float scale = 0.3;
    constexpr Vector3 headOrigin = {0, 1.5, 0};

    this->setScale(scale);
    this->setHeadOrigin(headOrigin);
}

void zappy::gui::raylib::FlyingPikachuPlayerModel::init()
{
    APlayerModel::init();

    APlayerModel::_initModel(assets::POKEMON_FLYING_PIKACHU_PATH);

    constexpr Vector3 rotation = {0, -180, 0};
    AModel::rotate(rotation);

    constexpr int idle = 1;
    constexpr int walk = 1;

    this->_animationIndexMap[State::IDLE] = idle;
    this->_animationIndexMap[State::WALK] = walk;
    this->_animationIndexMap[State::EJECT] = walk;

    constexpr float idleSpeed = 4.f;
    constexpr float walkSpeed = 4.f;
    constexpr float ejectSpeed = walkSpeed * 7.0f;

    this->_animationFrameSpeedMap[State::IDLE] = idleSpeed;
    this->_animationFrameSpeedMap[State::WALK] = walkSpeed;
    this->_animationFrameSpeedMap[State::EJECT] = ejectSpeed;
}

void zappy::gui::raylib::FlyingPikachuPlayerModel::idle()
{
    this->_state = State::IDLE;
}
