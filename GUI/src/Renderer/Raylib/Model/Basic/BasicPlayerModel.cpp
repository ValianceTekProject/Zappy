/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** BasicPlayerModel.cpp
*/

#include "BasicPlayerModel.hpp"

#include <raylib.h>
#include <stdio.h>

zappy::gui::raylib::BasicPlayerModel::BasicPlayerModel(const int &id) :
    APlayerModel::APlayerModel(id)
{
    constexpr float scale = 0.15f;
    constexpr Vector3 headOrigin = { 0.0f, 1.5f, 0.0f };

    this->setScale(scale);
    this->setHeadOrigin(headOrigin);
}

void zappy::gui::raylib::BasicPlayerModel::init()
{
    APlayerModel::init();

    APlayerModel::_initModel(assets::BASIC_PLAYER_PATH);

    constexpr Vector3 rotation = { 0.0f, 180.0f, 0.0f };
    this->_northRotation = rotation;
    AModel::setRotation(this->_northRotation);

    constexpr int idle = 2;
    constexpr int walk = 10;

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
