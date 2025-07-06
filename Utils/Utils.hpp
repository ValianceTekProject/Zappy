/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Utils
*/

#pragma once

#include <csignal>
#include <iostream>

#include "IObserver.hpp"

namespace zappy {

    namespace server {
        class Server;
    }

    namespace game {
        class Game;
    }

    namespace utils {

        /**
         * @class Signal
         * @brief Classe gérant les signaux système pour le serveur Zappy.
         *
         * Cette classe observe les signaux système (ex: SIGINT) et agit sur le serveur et le jeu en conséquence.
         */
        class Signal : public zappy::observer::IObserver {
           public:
            /**
             * @brief Constructeur.
             * @param server Référence au serveur à contrôler.
             * @param game Référence au jeu à contrôler.
             */
            explicit Signal(
                zappy::server::Server &server, zappy::game::Game &game);

            /**
             * @brief Méthode appelée lors d'une notification (signal reçu).
             * @param sig Le signal reçu.
             */
            void onNotify(int sig) override;

            /**
             * @brief Initialise le traitement des signaux avec l'instance donnée.
             * @param instance Instance de Signal à utiliser.
             */
            static void initSignalHandling(Signal *instance);

            /**
             * @brief Wrapper statique pour gérer les signaux.
             * @param sig Signal reçu.
             */
            static void signalWrapper(int sig);

            /**
             * @brief Remplace un handler de signal personnalisé.
             * @param sig Signal à gérer.
             * @param handler Fonction handler.
             */
            static void my_signal(int sig, sighandler_t handler);

            /**
             * @brief Arrête proprement le serveur lors d'un signal.
             * @param sig Signal reçu.
             */
            void stopServer(int sig);

            /**
             * @brief Ferme toutes les connexions clients.
             */
            void closeClients();

           private:
            server::Server &_server;   ///< Référence au serveur.
            game::Game &_game;         ///< Référence au jeu.
            static Signal *_instance;  ///< Instance statique pour le handler.
        };

        /**
         * @class Utils
         * @brief Classe utilitaire (vide actuellement).
         */
        class Utils {
           public:
        };

    }  // namespace utils

}  // namespace zappy
