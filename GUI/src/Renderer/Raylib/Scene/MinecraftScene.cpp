/*
** EPITECH PROJECT, 2025
** GUI
** File description:
** MinecraftScene.cpp
*/

#include "MinecraftScene.hpp"

zappy::gui::raylib::MinecraftScene::MinecraftScene(const std::shared_ptr<game::GameState> &gameState) :
    AScene::AScene(gameState)
{
    srand(time(NULL));
}

void zappy::gui::raylib::MinecraftScene::init()
{
    AScene::init();

    this->_mapRenderer->init(assets::MINECRAFT_FLOOR_PATH);

    for (size_t i = 0; i < zappy::game::RESOURCE_QUANTITY; ++i) {
        auto type = static_cast<zappy::game::Resource>(i);
        auto model = std::make_unique<zappy::gui::raylib::PokemonResourceModel>(-1, type);
        _mapRenderer->addResourceModel(type, std::move(model));
    }

    // Init le background
    this->_skybox.init(zappy::gui::raylib::assets::BASIC_SKYBOX_MODEL_PATH);
}

bool zappy::gui::raylib::MinecraftScene::shouldClose() const
{
    return WindowShouldClose();
}

void zappy::gui::raylib::MinecraftScene::addEgg(const int &id)
{
    auto egg = std::make_unique<PokemonEggModel>(id);

    _mapRenderer->addEgg(std::move(egg));

    AScene::addEgg(id);
}

void zappy::gui::raylib::MinecraftScene::addPlayer(const int &id)
{
    float r = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);

    float cumulated = 0.0f;
    size_t modelIndex = 0;
    for (size_t i = 0; i < _constructorProbabilities.size(); ++i) {
        cumulated += _constructorProbabilities[i];
        if (r < cumulated) {
            modelIndex = i;
            break;
        }
    }

    if (modelIndex >= _constructorProbabilities.size())
        modelIndex = 0;

    auto player = _playerModelsConstructors.at(0)(id);

    _mapRenderer->addPlayer(std::move(player));

    AScene::addPlayer(id);
}

void zappy::gui::raylib::MinecraftScene::endGame(const std::string &teamName)
{
    DrawText(TextFormat("Team %s wins!", teamName.c_str()), 10, 10, 20, GREEN);
}
