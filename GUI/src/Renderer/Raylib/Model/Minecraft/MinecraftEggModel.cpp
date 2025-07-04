/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** MinecraftEggModel.cpp
*/

#include "MinecraftEggModel.hpp"

zappy::gui::raylib::MinecraftEggModel::MinecraftEggModel(const int &id) : AEggModel::AEggModel(id)
{
    constexpr float scale = 0.1f;
    setScale(scale);
}

void zappy::gui::raylib::MinecraftEggModel::init()
{
    AEggModel::init();

    AEggModel::_initModel(assets::MINECRAFT_EGG_PATH);

    constexpr int idle = 0;
    constexpr int open = 2;

    this->_animationIndexMap[State::IDLE] = idle;
    this->_animationIndexMap[State::OPEN] = open;

    constexpr float idleSpeed = 7.5f;
    constexpr float openSpeed = 7.5f;

    this->_animationFrameSpeedMap[State::IDLE] = idleSpeed;
    this->_animationFrameSpeedMap[State::OPEN] = openSpeed;
}
