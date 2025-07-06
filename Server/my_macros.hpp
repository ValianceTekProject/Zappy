//
// EPITECH PROJECT, 2025
// My_maros
// File description:
// Macros and constext for the entire project
//

#pragma once

/**
 * @file Constants.hpp
 * @brief Définitions de constantes globales et énumérations pour le projet Zappy.
 */

#define OK 0   /**< Code de succès */
#define KO 84  /**< Code d'erreur */

namespace zappy {

    /**
     * @enum RunningState
     * @brief États possibles du serveur ou du jeu.
     */
    enum class RunningState { 
        RUN,   /**< Le serveur ou jeu est en cours d'exécution */
        PAUSE, /**< Le serveur ou jeu est en pause */
        STOP   /**< Le serveur ou jeu est arrêté */
    };
    
    /** 
     * @brief Constante utilisée pour indiquer qu'une valeur n'a pas été initialisée ou est invalide.
     */
    constexpr int noValue = -1;

    /**
     * @brief Séquence de caractères considérée comme fin de commande ou espace blanc.
     */
    constexpr char endSequence[] = " \n\r\t";

    namespace game {
        // Espace réservé pour le namespace game
    }

    namespace server {
        // Espace réservé pour le namespace server
    }
}  // namespace zappy
