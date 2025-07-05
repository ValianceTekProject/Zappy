/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** Music.hpp
*/

#pragma once
#include <string>
#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            class MusicGame {
                public:
                    MusicGame();
                    ~MusicGame() = default;

                    void init(const std::string &musicPath);

                    void update();

                    void render() const;

                    void unload();

                    void setPause(bool pause) { _pause = pause; }

                private:
                    Music _music;

                    bool _pause;
            };
        }
    }
}