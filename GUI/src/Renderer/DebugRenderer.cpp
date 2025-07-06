/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** NcursesRenderer.cpp
*/

#include "DebugRenderer.hpp"

/// @brief Constructeur du renderer Debug.
/// Initialise les pointeurs de menu et la scène par défaut.
zappy::gui::DebugRenderer::DebugRenderer() :
    ARenderer::ARenderer(),
    _shouldClose(false)
{}
