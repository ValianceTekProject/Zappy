//
// EPITECH PROJECT, 2025
// Player
// File description:
// Player
//

#pragma once

#include <cstddef>
#include <cstring>
#include <functional>
#include <memory>
#include <mutex>
#include <queue>
#include <string>
#include <sys/socket.h>
#include <unistd.h>
#include <vector>

#include "Inventory.hpp"
#include "my_macros.hpp"

namespace zappy {
    namespace server {

        /**
         * @enum ClientState
         * @brief Représente l'état d'un client connecté au serveur.
         */
        enum class ClientState {
            WAITING_TEAM_NAME,   /**< En attente du nom de l'équipe */
            CONNECTED,           /**< Client connecté */
            DISCONNECTED,        /**< Client déconnecté */
            UNDEFINED            /**< État indéfini */
        };

        /**
         * @class Client
         * @brief Représente un client connecté au serveur.
         */
        class Client {
           public:
            /**
             * @brief Constructeur avec socket.
             * @param socket Descripteur du socket du client.
             */
            Client(int socket)
                : _socket(socket), _state(ClientState::WAITING_TEAM_NAME)
            {
                this->queueMutex = std::make_unique<std::mutex>();
            };

            /**
             * @brief Destructeur.
             * Vide la file de messages en protégeant l'accès via un mutex.
             */
            ~Client()
            {
                if (queueMutex) {
                    std::lock_guard<std::mutex> lock(*queueMutex);
                    while (!queueMessage.empty()) {
                        queueMessage.pop();
                    }
                }
            }

            /**
             * @brief Obtient le socket du client.
             * @return int Descripteur du socket.
             */
            int getSocket() const { return this->_socket; }

            /**
             * @brief Obtient l'état actuel du client.
             * @return ClientState État du client.
             */
            ClientState getState() const { return this->_state; }

            /**
             * @brief Définit l'état du client.
             * @param state Nouvel état.
             */
            void setState(ClientState state) { this->_state = state; }

            /**
             * @brief Envoie un message au client via le socket.
             * @param buf Message à envoyer.
             */
            void sendMessage(const std::string &buf)
            {
                ssize_t bytesSent =
                    send(this->_socket, buf.c_str(), buf.size(), 0);
                (void)bytesSent;
            }

            /// File des messages en attente d'envoi
            std::queue<std::string> queueMessage;

            /// Mutex pour protéger l'accès à la file de messages
            std::shared_ptr<std::mutex> queueMutex = nullptr;

           private:
            int _socket;              ///< Socket du client
            ClientState _state;       ///< État actuel du client
        };
    }  // namespace server
}  // namespace zappy
