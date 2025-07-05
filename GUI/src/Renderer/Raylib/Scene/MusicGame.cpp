/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Music.cpp
*/

#include "MusicGame.hpp"

zappy::gui::raylib::MusicGame::MusicGame()
    : _music(),
    _pause(false) {}

void zappy::gui::raylib::MusicGame::init(const std::string &musicPath)
{
    _music = LoadMusicStream(musicPath.c_str());
}

void zappy::gui::raylib::MusicGame::update()
{
    UpdateMusicStream(_music);
}

void zappy::gui::raylib::MusicGame::render() const
{
    if (_pause) {
        if (IsMusicStreamPlaying(_music))
            StopMusicStream(_music);
    } else {
        if (!IsMusicStreamPlaying(_music))
            PlayMusicStream(_music);
    }
}

void zappy::gui::raylib::MusicGame::unload()
{
    UnloadMusicStream(_music);
    CloseAudioDevice();
}