/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** AxolotlPlayerModel.cpp
*/

#include "AxolotlPlayerModel.hpp"

zappy::gui::raylib::AxolotlPlayerModel::AxolotlPlayerModel(const int &id) :
    APlayerModel::APlayerModel(id)
{
    constexpr float scale = 0.5f;
    constexpr Vector3 headOrigin = {0, 1.0, 0};

    this->setScale(scale);
    this->setHeadOrigin(headOrigin);
}

void zappy::gui::raylib::AxolotlPlayerModel::init()
{
    APlayerModel::init();
    APlayerModel::_initModel(assets::MINECRAFT_AXOLOTL_MODEL);

    // Texture2D tex = LoadTexture(assets::MINECRAFT_AXOLOTL_TEXTURE.c_str());
    // if (!tex.id)
    //     std::cerr << "Failed to load texture: " << assets::MINECRAFT_AXOLOTL_TEXTURE << std::endl;
    // // Assigne la texture diffuse au matériau 0
    // this->_model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = tex;

    constexpr int idle = 1;
    constexpr int walk = 2;

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
