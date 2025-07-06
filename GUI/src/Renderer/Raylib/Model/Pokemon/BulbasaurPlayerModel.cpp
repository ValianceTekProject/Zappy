/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** BulbasaurPlayerModel.cpp
*/

#include "BulbasaurPlayerModel.hpp"

#include <raylib.h>
#include <stdio.h>

zappy::gui::raylib::BulbasaurPlayerModel::BulbasaurPlayerModel(const int &id) :
    APlayerModel::APlayerModel(id)
{
    constexpr float scale = 100.f;
    constexpr Vector3 headOrigin = {0, 1.0, 0};

    this->setScale(scale);
    this->setHeadOrigin(headOrigin);
}

void zappy::gui::raylib::BulbasaurPlayerModel::init()
{
    APlayerModel::init();
    APlayerModel::_initModel(assets::POKEMON_BULBASAUR_MODEL);

    constexpr int textureCount = 3;
    for (int i = 0; i < textureCount; ++i) {
        std::string path = assets::POKEMON_BULBASAUR_TEXTURE + std::to_string(i) + ".jpeg";
        Texture2D tex = LoadTexture(path.c_str());
        if (!tex.id) {
            std::cerr << "Failed to load texture: " << path << std::endl;
            continue;
        }
        // Assigne la texture diffuse au matériau 0
        _model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;
    }

    constexpr Vector3 rotation = {0, 180, 0};
    this->_northRotation = rotation;
    AModel::setRotation(this->_northRotation);

    constexpr int idle = 1;
    constexpr int walk = 8;

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
