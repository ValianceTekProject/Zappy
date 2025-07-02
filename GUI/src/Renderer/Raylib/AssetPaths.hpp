/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** AssetPaths.hpp
*/

#pragma once

#include <string>

namespace zappy {
    namespace gui {
        namespace raylib {
            namespace assets {
                constexpr const char *ASSETS_PATH = "src/Renderer/Raylib/Assets/";

                // Basic Scene assets
                inline std::string BASIC_SCENE_PATH = std::string(ASSETS_PATH) + "Basic/";

                inline std::string BASIC_PLAYER_PATH = BASIC_SCENE_PATH + "player.glb";

                inline std::string BASIC_EGG_PATH = BASIC_SCENE_PATH + "egg.glb";

                // Pokemon Scene assets
                inline std::string POKEMON_SCENE_PATH = std::string(ASSETS_PATH) + "Pokemon/";

                inline std::string POKEMON_PLAYER_PATH = BASIC_SCENE_PATH + "player.glb";

                inline std::string POKEMON_EGG_PATH = POKEMON_SCENE_PATH + "egg.glb";

                // Floor assets
                inline std::string BASIC_FLOOR_PATH = std::string(BASIC_SCENE_PATH) + "floor.jpg";

                inline std::string POKEMON_FLOOR_PATH = std::string(POKEMON_SCENE_PATH) + "floor.png";

                // Basic Scene resources assets
                inline std::string BASIC_SCENE_RESOURCES_PATH = std::string(BASIC_SCENE_PATH) + "Resources/";

                inline std::string BASIC_FOOD_PATH = BASIC_SCENE_RESOURCES_PATH + "food.glb";
                inline std::string BASIC_LINEMATE_PATH = BASIC_SCENE_RESOURCES_PATH + "linemate.glb";
                inline std::string BASIC_DERAUMERE_PATH = BASIC_SCENE_RESOURCES_PATH + "deraumere.glb";
                inline std::string BASIC_SIBUR_PATH = BASIC_SCENE_RESOURCES_PATH + "sibur.glb";
                inline std::string BASIC_MENDIANE_PATH = BASIC_SCENE_RESOURCES_PATH + "mendiane.glb";
                inline std::string BASIC_PHIRAS_PATH = BASIC_SCENE_RESOURCES_PATH + "phiras.glb";
                inline std::string BASIC_THYSTAME_PATH = BASIC_SCENE_RESOURCES_PATH + "thystame.glb";

                // Pokemon Scene resources assets
                inline std::string POKEMON_SCENE_RESOURCES_PATH = std::string(POKEMON_SCENE_PATH) + "Resources/";

                inline std::string POKEMON_FOOD_PATH = POKEMON_SCENE_RESOURCES_PATH + "food.glb";
                inline std::string POKEMON_LINEMATE_PATH = POKEMON_SCENE_RESOURCES_PATH + "linemate.glb";
                inline std::string POKEMON_DERAUMERE_PATH = POKEMON_SCENE_RESOURCES_PATH + "deraumere.glb";
                inline std::string POKEMON_SIBUR_PATH = POKEMON_SCENE_RESOURCES_PATH + "sibur.glb";
                inline std::string POKEMON_MENDIANE_PATH = POKEMON_SCENE_RESOURCES_PATH + "mendiane.glb";
                inline std::string POKEMON_PHIRAS_PATH = POKEMON_SCENE_RESOURCES_PATH + "phiras.glb";
                inline std::string POKEMON_THYSTAME_PATH = POKEMON_SCENE_RESOURCES_PATH + "thystame.glb";
            }
        }
    }
}
