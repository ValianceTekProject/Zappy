/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** GameMenu.hpp
*/

#pragma once

#include "InputManager.hpp"
#include "GameState.hpp"
#include "RendererError.hpp"
#include "MenuMacros.hpp"

#include <memory>
#include <map>
#include <list>

namespace zappy {
    namespace gui{
        namespace raylib {
            class GameMenu {
                public:
                    /**
                     * @brief Constructeur de GameMenu.
                     * @param gameState Pointeur partagé vers l'état du jeu.
                     */
                    GameMenu(const std::shared_ptr<game::GameState> &gameState);

                    /**
                     * @brief Destructeur par défaut.
                     */
                    ~GameMenu() = default;

                    /**
                     * @brief Initialise les composants du menu.
                     */
                    void init();

                    /**
                     * @brief Définit la fréquence actuelle du serveur.
                     * @param frequency Nouvelle fréquence.
                     */
                    void setFrequency(const size_t &frequency) { this->_frequency = frequency; }

                    /**
                     * @brief Récupère la fréquence actuelle.
                     * @return Fréquence actuelle.
                     */
                    size_t getFrequency() const { return this->_frequency; }

                    /**
                     * @brief Vérifie si la fréquence a été modifiée depuis le dernier appel.
                     * @return true si modifiée, false sinon.
                     */
                    bool hasFrequencyChanged();

                    /**
                     * @brief Gère les entrées utilisateur.
                     * @param inputManager Gestionnaire d'input.
                     */
                    void handleInput(const InputManager &inputManager);

                    /**
                     * @brief Met à jour l'état du menu.
                     */
                    void update();

                    /**
                     * @brief Affiche le menu à l'écran.
                     */
                    void render() const;

                    /**
                     * @brief Ajoute un joueur à la liste des joueurs affichés.
                     * @param id Identifiant du joueur.
                     */
                    void addPlayer(const int &id);

                    /**
                     * @brief Affiche un message de broadcast reçu par un joueur.
                     * @param id ID du joueur.
                     * @param message Message reçu.
                     * @param playerTeam Nom de l'équipe du joueur.
                     */
                    void playerBroadcast(const int &id, const std::string &message, const std::string &playerTeam);

                    /**
                     * @brief Supprime un joueur du menu.
                     * @param id ID du joueur à retirer.
                     */
                    void removePlayer(const int &id);

                private:
                    /**
                     * @brief Décrypte le message broadcast d’un joueur.
                     * @param id ID du joueur.
                     * @param message Message chiffré.
                     * @param playerTeam Équipe du joueur.
                     * @return Message déchiffré et affichable.
                     */
                    std::string _decryptBroadcast(const int &id, const std::string &message, const std::string &playerTeam);

                    /**
                     * @brief Gère les inputs liés à la fréquence.
                     * @param inputManager Gestionnaire d'input.
                     */
                    void _handleFreqInput(const InputManager &inputManager);

                    /**
                     * @brief Gère les inputs liés aux joueurs affichés.
                     * @param inputManager Gestionnaire d'input.
                     */
                    void _handlePlayersInput(const InputManager &inputManager);

                    /**
                     * @brief Affiche l'aide pour afficher/masquer les infos.
                     */
                    void _renderShowHideHelp(const int &screenWidth, const int &screenHeight) const;

                    /**
                     * @brief Affiche les messages broadcast.
                     */
                    void _renderBroadcasts(const int &screenWidth, const int &screenHeight) const;

                    /**
                     * @brief Affiche l’aide du menu.
                     */
                    void _renderHelp(const int &screenWidth, const int &screenHeight) const;

                    /**
                     * @brief Affiche et contrôle la fréquence.
                     */
                    void _renderFreq(const int &screenWidth, const int &screenHeight) const;

                    /**
                     * @brief Affiche les infos de tous les joueurs sélectionnés.
                     */
                    void _renderPlayersInfos(const int &screenWidth, const int &screenHeight) const;

                    /**
                     * @brief Affiche les infos d’un joueur individuel.
                     * @param playerId ID du joueur.
                     * @param x Position X du cadre.
                     * @param y Position Y du cadre.
                     * @param textSize Taille du texte.
                     * @param spacingY Espacement vertical entre les lignes.
                     * @param boxWidth Largeur du cadre.
                     * @param boxHeight Hauteur du cadre.
                     */
                    void _renderPlayerInfo(
                        const int &playerId,
                        const int &x, const int &y,
                        const int &textSize,
                        const int &spacingY,
                        const int &boxWidth,
                        const int &boxHeight
                    ) const;

                    /// Pointeur partagé vers l'état du jeu.
                    const std::shared_ptr<game::GameState> _gameState;

                    /// Taille de police utilisée dans le menu.
                    int _fontSize;
                    /// Couleur du texte.
                    Color _textColor;
                    /// Couleur des cadres de fond.
                    Color _boxColor;

                    /// Fréquence actuelle du jeu.
                    size_t _frequency;
                    /// Indique si la fréquence a été modifiée.
                    bool _freqChanged;

                    /// Liste des IDs des joueurs suivis.
                    std::vector<int> _playersIds;
                    /// Index du premier joueur affiché.
                    int _displayedPlayersIndex;
                    /// Nombre de joueurs affichés en même temps.
                    int _numberPlayerDisplayed;

                    /// Largeur des cadres d’infos joueur.
                    int _playerBoxWidth;

                    /// Historique des messages broadcast.
                    std::list<std::string> _broadcasts;

                    /// Section du menu actuellement modifiée.
                    MenuModifiedSection _modifiedSection;
                    /// Gestionnaires associés aux sections modifiables.
                    std::map<MenuModifiedSection, void (GameMenu::*)(const InputManager&)> _modifiedSectionHandlers;

                    /// Affiche toutes les infos si activé.
                    bool _displayAll;
                    /// Raccourcis clavier pour activer/désactiver les parties du menu.
                    std::map<GameMenuState, int> _menuStatesKeys;
                    /// État actif/inactif des différentes sections du menu.
                    std::map<GameMenuState, bool> _menuStates;
                    /// Fonctions d'affichage des sections.
                    std::map<GameMenuState, void (GameMenu::*)(const int&, const int&) const> _menuRenderFunctions;

                    /// Modificateurs de fréquence liés aux touches.
                    const std::map<int, int> _frequencyKeyModifiers = {
                        { KEY_UP, 1 },
                        { KEY_DOWN, -1 },
                        { KEY_LEFT, -10 },
                        { KEY_RIGHT, 10 }
                    };
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
