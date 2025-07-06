/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** TeamGui
*/

#pragma once

#include "ATeams.hpp"

namespace zappy {
    namespace game {
        class ServerPlayer;

        /**
         * @brief GUI-specific team implementation for the Zappy game
         * 
         * This class extends ATeams to provide a team implementation specifically
         * designed for GUI client interactions. It inherits all the standard team
         * functionality from ATeams including thread-safe player management,
         * team identity, and player list operations. This class serves as a
         * concrete implementation for teams that may have GUI-specific requirements
         * or behaviors in the future.
         */
        class TeamsGui : public ATeams {
           public:
            /**
             * @brief Constructor for TeamsGui
             * 
             * Creates a new GUI team by calling the parent ATeams constructor.
             * The team is initialized with a name and unique identifier,
             * inheriting all standard team functionality.
             * 
             * @param name The name of the team (immutable)
             * @param id Unique identifier for the team
             */
            TeamsGui(const std::string &name, int id) : ATeams(name, id) {}

            /**
             * @brief Default destructor
             * 
             * Uses the default destructor as no additional cleanup is required
             * beyond what the parent ATeams class provides.
             */
            ~TeamsGui() = default;

           private:
        };
    }  // namespace game
}  // namespace zappy
