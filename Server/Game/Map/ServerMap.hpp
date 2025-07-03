//
// EPITECH PROJECT, 2025
// Map
// File description:
// Map
//

#pragma once

#include "Egg.hpp"
#include "Map.hpp"
#include "ITeams.hpp"
#include <sstream>
#include <list>
#include <chrono>
#include "TeamsGui.hpp"
#include "GuiCommand.hpp"

namespace zappy {
    namespace game {

        #define SERVER_FATHER_ID -1
        
        class CommandHandlerGui;

        class MapServer : public Map {
           public:
            explicit MapServer(int mapWidth, int mapHeight, zappy::game::CommandHandlerGui &commandHandlerGui);
            ~MapServer() = default;

            void setEggsonMap(std::vector<std::shared_ptr<ITeams>> &teamList, int clientNb);
            int addNewEgg(int teamId, int x, int y);
            zappy::game::Egg popEgg();
            void replaceResources();

            std::list<Egg> &getEggList() { return _eggList; }

            std::chrono::steady_clock::time_point _lastResourceRespawn = std::chrono::steady_clock::now();
            std::mutex _resourceMutex;

           private:
            int _idEggTot = 2;
            void _placeResources();
            std::list<Egg> _eggList;
            zappy::game::CommandHandlerGui &_commandHandlerGui;
        };

    }  // namespace game
}  // namespace zappy
