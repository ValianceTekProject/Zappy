/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** PokemonScene.cpp
*/

#include "PokemonScene.hpp"

zappy::gui::raylib::PokemonScene::PokemonScene(const std::shared_ptr<game::GameState> &gameState) :
    AScene::AScene(gameState)
{}

void zappy::gui::raylib::PokemonScene::init()
{
    AScene::init();

    for (size_t i = 0; i < zappy::game::RESOURCE_QUANTITY; ++i) {
        auto type = static_cast<zappy::game::Resource>(i);
        auto model = std::make_unique<zappy::gui::raylib::PokemonResourceModel>(-1, type); // ou AResourceModel si tu préfères
        _mapRenderer->addResourceModel(type, std::move(model));
    }

    // Init le background
    this->_skybox.init(zappy::gui::raylib::assets::BASIC_SKYBOX_MODEL_PATH);
}

bool zappy::gui::raylib::PokemonScene::shouldClose() const
{
    return WindowShouldClose();
}

void zappy::gui::raylib::PokemonScene::addEgg(const int &id)
{
    auto egg = std::make_unique<PokemonEggModel>(id);

    _mapRenderer->addEgg(std::move(egg));

    AScene::addEgg(id);
}

void zappy::gui::raylib::PokemonScene::addPlayer(const int &id)
{
    auto player = std::make_unique<PokemonPlayerModel>(id);

    _mapRenderer->addPlayer(std::move(player));

    AScene::addPlayer(id);
}

void zappy::gui::raylib::PokemonScene::endGame(const std::string &teamName)
{
    DrawText(TextFormat("Team %s wins!", teamName.c_str()), 10, 10, 20, GREEN);
}
