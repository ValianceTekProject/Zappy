/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonEggModel.cpp
*/

#include "PokemonEggModel.hpp"

zappy::gui::raylib::PokemonEggModel::PokemonEggModel(const int &id) : AEggModel::AEggModel(id)
{
    constexpr float scale = 0.15f;
    setScale(scale);
}

void zappy::gui::raylib::PokemonEggModel::init()
{
    AEggModel::init();

    AEggModel::_initModel(assets::POKEMON_EGG_PATH);

    if (_animsCount > 0 && _modelAnimations != nullptr) {
        std::cout << "Configuring " << _animsCount << " animation(s) for egg model" << std::endl;

        constexpr int singleAnimation = 0;

        this->_animationIndexMap[State::IDLE] = singleAnimation;
        this->_animationIndexMap[State::OPEN] = singleAnimation;

        constexpr float idleSpeed = 7.5f;
        constexpr float openSpeed = 15.0f;

        this->_animationFrameSpeedMap[State::IDLE] = idleSpeed;
        this->_animationFrameSpeedMap[State::OPEN] = openSpeed;
    }
}
