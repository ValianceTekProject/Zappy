/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IPlayerAction.hpp
*/

#pragma once

#include "Movement.hpp"
#include "APlayerModel.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class IPlayerAction
            {
                public:
                    /**
                     * @brief Destructeur virtuel par défaut.
                     */
                    virtual ~IPlayerAction() = default;

                    /**
                     * @brief Récupère l'identifiant du joueur associé à cette action.
                     * @return Identifiant du joueur.
                     */
                    virtual int getPlayerId() const = 0;

                    /**
                     * @brief Récupère le type d'action.
                     * @return Type de l'action.
                     */
                    virtual ActionType getActionType() const = 0;

                    /**
                     * @brief Indique si l'action a déjà commencé.
                     * @return true si l'action a commencé, false sinon.
                     */
                    virtual bool hasActionStarted() const = 0;

                    /**
                     * @brief Démarre l'action.
                     */
                    virtual void startAction() = 0;

                    /**
                     * @brief Met à jour l'action avec le temps écoulé et le joueur concerné.
                     * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
                     * @param player Référence au joueur affecté par l'action.
                     */
                    virtual void update(const float &deltaUnits, APlayerModel &player) = 0;

                    /**
                     * @brief Indique si l'action va se terminer dans ce frame.
                     * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
                     * @return true si l'action va se terminer, false sinon.
                     */
                    virtual bool ActionWillEnd(const float &deltaUnits) const = 0;

                    /**
                     * @brief Termine l'action et effectue les opérations finales.
                     * @param deltaUnits Temps écoulé depuis la dernière mise à jour.
                     * @param player Référence au joueur affecté par l'action.
                     */
                    virtual void finishAction(const float &deltaUnits, APlayerModel &player) = 0;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
