/*
** EPITECH PROJECT, 2024
** GUI
** File description:
** AFloor.cpp
*/

#include "AFloor.hpp"

zappy::gui::raylib::AFloor::AFloor(
    const size_t &width,
    const size_t &height,
    const std::string &tileTexturePath,
    const float &tileSize
) : _width(width),
    _height(height),
    _tileSize(tileSize),
    _tileTexturePath(tileTexturePath),
    _tileTexture()
{}
