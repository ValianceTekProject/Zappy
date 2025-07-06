/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** Server
*/

#pragma once

#include <csignal>
#include <functional>
#include <iostream>
#include <mutex>
#include <netinet/in.h>
#include <poll.h>
#include <sstream>
#include <string>
#include <thread>
#include <vector>
#include <optional>

#include "Client/Client.hpp"
#include "SocketServer.hpp"
#include "Error/Error.hpp"
#include "Game.hpp"
#include "my_macros.hpp"
#include "Utils.hpp"
#include "TeamsGui.hpp"

namespace zappy {

    namespace server {

        /**
         * @class Server
         * @brief Classe principale gérant le serveur Zappy.
         *
         * Gère les connexions clients, le polling, la communication avec les clients,
         * ainsi que la boucle principale du serveur.
         */
        class Server {
           public:
            /**
             * @brief Constructeur du serveur, initialise avec les arguments passés en ligne de commande.
             * @param argc Nombre d'arguments.
             * @param argv Tableau des arguments.
             */
            Server(int argc, char const *argv[]);

            /**
             * @brief Destructeur par défaut.
             */
            ~Server() = default;

            /**
             * @brief Lance le serveur.
             */
            void runServer();

            /**
             * @brief Obtient la largeur de la carte.
             * @return La largeur.
             */
            int getWidth() const { return this->_width; }

            /**
             * @brief Obtient la hauteur de la carte.
             * @return La hauteur.
             */
            int getHeight() const { return this->_height; }

            /**
             * @brief Obtient le nombre maximal de clients.
             * @return Le nombre de clients.
             */
            int getClientNb() const { return this->_clientNb; }

            /**
             * @brief Obtient la fréquence du serveur.
             * @return La fréquence.
             */
            int getFreq() const { return this->_freq; }

            /**
             * @brief Lance la boucle principale du serveur.
             */
            void runLoop();

            /**
             * @brief Effectue une boucle de gestion des événements poll.
             */
            void pfdLoop();

            /**
             * @brief Traite un message reçu d'un client.
             * @param clientSocket Le socket du client.
             * @param buffer Le contenu du message.
             */
            void handleClientMessage(int clientSocket, std::string buffer);

            /**
             * @brief Attache un observateur au serveur.
             * @param observer Observateur à attacher.
             */
            void attachObserver(std::shared_ptr<zappy::observer::IObserver> observer);

            /**
             * @brief Notifie tous les observateurs d'un signal.
             * @param sig Signal à transmettre.
             */
            void notifyObservers(int sig);

            /**
             * @brief Définit l'état de fonctionnement du serveur.
             * @param state Nouvel état.
             */
            void setRunningState(RunningState state) { _serverRun = state; }

            /**
             * @brief Vide la liste des équipes.
             */
            void clearTeams() { _teamList.clear(); }

            /**
             * @brief Envoie un message à un client via son socket.
             * @param buf Le message à envoyer.
             * @param socket Le socket client.
             */
            void sendMessage(const std::string &buf, int socket) { send(socket, buf.c_str(), buf.size(), 0); }

            /**
             * @brief Recherche un joueur par son socket.
             * @param socket Le socket du client.
             * @return Un optional contenant le joueur si trouvé, sinon vide.
             */
            std::optional<std::shared_ptr<zappy::game::ServerPlayer>> getPlayerBySocket(const int &socket);

           private:
            std::vector<std::shared_ptr<zappy::observer::IObserver>> _observers; ///< Liste des observateurs.
            std::unique_ptr<zappy::game::Game> _game = nullptr;                   ///< Instance du jeu.
            std::unique_ptr<server::SocketServer> _socket = nullptr;             ///< Instance du serveur socket.

            RunningState _serverRun = RunningState::RUN;                         ///< État de fonctionnement du serveur.

            std::vector<struct pollfd> _fds;                                     ///< Liste des pollfd.

            std::vector<std::shared_ptr<zappy::game::ITeams>> _teamList;         ///< Liste des équipes.
            std::unordered_map<std::string, std::function<void(int)>> _flags;    ///< Flags et leurs fonctions associées.

            std::mutex _socketLock;                                              ///< Mutex pour accès socket.
            std::mutex _endLock;                                                 ///< Mutex pour fin de boucle.

            int _port = noValue;                                                 ///< Port d'écoute.
            int _width = noValue;                                                ///< Largeur de la carte.
            int _height = noValue;                                               ///< Hauteur de la carte.
            int _clientNb = noValue;                                             ///< Nombre maximal de clients.
            int _freq = noValue;                                                 ///< Fréquence du serveur.
            std::vector<std::string> _namesTeam;                                ///< Noms des équipes.

            /**
             * @brief Analyse les flags passés en arguments.
             * @param argc Nombre d'arguments.
             * @param argv Tableau des arguments.
             */
            void _parseFlags(int argc, char const *argv[]);

            /**
             * @brief Analyse un nom d'équipe.
             * @param index Index courant dans argv.
             * @param argv Tableau des arguments.
             */
            void _parseName(int &index, char const *argv[]);

            /**
             * @brief Analyse un flag avec une valeur entière.
             * @param index Index courant dans argv.
             * @param arg Le flag.
             * @param value La valeur associée.
             */
            void _parseFlagsInt(int &index, std::string arg, std::string value);

            /**
             * @brief Vérifie que tous les paramètres sont corrects.
             */
            void _checkParams();

            /**
             * @brief Gère une nouvelle connexion si elle est détectée sur un pollfd.
             * @param pfd Le pollfd à vérifier.
             * @return true si une nouvelle connexion a été acceptée, false sinon.
             */
            bool _handleNewConnection(struct pollfd &pfd);

            /**
             * @brief Récupère la commande envoyée par un client.
             * @param fd Le pollfd du client.
             * @return La commande reçue.
             */
            std::string _getClientCommand(const struct pollfd &fd);

            /**
             * @brief Gère la déconnexion d'un client.
             * @param content Contenu reçu.
             * @param fd Le pollfd du client.
             * @return L'état du client après traitement.
             */
            ClientState _handleClientDisconnection(const std::string &content, struct pollfd &fd);

            /**
             * @brief Gère la commande envoyée par un client.
             * @param command La commande reçue.
             * @param pfd Le pollfd du client.
             */
            void _handleClientCommand(const std::string &command, struct pollfd &pfd);

            /**
             * @brief Ajoute un joueur à une équipe après connexion.
             * @param team L'équipe concernée.
             * @param pfd Le pollfd du client.
             */
            void _playerConnect(std::shared_ptr<zappy::game::ITeams> &team, struct pollfd &pfd);

            /**
             * @brief Gère la connexion d'une interface graphique.
             * @param team L'équipe GUI.
             */
            void _guiConnect(std::shared_ptr<zappy::game::ITeams> &team);

            /**
             * @brief Envoie les commandes initiales à la GUI.
             * @param teamsGui Instance de TeamsGui.
             */
            void _initialCommandGui(std::shared_ptr<zappy::game::TeamsGui> &teamsGui);

        };

    }  // namespace server
}  // namespace zappy
