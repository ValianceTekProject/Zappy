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
                constexpr const char *ASSETS_PATH = "GUI/src/Renderer/Raylib/Assets/";

                // Theme Images assets
                inline std::string BASIC_THEME_IMAGE_PATH = std::string(ASSETS_PATH) + "basicTheme.png";

                inline std::string POKEMON_THEME_IMAGE_PATH = std::string(ASSETS_PATH) + "pokemonTheme.png";

                // Basic Scene assets
                inline std::string BASIC_SCENE_PATH = std::string(ASSETS_PATH) + "Basic/";

                inline std::string BASIC_FLOOR_PATH = std::string(BASIC_SCENE_PATH) + "floor.jpg";

                inline std::string BASIC_PLAYER_PATH = BASIC_SCENE_PATH + "player.glb";

                inline std::string BASIC_EGG_PATH = BASIC_SCENE_PATH + "egg.glb";

                inline std::string BASIC_BG_PATH = BASIC_SCENE_PATH + "bg.glb";

                // Basic Music asset
                inline std::string BASIC_MUSIC_PATH = std::string(BASIC_SCENE_PATH) + "music.mp3";

                // Basic Scene resources assets
                inline std::string BASIC_SCENE_RESOURCES_PATH = BASIC_SCENE_PATH + "Resources/";

                inline std::string BASIC_FOOD_PATH = BASIC_SCENE_RESOURCES_PATH + "food.glb";
                inline std::string BASIC_LINEMATE_PATH = BASIC_SCENE_RESOURCES_PATH + "linemate.glb";
                inline std::string BASIC_DERAUMERE_PATH = BASIC_SCENE_RESOURCES_PATH + "deraumere.glb";
                inline std::string BASIC_SIBUR_PATH = BASIC_SCENE_RESOURCES_PATH + "sibur.glb";
                inline std::string BASIC_MENDIANE_PATH = BASIC_SCENE_RESOURCES_PATH + "mendiane.glb";
                inline std::string BASIC_PHIRAS_PATH = BASIC_SCENE_RESOURCES_PATH + "phiras.glb";
                inline std::string BASIC_THYSTAME_PATH = BASIC_SCENE_RESOURCES_PATH + "thystame.glb";

                // Basic skybox assets
                inline std::string BASIC_SKYBOX_PATH = std::string(BASIC_SCENE_PATH) + "Skybox/";
                inline std::string BASIC_SKYBOX_MODEL_PATH = BASIC_SKYBOX_PATH + "Cubemap_Tropical_01-512x512.png";
                inline std::string BASIC_SKYBOX_VS_PATH = BASIC_SKYBOX_PATH + "skybox.vs";
                inline std::string BASIC_SKYBOX_FS_PATH = BASIC_SKYBOX_PATH + "skybox.fs";
                inline std::string BASIC_SKYBOX_CUBEMAP_VS_PATH = BASIC_SKYBOX_PATH + "cubemap.vs";
                inline std::string BASIC_SKYBOX_CUBEMAP_FS_PATH = BASIC_SKYBOX_PATH + "cubemap.fs";

                // Pokemon Scene assets
                inline std::string POKEMON_SCENE_PATH = std::string(ASSETS_PATH) + "Pokemon/";

                inline std::string POKEMON_FLOOR_PATH = std::string(POKEMON_SCENE_PATH) + "floor.png";

                inline std::string POKEMON_FLYING_PIKACHU_PATH = POKEMON_SCENE_PATH + "flyingPikachu.glb";
                inline std::string POKEMON_BULBASAUR_PATH = POKEMON_SCENE_PATH + "Bulbasaur/";
                inline std::string POKEMON_BULBASAUR_MODEL = POKEMON_BULBASAUR_PATH + "source/bulbasaur.glb";
                inline std::string POKEMON_BULBASAUR_TEXTURE = POKEMON_BULBASAUR_PATH + "textures/bulbasaur_";
                inline std::string POKEMON_BULBASAUR_TEXTURE_EXTENSION = POKEMON_BULBASAUR_PATH + ".jpeg";

                inline std::string POKEMON_EGG_PATH = POKEMON_SCENE_PATH + "egg.glb";

                // Pokemon Music asset
                inline std::string POKEMON_MUSIC_PATH = std::string(POKEMON_SCENE_PATH) + "music.mp3";

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
