/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AModel.cpp
*/

#include "AModel.hpp"

zappy::gui::raylib::AModel::AModel() :
    _position({ 0.0f, 0.0f, 0.0f }),
    _scale(1),
    _rotation({ 0.0f, 0.0f, 0.0f }),
    _color(WHITE),
    _model()
{}

void zappy::gui::raylib::AModel::init()
{}

void zappy::gui::raylib::AModel::setRotation(const Vector3 &rotation)
{
    this->_rotation = rotation;
}

void zappy::gui::raylib::AModel::translate(const Vector3 &translation)
{
    this->_position = Vector3Add(_position, translation);
}

void zappy::gui::raylib::AModel::rotate(const Vector3 &rotation)
{
    this->_rotation.x += rotation.x;
    this->_rotation.y += rotation.y;
    this->_rotation.z += rotation.z;
}

void zappy::gui::raylib::AModel::render()
{
    const Vector3 scale = { _scale, _scale, _scale };

    Vector3 axis = Vector3Normalize(_rotation);
    float angle = Vector3Length(_rotation);

    if (angle == 0.0f)
        axis = { 0.0f, 1.0f, 0.0f };

    DrawModelEx(_model, _position, axis, angle, scale, _color);
}

void zappy::gui::raylib::AModel::_initModel(const std::string &modelPath)
{
    this->_model = LoadModel(modelPath.c_str());
    if (!IsModelValid(this->_model) || _model.meshCount == 0)
        std::cerr << modelPath.c_str() << "Model not load" << std::endl;
}
