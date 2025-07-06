/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IRenderer.hpp
*/

#pragma once

#include "GameState.hpp"
#include "GuiProtocol.hpp"

#include <iostream>
#include <memory>
#include <unordered_map>
#include <functional>

namespace zappy {
    namespace gui {
        using ProtocolRequest = std::unordered_map<network::GuiProtocol, std::function<void(const int &f, const int &d)>>;

        class IRenderer
        {
            public:
                virtual ~IRenderer() = default;

                /**
                 * @brief Initialize the renderer
                 */
                virtual void init() = 0;

                /**
                 * @brief Set the Game State object
                 *
                 * @param gameState shared pointer to the game state
                 */
                virtual void setGameState(std::shared_ptr<game::GameState> &gameState) = 0;

                /**
                 * @brief Set the Protocol Requests object
                 *
                 * @param protocolRequests the protocol requests
                 */
                virtual void setProtocolRequests(const ProtocolRequest &protocolRequests) = 0;

                /**
                 * @brief Set the Frequency object
                 *
                 * @param frequency new frequency
                 */
                virtual void setFrequency(const size_t &frequency) = 0;

                /**
                 * @brief Update the renderer's inputs
                 */
                virtual void handleInput() = 0;
                /**
                 * @brief Update the renderer
                 */
                virtual void update() = 0;

                /**
                 * @brief Render the renderer
                 */
                virtual void render() const = 0;

                /**
                 * @brief Check if the renderer should close
                 *
                 * @return true or false
                 */
                virtual bool shouldClose() const = 0;

                /**
                 * @brief Add an egg to the renderer
                 *
                 * @param eggId id of the egg
                 * @param fatherId id of the father of the egg
                 * @param x x position of the egg
                 * @param y y position of the egg
                 */
                virtual void addEgg(
                    const int &eggId,
                    const int &fatherId,
                    const int &x,
                    const int &y
                ) = 0;
                /**
                 * @brief Add a player to the renderer
                 *
                 * @param player Reference to the player
                 */
                virtual void addPlayer(const game::Player &player) = 0;

                /**
                 * @brief Update a player's position
                 *
                 * @param id id of the player
                 * @param x x position of the player
                 * @param y y position of the player
                 * @param orientation orientation of the player
                 */
                virtual void updatePlayerPosition(
                    const int &id,
                    const int &x,
                    const int &y,
                    const game::Orientation &orientation
                ) = 0;
                /**
                 * @brief Update a player's level
                 *
                 * @param id id of the player
                 * @param level level of the player
                 */
                virtual void updatePlayerLevel(const int &id, const size_t &level) = 0;
                /**
                 * @brief Update a player's inventory
                 *
                 * @param id id of the player
                 * @param inventory inventory of the player
                 */
                virtual void updatePlayerInventory(const int &id, const game::Inventory &inventory) = 0;

                /**
                 * @brief Player expulsion
                 *
                 * @param id id of the player
                 */
                virtual void playerExpulsion(const int &id) = 0;

                /**
                 * @brief Update the time unit
                 *
                 * @param id id of the player
                 * @param message message to send to the player
                 */
                virtual void playerBroadcast(const int &id, const std::string &message) = 0;

                /**
                 * @brief Update the time unit
                 *
                 * @param x x position of the time unit
                 * @param y y position of the time unit
                 * @param level level of the time unit
                 * @param playerIds ids of the players who collected the time unit
                 */
                virtual void startIncantation(
                    const int &x, const int &y,
                    const int &level,
                    const std::vector<int> &playerIds
                ) = 0;
                /**
                 * @brief Update the time unit
                 *
                 * @param x x position of the time unit
                 * @param y y position of the time unit
                 * @param result result of the incantation
                 */
                virtual void endIncantation(const int &x, const int &y, const bool &result) = 0;

                /**
                 * @brief Update the time unit
                 *
                 * @param eggId id of the egg
                 */
                virtual void hatchEgg(const int &eggId) = 0;

                /**
                 * @brief Update the time unit
                 *
                 * @param eggId id of the egg
                 */
                virtual void removeEgg(const int &eggId) = 0;
                /**
                 * @brief Remove a player from the renderer
                 *
                 * @param id id of the player
                 */
                virtual void removePlayer(const int &id) = 0;

                /**
                 * @brief Update the time unit
                 *
                 * @param teamName name of the team
                 */
                virtual void endGame(const std::string &teamName) = 0;
        };
    }
}
