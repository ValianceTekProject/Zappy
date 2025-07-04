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

        enum class ClientState {
            WAITING_TEAM_NAME,
            CONNECTED,
            DISCONNECTED,
            UNDEFINED
        };

        class Client {
           public:
            Client(int socket)
                : _socket(socket), _state(ClientState::WAITING_TEAM_NAME)
            {
                this->queueMutex = std::make_unique<std::mutex>();
            };

            ~Client()
            {
                if (queueMutex) {
                    std::lock_guard<std::mutex> lock(*queueMutex);
                    while (!queueMessage.empty()) {
                        queueMessage.pop();
                    }
                }
            }

            int getSocket() const { return this->_socket; }

            ClientState getState() const { return this->_state; }

            void setState(ClientState state) { this->_state = state; }

            void sendMessage(const std::string &buf)
            {
                ssize_t bytesSent =
                    send(this->_socket, buf.c_str(), buf.size(), 0);
                (void)bytesSent;
            }

            std::queue<std::string> queueMessage;
            std::shared_ptr<std::mutex> queueMutex = nullptr;

           private:
            int _socket;
            ClientState _state;
        };
    }  // namespace server
}  // namespace zappy
