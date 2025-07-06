/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** Gui.hpp
*/

#pragma once

#include "GuiError.hpp"
#include "ParsingError.hpp"

#include "GameState.hpp"

#include "RaylibRenderer.hpp"
#include "NcursesRenderer.hpp"
#include "DebugRenderer.hpp"

#include "NetworkManager.hpp"
#include "Protocol.hpp"

#include <string>
#include <sstream>
#include <memory>
#include <thread>

namespace zappy {
    namespace gui {
        class Gui
        {
            public:
                Gui();
                ~Gui() = default;

                void parseArgs(int argc, char const *argv[]);
                void init();

                void run();

            private:
                void _initNetwork();
                bool _isMapCreated() { return _gameState->getMap() != nullptr; }

                bool _debug;

                std::string _ip;
                size_t _port;

                std::unique_ptr<network::Protocol> _protocol;
                std::shared_ptr<game::GameState> _gameState;
                std::shared_ptr<IRenderer> _renderer;
        };
    }
}

