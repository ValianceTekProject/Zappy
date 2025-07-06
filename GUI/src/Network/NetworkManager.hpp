/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** NetworkManager.hpp
*/

#pragma once

#include "NetworkError.hpp"

#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <poll.h>
#include <unistd.h>

#include <memory>
#include <functional>
#include <sstream>
#include <queue>
#include <mutex>
#include <cstring>

namespace zappy {
    namespace network {

        /**
         * @brief Représente un message reçu depuis le serveur.
         */
        struct ServerMessage {
            std::string command; ///< Commande extraite du message.
            std::string params;  ///< Paramètres associés à la commande.
            std::string raw;     ///< Message brut tel que reçu.
        };

        class NetworkManager {
            public:
                /**
                 * @brief Constructeur par défaut.
                 */
                NetworkManager();

                /**
                 * @brief Destructeur. Ferme la connexion si active.
                 */
                ~NetworkManager();

                /**
                 * @brief Établit une connexion au serveur.
                 * 
                 * @param host Adresse IP ou nom d’hôte du serveur.
                 * @param port Port de connexion.
                 * @return true si la connexion a réussi, false sinon.
                 */
                bool connect(const std::string &host, int port);

                /**
                 * @brief Ferme la connexion avec le serveur.
                 */
                void disconnect();

                /**
                 * @brief Vérifie si une connexion est active.
                 * @return true si connecté, false sinon.
                 */
                bool isConnected() const;

                /**
                 * @brief Envoie une commande brute au serveur.
                 * 
                 * @param command Chaîne représentant la commande à envoyer.
                 * @return true si l’envoi a réussi, false sinon.
                 */
                bool sendCommand(const std::string &command);

                /**
                 * @brief Récupère les messages en attente du serveur.
                 * 
                 * @return Un vecteur de messages ServerMessage reçus.
                 */
                std::vector<ServerMessage> receiveMessages();

                /**
                 * @brief Définit une fonction de rappel appelée à chaque réception de message.
                 * 
                 * @param callback Fonction prenant un `ServerMessage` en paramètre.
                 */
                void setMessageCallback(std::function<void(const ServerMessage &)> callback);

            private:
                int _socket; ///< Descripteur de socket.
                bool _connected; ///< Indique si une connexion est active.
                std::string _buffer; ///< Tampon pour les données brutes reçues.
                std::queue<ServerMessage> _messageQueue; ///< File des messages à traiter.
                std::function<void(const ServerMessage &)> _messageCallback; ///< Callback de message.
                mutable std::mutex _mutex; ///< Mutex pour la synchronisation des accès concurrents.

                /**
                 * @brief Parse un message brut en structure ServerMessage.
                 * 
                 * @param raw Message brut à analyser.
                 * @return Structure ServerMessage extraite.
                 */
                ServerMessage parseMessage(const std::string &raw);

                /**
                 * @brief Traite le tampon interne pour extraire les messages complets.
                 */
                void processBuffer();
        };

    } // namespace network
} // namespace zappy
