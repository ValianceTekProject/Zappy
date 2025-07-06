/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IScene.hpp
*/

#pragma once

#include "IFloor.hpp"
#include "InputManager.hpp"

#include "GameState.hpp"

#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            /**
             * @brief Interface représentant une scène dans le GUI.
             */
            class IScene
            {
                public:
                    /**
                     * @brief Destructeur virtuel par défaut.
                     */
                    virtual ~IScene() = default;

                    /**
                     * @brief Initialise la scène.
                     */
                    virtual void init() = 0;

                    /**
                     * @brief Récupère une référence modifiable vers la caméra.
                     * 
                     * @return Référence modifiable vers la caméra.
                     */
                    virtual Camera &getCamera() = 0;

                    /**
                     * @brief Récupère une référence constante vers la caméra.
                     * 
                     * @return Référence constante vers la caméra.
                     */
                    virtual const Camera &getCamera() const = 0;

                    /**
                     * @brief Gère les entrées utilisateur.
                     * 
                     * @param inputManager Gestionnaire des entrées utilisateur.
                     */
                    virtual void handleInput(InputManager &inputManager) = 0;

                    /**
                     * @brief Met à jour l'état de la scène.
                     */
                    virtual void update() = 0;

                    /**
                     * @brief Rendu de la scène.
                     */
                    virtual void render() const = 0;

                    /**
                     * @brief Indique si la scène doit se fermer.
                     * 
                     * @return true si la scène doit se fermer, sinon false.
                     */
                    virtual bool shouldClose() const = 0;

                    /**
                     * @brief Ajoute un œuf à la scène.
                     * 
                     * @param eggId Identifiant de l'œuf.
                     */
                    virtual void addEgg(const int &eggId) = 0;

                    /**
                     * @brief Ajoute un joueur à la scène.
                     * 
                     * @param id Identifiant du joueur.
                     */
                    virtual void addPlayer(const int &id) = 0;

                    /**
                     * @brief Met à jour la position et l'orientation d'un joueur.
                     * 
                     * @param id Identifiant du joueur.
                     * @param x Nouvelle position X.
                     * @param y Nouvelle position Y.
                     * @param orientation Nouvelle orientation.
                     */
                    virtual void updatePlayerPosition(const int &id, const int &x, const int &y, const game::Orientation &orientation) = 0;

                    /**
                     * @brief Déclenche une expulsion de joueur.
                     * 
                     * @param id Identifiant du joueur.
                     */
                    virtual void playerExpulsion(const int &id) = 0;

                    /**
                     * @brief Envoie un message de broadcast d'un joueur.
                     * 
                     * @param id Identifiant du joueur.
                     * @param message Message diffusé.
                     */
                    virtual void playerBroadcast(const int &id, const std::string &message) = 0;

                    /**
                     * @brief Démarre une incantation.
                     * 
                     * @param x Position X de l'incantation.
                     * @param y Position Y de l'incantation.
                     * @param level Niveau de l'incantation.
                     * @param playerIds Liste des identifiants des joueurs participants.
                     */
                    virtual void startIncantation(
                        const int &x, const int &y,
                        const int &level,
                        const std::vector<int> &playerIds
                    ) = 0;

                    /**
                     * @brief Termine une incantation.
                     * 
                     * @param x Position X de l'incantation.
                     * @param y Position Y de l'incantation.
                     * @param result Résultat de l'incantation (true si réussie).
                     */
                    virtual void endIncantation(const int &x, const int &y, const bool &result) = 0;

                    /**
                     * @brief Fait éclore un œuf.
                     * 
                     * @param eggId Identifiant de l'œuf.
                     */
                    virtual void hatchEgg(const int &eggId) = 0;

                    /**
                     * @brief Supprime un œuf.
                     * 
                     * @param eggId Identifiant de l'œuf.
                     */
                    virtual void removeEgg(const int &eggId) = 0;

                    /**
                     * @brief Supprime un joueur.
                     * 
                     * @param id Identifiant du joueur.
                     */
                    virtual void removePlayer(const int &id) = 0;

                    /**
                     * @brief Termine la partie.
                     * 
                     * @param teamName Nom de l'équipe gagnante.
                     */
                    virtual void endGame(const std::string &teamName) = 0;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
