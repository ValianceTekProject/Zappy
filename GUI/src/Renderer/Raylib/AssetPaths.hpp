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

                // Floor assets
                inline std::string FLOOR_PATH = std::string(ASSETS_PATH) + "grass.jpg";

                // Basic Scene assets
                inline std::string BASIC_SCENE_PATH = std::string(ASSETS_PATH) + "Basic/";

                inline std::string BASIC_PLAYER_PATH = BASIC_SCENE_PATH + "player.glb";

                inline std::string BASIC_EGG_PATH = BASIC_SCENE_PATH + "egg.glb";

                inline std::string BASIC_BG_PATH = BASIC_SCENE_PATH + "bg.glb";

                // Basic skybox assets
                inline std::string BASIC_SKYBOX_PATH = std::string(BASIC_SCENE_PATH) + "Skybox/";

                inline std::string BASIC_SKYBOX_MODEL_PATH = BASIC_SKYBOX_PATH + "Cubemap_Tropical_01-512x512.png";

                inline std::string BASIC_SKYBOX_VS_PATH = BASIC_SKYBOX_PATH + "skybox.vs";

                inline std::string BASIC_SKYBOX_FS_PATH = BASIC_SKYBOX_PATH + "skybox.fs";

                inline std::string BASIC_SKYBOX_CUBEMAP_VS_PATH = BASIC_SKYBOX_PATH + "cubemap.vs";

                inline std::string BASIC_SKYBOX_CUBEMAP_FS_PATH = BASIC_SKYBOX_PATH + "cubemap.fs";

                // Basic Scene resources assets

                inline std::string BASIC_SCENE_RESOURCES_PATH = BASIC_SCENE_PATH + "Resources/";

                inline std::string BASIC_FOOD_PATH = BASIC_SCENE_RESOURCES_PATH + "food.glb";
                inline std::string BASIC_LINEMATE_PATH = BASIC_SCENE_RESOURCES_PATH + "linemate.glb";
                inline std::string BASIC_DERAUMERE_PATH = BASIC_SCENE_RESOURCES_PATH + "deraumere.glb";
                inline std::string BASIC_SIBUR_PATH = BASIC_SCENE_RESOURCES_PATH + "sibur.glb";
                inline std::string BASIC_MENDIANE_PATH = BASIC_SCENE_RESOURCES_PATH + "mendiane.glb";
                inline std::string BASIC_PHIRAS_PATH = BASIC_SCENE_RESOURCES_PATH + "phiras.glb";
                inline std::string BASIC_THYSTAME_PATH = BASIC_SCENE_RESOURCES_PATH + "thystame.glb";

                // Pokemon Scene assets
            }
        }
    }
}
