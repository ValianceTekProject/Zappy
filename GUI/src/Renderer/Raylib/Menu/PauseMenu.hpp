/*
** EPITECH PROJECT, 2025
** zappy
** File description:
** PauseMenu.hpp
*/

#pragma once
#include "InputManager.hpp"
#include "MenuMacros.hpp"
#include "AssetPaths.hpp"

#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            class PauseMenu {
                public:
                    /**
                     * @brief Constructeur de PauseMenu.
                     */
                    PauseMenu();

                    /**
                     * @brief Destructeur de PauseMenu.
                     */
                    ~PauseMenu();

                    /**
                     * @brief Initialise le menu pause (textures, couleurs, thèmes...).
                     */
                    void init();

                    /**
                     * @brief Gère les entrées utilisateur.
                     * @param inputManager Référence vers le gestionnaire d'input.
                     */
                    void handleInput(const InputManager &inputManager);

                    /**
                     * @brief Met à jour l’état du menu pause.
                     */
                    void update();

                    /**
                     * @brief Vérifie si un changement de scène est demandé.
                     * @return true si un changement est requis, false sinon.
                     */
                    bool shouldChangeScene();

                    /**
                     * @brief Retourne le type de scène vers laquelle basculer.
                     * @return Type de la scène (jeu, menu principal, etc.).
                     */
                    SceneType getSceneType() const { return _sceneType; }

                    /**
                     * @brief Affiche le menu pause ou le sous-menu des thèmes à l’écran.
                     */
                    void render() const;

                    /**
                     * @brief Indique si le menu pause est actuellement actif.
                     * @return true si actif, false sinon.
                     */
                    bool isActive() const { return _display; }

                private:
                    /**
                     * @brief Structure représentant un thème disponible dans le menu pause.
                     */
                    struct Theme {
                        std::string name;   ///< Nom du thème.
                        Texture2D texture;  ///< Texture associée au thème.
                    };

                    /// Taille de la police utilisée pour l’affichage.
                    int _fontSize;
                    /// Couleur du texte.
                    Color _textColor;
                    /// Couleur du cadre.
                    Color _boxColor;
                    /// Couleur de sélection active.
                    Color _selectedColor;
                    /// Couleur normale (non sélectionnée).
                    Color _normalColor;

                    /// Indique si le menu est affiché.
                    bool _display;
                    /// Indique si une scène doit être changée.
                    bool _shouldChangeScene;
                    /// Scène cible à charger.
                    SceneType _sceneType;

                    /// Dernière touche utilisée.
                    int _key;
                    /// Bouton actuellement sélectionné.
                    int _selectedButton;
                    /// Thème actuellement sélectionné.
                    int _selectedTheme;

                    /// État courant du menu pause.
                    PauseMenuState _menuState;
                    /// Liste des thèmes disponibles.
                    std::vector<Theme> _themes;

                    /// Indique si le sous-menu des thèmes est actif.
                    bool _isThemeMenu;

                    /// Index du thème actuellement utilisé.
                    int _currentThemeIndex;

                    /**
                     * @brief Calcule un facteur d’échelle uniforme basé sur la résolution.
                     * @return Facteur d’échelle.
                     */
                    float _getUniformScale() const;

                    /**
                     * @brief Affiche le menu pause principal.
                     * @param scale Facteur d’échelle.
                     * @param x Position X de base.
                     * @param y Position Y de base.
                     * @param w Largeur du cadre.
                     * @param h Hauteur du cadre.
                     */
                    void _renderPauseMenu(float scale, int x, int y, int w, int h) const;

                    /**
                     * @brief Affiche le sous-menu des thèmes disponibles.
                     * @param scale Facteur d’échelle.
                     * @param x Position X de base.
                     * @param y Position Y de base.
                     * @param w Largeur du cadre.
                     * @param h Hauteur du cadre.
                     */
                    void _renderThemeMenu(float scale, int x, int y, int w, int h) const;

                    /**
                     * @brief Affiche un thème dans le menu des thèmes.
                     * @param theme Thème à afficher.
                     * @param x Position X.
                     * @param y Position Y.
                     * @param width Largeur du cadre.
                     * @param height Hauteur du cadre.
                     * @param selected true si le thème est sélectionné, false sinon.
                     */
                    void _renderTheme(const Theme& theme, int x, int y, int width, int height, bool selected) const;
            };
        }
    }
}
