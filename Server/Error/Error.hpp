/*
** EPITECH PROJECT, 2025
** B-NWP-400-BDX-4-1-jetpack-baptiste.blambert
** File description:
** Error
*/

#pragma once

#include <stdexcept>
#include <string>

namespace zappy {
    namespace error {
        /**
         * @class Error
         * @brief Classe de base pour les erreurs personnalisées.
         */
        class Error : std::exception {
           public:
            /**
             * @brief Constructeur avec message d'erreur.
             * @param msg Message d'erreur.
             */
            Error(std::string msg) : _errorMsg(std::move(msg)) {}

            /**
             * @brief Récupère le message d'erreur.
             * @return const char* Message d'erreur en C-string.
             */
            char const *getMessage() const;

            /**
             * @brief Retourne le message d'erreur (override de std::exception).
             * @return const char* Message d'erreur en C-string.
             */
            const char *what() const throw();

           private:
            std::string _errorMsg; /**< Message d'erreur */
        };

        /**
         * @class InvalidArg
         * @brief Erreur spécifique pour arguments invalides.
         */
        class InvalidArg : public Error {
           public:
            /**
             * @brief Constructeur avec message d'erreur.
             * @param msg Message d'erreur.
             */
            InvalidArg(std::string msg) : Error(std::move(msg)) {}
        };

        /**
         * @class ServerConnection
         * @brief Erreur spécifique pour erreurs de connexion serveur.
         */
        class ServerConnection : public Error {
           public:
            /**
             * @brief Constructeur avec message d'erreur.
             * @param msg Message d'erreur.
             */
            ServerConnection(std::string msg) : Error(std::move(msg)) {}
        };

        /**
         * @class SocketError
         * @brief Erreur liée aux sockets.
         */
        class SocketError : public Error {
           public:
            /**
             * @brief Constructeur avec message d'erreur.
             * @param msg Message d'erreur.
             */
            SocketError(std::string msg) : Error(std::move(msg)) {}
        };

        /**
         * @class EggError
         * @brief Erreur liée aux œufs dans le jeu.
         */
        class EggError : public Error {
           public:
            /**
             * @brief Constructeur avec message d'erreur.
             * @param msg Message d'erreur.
             */
            EggError(std::string msg) : Error(std::move(msg)) {}
        };
    }  // namespace error
}  // namespace zappy
