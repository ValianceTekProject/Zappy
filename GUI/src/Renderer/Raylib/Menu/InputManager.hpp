/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** InputManager.hpp
*/

#pragma once

#include <raylib.h>

#include <unordered_map>
#include <vector>
#include <functional>

namespace zappy {
    namespace gui {
        namespace raylib {
            class InputManager {
                public:
                    /**
                     * @brief État possible d'une touche ou d'un bouton de souris.
                     */
                    enum class KeyState {
                        NONE,      ///< Aucun état détecté (pas d'interaction).
                        PRESSED,   ///< Touche ou bouton pressé ce frame.
                        HELD,      ///< Touche ou bouton maintenu.
                        RELEASED   ///< Touche ou bouton relâché ce frame.
                    };

                    /**
                     * @brief Met à jour les états des touches et de la souris.
                     */
                    void update();

                    /**
                     * @brief Récupère l'état d'une touche spécifique.
                     * @param key Code de la touche (ex : KEY_SPACE).
                     * @return État de la touche (PRESSED, HELD, etc.).
                     */
                    KeyState getKeyState(int key) const;

                    /**
                     * @brief Vérifie si une touche a été pressée ce frame.
                     * @param key Code de la touche.
                     * @return true si pressée, false sinon.
                     */
                    bool isKeyPressed(int key) const;

                    /**
                     * @brief Vérifie si une touche a été relâchée ce frame.
                     * @param key Code de la touche.
                     * @return true si relâchée, false sinon.
                     */
                    bool isKeyReleased(int key) const;

                    /**
                     * @brief Récupère l'état d'un bouton de souris.
                     * @param button Code du bouton (ex : MOUSE_LEFT_BUTTON).
                     * @return État du bouton.
                     */
                    KeyState getMouseButtonState(int button) const;

                    /**
                     * @brief Vérifie si un bouton de souris a été pressé ce frame.
                     * @param button Code du bouton.
                     * @return true si pressé, false sinon.
                     */
                    bool isMouseButtonPressed(int button) const;

                    /**
                     * @brief Récupère le déplacement de la souris depuis le dernier frame.
                     * @return Vecteur de déplacement.
                     */
                    Vector2 getMouseDelta() const { return _mouseDelta; }

                    /**
                     * @brief Récupère la dernière position connue de la souris.
                     * @return Position de la souris.
                     */
                    Vector2 getMousePosition() const { return _lastMousePosition; }

                private:
                    /**
                     * @brief Met à jour l’état d’un ensemble de touches ou boutons.
                     * @param states Référence vers la map d’état à mettre à jour.
                     * @param isDownFn Fonction qui indique si une touche/bouton est actuellement enfoncé.
                     */
                    void _updateState(
                        std::unordered_map<int, KeyState> &states,
                        const std::function<bool(int)> &isDownFn
                    );

                    /// États des touches clavier.
                    std::unordered_map<int, KeyState> _keyStates;
                    /// États des boutons de souris.
                    std::unordered_map<int, KeyState> _mouseButtonStates;

                    /// Dernière position enregistrée de la souris.
                    Vector2 _lastMousePosition = {0.0f, 0.0f};
                    /// Déplacement de la souris depuis la dernière frame.
                    Vector2 _mouseDelta = {0.0f, 0.0f};
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
