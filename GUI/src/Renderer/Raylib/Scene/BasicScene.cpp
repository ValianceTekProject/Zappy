/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** BasicScene.cpp
*/

#include "BasicScene.hpp"

zappy::gui::raylib::BasicScene::BasicScene(const std::shared_ptr<game::GameState> &gameState)
    : AScene::AScene(gameState)
{}

void zappy::gui::raylib::BasicScene::init()
{
    AScene::init();

    this->_mapRenderer->init(assets::BASIC_FLOOR_PATH);

    for (size_t i = 0; i < zappy::game::RESOURCE_QUANTITY; ++i) {
        auto type = static_cast<zappy::game::Resource>(i);
        auto model = std::make_unique<zappy::gui::raylib::BasicResourceModel>(-1, type);
        _mapRenderer->addResourceModel(type, std::move(model));
    }

    // Init le background
    this->_skybox.init(zappy::gui::raylib::assets::BASIC_SKYBOX_MODEL_PATH);

    // Init la musique
    this->_music.init(zappy::gui::raylib::assets::BASIC_MUSIC_PATH);
}

bool zappy::gui::raylib::BasicScene::shouldClose() const
{
    return WindowShouldClose();
}

void zappy::gui::raylib::BasicScene::addEgg(const int &id)
{
    auto egg = std::make_unique<BasicEggModel>(id);

    _mapRenderer->addEgg(std::move(egg));

    AScene::addEgg(id);
}

void zappy::gui::raylib::BasicScene::addPlayer(const int &id)
{
    auto player = std::make_unique<BasicPlayerModel>(id);

    player->setColor(Color{
        static_cast<uint8_t>(GetRandomValue(0, 255)),
        static_cast<uint8_t>(GetRandomValue(0, 255)),
        static_cast<uint8_t>(GetRandomValue(0, 255)),
        255
    });

    _mapRenderer->addPlayer(std::move(player));

    AScene::addPlayer(id);
}

void zappy::gui::raylib::BasicScene::endGame(const std::string &teamName)
{
    DrawText(TextFormat("Team %s wins!", teamName.c_str()), 10, 10, 20, GREEN);
}
