/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** MenuMacros.ipp
*/

#pragma once

#include "MenuMacros.hpp"

inline zappy::gui::raylib::MenuModifiedSection zappy::gui::raylib::operator+(const MenuModifiedSection &modifiedSection, int offset) noexcept
{
    return static_cast<MenuModifiedSection>((static_cast<int>(modifiedSection) + offset + MENU_MOD_SECTION_SIZE) % MENU_MOD_SECTION_SIZE);
}

inline zappy::gui::raylib::MenuModifiedSection &zappy::gui::raylib::operator+=(MenuModifiedSection &modifiedSection, int offset) noexcept
{
    return modifiedSection = modifiedSection + offset;
}

inline zappy::gui::raylib::MenuModifiedSection &zappy::gui::raylib::operator++(MenuModifiedSection &ms) noexcept
{
    return ms = ms + 1;
}

inline zappy::gui::raylib::MenuModifiedSection zappy::gui::raylib::operator++(MenuModifiedSection &ms, int) noexcept
{
    MenuModifiedSection oldMs = ms;
    ms = ms + 1;
    return oldMs;
}
