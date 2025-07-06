/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameState.hpp
*/

#pragma once

#include "Map.hpp"
#include "Player.hpp"

#include <memory>

namespace zappy {
    namespace game {
        using EggList = std::vector<std::reference_wrapper<Egg>>;
        using PlayerList = std::vector<std::reference_wrapper<Player>>;

        class GameState {
            public:
                /**
                 * @brief Constructeur par défaut. Initialise la fréquence à 100.
                 */
                GameState() : _frequency(100) {}

                /**
                 * @brief Destructeur par défaut.
                 */
                ~GameState() = default;

                /**
                 * @brief Définit la fréquence du serveur.
                 * @param frequency Nouvelle fréquence.
                 */
                void setFrequency(const size_t &frequency) { this->_frequency = frequency; }
                /**
                 * @brief Initialise la carte du jeu avec les dimensions données.
                 * @param width Largeur de la carte.
                 * @param height Hauteur de la carte.
                 */
                void initMap(const size_t &width, const size_t &height) { this->_map = std::make_shared<Map>(width, height); }

                /**
                 * @brief Récupère la fréquence du serveur.
                 * @return Fréquence actuelle.
                 */
                size_t getFrequency() const { return this->_frequency; }

                /**
                 * @brief Récupère la carte actuelle.
                 * @return Carte partagée (modifiable).
                 */
                std::shared_ptr<Map> getMap() { return this->_map; }
                /**
                 * @brief Récupère la carte actuelle (version constante).
                 * @return Carte partagée (const).
                 */
                const std::shared_ptr<Map> &getMap() const { return this->_map; }
                /**
                 * @brief Récupère une case spécifique de la carte.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @return Référence constante à la case demandée.
                 */
                const Tile &getTile(const size_t &x, const size_t &y) const { return this->_map->getTile(x, y); }

                /**
                 * @brief Récupère la liste des équipes connues.
                 * @return Référence vers le vecteur des noms d'équipes.
                 */
                const std::vector<std::string> &getTeams() const { return this->_teams; }
                /**
                 * @brief Récupère la liste des œufs.
                 * @return Référence vers le vecteur des œufs.
                 */
                const std::vector<Egg> &getEggs() const { return this->_eggs; }
                /**
                 * @brief Récupère la liste des joueurs.
                 * @return Référence vers le vecteur des joueurs.
                 */
                const std::vector<Player> &getPlayers() const { return this->_players; }

                /**
                 * @brief Met à jour une case de la carte.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @param tile Nouvelle case.
                 */
                void updateTile(const size_t &x, const size_t &y, Tile &tile) { this->_map->setTile(x, y, tile); }

                /**
                 * @brief Ajoute une équipe au jeu.
                 * @param teamName Nom de l'équipe.
                 */
                void addTeam(const std::string &teamName) { this->_teams.push_back(teamName); }

                /**
                 * @brief Ajoute un œuf au jeu.
                 * @param eggId Identifiant de l'œuf.
                 * @param fatherId Identifiant du joueur père.
                 * @param x Position x.
                 * @param y Position y.
                 */
                void addEgg(const int &eggId, const int &fatherId, const int &x, const int &y)
                    { this->_eggs.push_back(Egg(eggId, fatherId, x, y)); }

                /**
                 * @brief Ajoute un joueur au jeu.
                 * @param player Joueur à ajouter.
                 */
                void addPlayer(const Player &player) { this->_players.push_back(player); }

                /**
                 * @brief Met à jour la position et l’orientation d’un joueur.
                 * @param id Identifiant du joueur.
                 * @param x Nouvelle position x.
                 * @param y Nouvelle position y.
                 * @param orientation Nouvelle orientation.
                 */
                void updatePlayerPosition(const int &id, const int &x, const int &y, const Orientation &orientation);

                /**
                 * @brief Met à jour le niveau d’un joueur.
                 * @param id Identifiant du joueur.
                 * @param level Nouveau niveau.
                 */
                void updatePlayerLevel(const int &id, const size_t &level);

                /**
                 * @brief Met à jour l’inventaire d’un joueur.
                 * @param id Identifiant du joueur.
                 * @param inventory Nouvel inventaire.
                 */
                void updatePlayerInventory(const int &id, const Inventory &inventory);

                /**
                 * @brief Gère l’expulsion d’un joueur.
                 * @param id Identifiant du joueur.
                 */
                void playerExpulsion(const int &id);

                /**
                 * @brief Gère un message de broadcast émis par un joueur.
                 * @param id Identifiant du joueur.
                 * @param message Message émis.
                 */
                void playerBroadcast(const int &id, const std::string &message);

                /**
                 * @brief Démarre une incantation.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @param level Niveau d'incantation.
                 * @param playerIds Liste des joueurs participants.
                 */
                void startIncantation(const int &x, const int &y, const int &level, const std::vector<int> &playerIds);

                /**
                 * @brief Termine une incantation.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @param result Résultat de l'incantation (true = succès).
                 */
                void endIncantation(const int &x, const int &y, const bool &result);

                /**
                 * @brief Fait éclore un œuf.
                 * @param eggId Identifiant de l’œuf.
                 */
                void hatchEgg(const int &eggId);

                /**
                 * @brief Supprime un œuf.
                 * @param eggId Identifiant de l’œuf.
                 */
                void removeEgg(const int &eggId);

                /**
                 * @brief Supprime un joueur.
                 * @param id Identifiant du joueur.
                 */
                void removePlayer(const int &id);

                /**
                 * @brief Récupère une référence à un œuf par son identifiant.
                 * @param eggId Identifiant de l’œuf.
                 * @return Référence vers l’œuf.
                 */
                Egg &getEggById(const int &eggId);

                /**
                 * @brief Récupère une référence constante à un œuf par son identifiant.
                 * @param eggId Identifiant de l’œuf.
                 * @return Référence constante vers l’œuf.
                 */
                const Egg &getEggById(const int &eggId) const;

                /**
                 * @brief Récupère une référence à un joueur par son identifiant.
                 * @param id Identifiant du joueur.
                 * @return Référence vers le joueur.
                 */
                Player &getPlayerById(const int &id);

                /**
                 * @brief Récupère une référence constante à un joueur par son identifiant.
                 * @param id Identifiant du joueur.
                 * @return Référence constante vers le joueur.
                 */
                const Player &getPlayerById(const int &id) const;

                /**
                 * @brief Récupère les œufs situés à une position donnée.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @return Liste de références vers les œufs.
                 */
                std::vector<std::reference_wrapper<Egg>> getEggsByCoord(const int &x, const int &y);

                /**
                 * @brief Récupère les œufs situés à une position donnée (const).
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @return Liste de références constantes vers les œufs.
                 */
                std::vector<std::reference_wrapper<const Egg>> getEggsByCoord(const int &x, const int &y) const;

                /**
                 * @brief Récupère les joueurs situés à une position donnée.
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @return Liste de références vers les joueurs.
                 */
                std::vector<std::reference_wrapper<Player>> getPlayersByCoord(const int &x, const int &y);

                /**
                 * @brief Récupère les joueurs situés à une position donnée (const).
                 * @param x Coordonnée x.
                 * @param y Coordonnée y.
                 * @return Liste de références constantes vers les joueurs.
                 */
                std::vector<std::reference_wrapper<const Player>> getPlayersByCoord(const int &x, const int &y) const;

                /**
                 * @brief Termine la partie avec l’équipe gagnante.
                 * @param teamName Nom de l’équipe victorieuse.
                 */
                void endGame(const std::string &teamName);

            private:
                size_t _frequency; ///< Fréquence d’actualisation du serveur.

                std::vector<Egg> _eggs; ///< Liste des œufs.
                std::vector<Player> _players; ///< Liste des joueurs.
                std::vector<std::string> _teams; ///< Liste des noms d’équipes.

                std::shared_ptr<game::Map> _map; ///< Carte du jeu.
        };

    } // namespace game
} // namespace zappy
