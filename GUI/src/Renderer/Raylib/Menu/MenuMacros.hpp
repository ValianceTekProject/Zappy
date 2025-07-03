/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MenuMacros.hpp
*/

#pragma once

#include "raylib.h"

#include <iostream>

namespace zappy {
    namespace gui {
        namespace raylib {
            constexpr float DEFAULT_SCREEN_WIDTH = 1920.f;
            constexpr float DEFAULT_SCREEN_HEIGHT = 1080.f;

            constexpr int DEFAULT_FONT_SIZE = 20;
            constexpr Color DEFAULT_TEXT_COLOR = WHITE;

            constexpr int MIN_FREQ = 1;
            constexpr int MAX_FREQ = 1000;

            constexpr int MIN_PLAYERS_DISPLAYED = 1;
            constexpr int MAX_PLAYERS_DISPLAYED = 6;

            enum class MenuState {
                HELP,
                BROADCASTS,
                PLAYERS,
            };

            enum class MenuModifiedSection {
                FREQ,
                PLAYERS,
                NONE,
            };

            constexpr int MENU_MOD_SECTION_SIZE = static_cast<int>(MenuModifiedSection::NONE);

            MenuModifiedSection operator+(const MenuModifiedSection &modifiedSection, int offset) noexcept;
            MenuModifiedSection &operator+=(MenuModifiedSection &modifiedSection, int offset) noexcept;
            MenuModifiedSection &operator++(MenuModifiedSection &ms) noexcept;
            MenuModifiedSection  operator++(MenuModifiedSection &ms, int) noexcept;

            struct MenuPlayerInfo {
                int id;
                bool hasIcon;
                Texture2D icon;

                MenuPlayerInfo(const int &id, const bool &hasIcon, const Texture2D &icon)
                    : id(id), hasIcon(hasIcon), icon(icon) {}
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy

#include "MenuMacros.ipp"
