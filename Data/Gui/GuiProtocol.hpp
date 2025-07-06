/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GuiProtocol.hpp
*/

#pragma once

#include <iostream>
#include <map>
#include <algorithm>

namespace zappy {
    namespace network {
        /**
         * @enum GuiProtocol
         * @brief Enum representing the different GUI protocol commands.
         */
        enum class GuiProtocol {
            MAP_SIZE,               ///< Map size command ("msz")
            TILE_CONTENT,           ///< Tile content command ("bct")
            MAP_CONTENT,            ///< Map content command ("mct")
            TEAM_NAME,              ///< Team name command ("tna")
            NEW_PLAYER,             ///< New player command ("pnw")
            PLAYER_POSITION,        ///< Player position command ("ppo")
            PLAYER_LEVEL,           ///< Player level command ("plv")
            PLAYER_INVENTORY,       ///< Player inventory command ("pin")
            PLAYER_EXPULSION,       ///< Player expulsion command ("pex")
            PLAYER_BROADCAST,       ///< Player broadcast command ("pbc")
            INCANTATION_START,      ///< Incantation start command ("pic")
            INCANTATION_END,        ///< Incantation end command ("pie")
            EGG_LAYING,             ///< Egg laying command ("pfk")
            RESOURCE_DROP,          ///< Resource drop command ("pdr")
            RESOURCE_COLLECT,       ///< Resource collect command ("pgt")
            PLAYER_DEATH,           ///< Player death command ("pdi")
            EGG_CREATED,            ///< Egg created command ("enw")
            EGG_HATCH,              ///< Egg hatch command ("ebo")
            EGG_DESTROYED,          ///< Egg destroyed command ("edi")
            TIME_UNIT_REQUEST,      ///< Time unit request command ("sgt")
            TIME_UNIT_MODIFICATION, ///< Time unit modification command ("sst")
            GAME_END,               ///< Game end command ("seg")
            SERVER_MESSAGE,         ///< Server message command ("smg")
            UNKNOWN_COMMAND,        ///< Unknown command ("suc")
            COMMAND_PARAMETER,      ///< Command parameter ("sbp")
            SIZE                    ///< Number of commands
        };

        /// Alias for GuiProtocol
        using GP = GuiProtocol;

        /**
         * @brief Cast GuiProtocol enum to its underlying index.
         * @param cmd GuiProtocol value.
         * @return size_t Corresponding index.
         */
        static constexpr size_t castGuiProtocol(GP cmd) noexcept {
            return static_cast<size_t>(cmd);
        }

        /// Number of GUI protocol commands
        static constexpr size_t GuiProtocolCount = castGuiProtocol(GP::SIZE);

        /**
         * @brief Mapping from string command to GuiProtocol enum.
         */
        const std::map<std::string, GuiProtocol> guiProtocolMap = {
            { "msz", GP::MAP_SIZE },
            { "bct", GP::TILE_CONTENT },
            { "mct", GP::MAP_CONTENT },
            { "tna", GP::TEAM_NAME },
            { "pnw", GP::NEW_PLAYER },
            { "ppo", GP::PLAYER_POSITION },
            { "plv", GP::PLAYER_LEVEL },
            { "pin", GP::PLAYER_INVENTORY },
            { "pex", GP::PLAYER_EXPULSION },
            { "pbc", GP::PLAYER_BROADCAST },
            { "pic", GP::INCANTATION_START },
            { "pie", GP::INCANTATION_END },
            { "pfk", GP::EGG_LAYING },
            { "pdr", GP::RESOURCE_DROP },
            { "pgt", GP::RESOURCE_COLLECT },
            { "pdi", GP::PLAYER_DEATH },
            { "enw", GP::EGG_CREATED },
            { "ebo", GP::EGG_HATCH },
            { "edi", GP::EGG_DESTROYED },
            { "sgt", GP::TIME_UNIT_REQUEST },
            { "sst", GP::TIME_UNIT_MODIFICATION },
            { "seg", GP::GAME_END },
            { "smg", GP::SERVER_MESSAGE },
            { "suc", GP::UNKNOWN_COMMAND },
            { "sbp", GP::COMMAND_PARAMETER }
        };

        /**
         * @brief Get the GuiProtocol enum corresponding to a string command.
         * @param str Command string.
         * @return GuiProtocol Corresponding enum or UNKNOWN_COMMAND if not found.
         */
        inline GuiProtocol getGuiProtocol(const std::string &str) {
            auto it = guiProtocolMap.find(str);
            if (it == guiProtocolMap.end())
                return GP::UNKNOWN_COMMAND;
            return it->second;
        }

        /**
         * @brief Get the string command corresponding to a GuiProtocol enum.
         * @param cmd GuiProtocol enum.
         * @return std::string Corresponding command string or unknown command string.
         */
        inline std::string getGuiProtocol(GuiProtocol cmd) {
            for (auto &[key, value] : guiProtocolMap) {
                if (value == cmd)
                    return key;
            }
            return getGuiProtocol(GP::UNKNOWN_COMMAND);
        }
    }
}
