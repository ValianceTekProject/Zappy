/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IModel.hpp
*/

#pragma once

#include "AssetPaths.hpp"

#include <iostream>
#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {

            /**
             * @brief Interface représentant un modèle 3D affichable dans la scène.
             */
            class IModel {
                public:
                    /**
                     * @brief Destructeur virtuel par défaut.
                     */
                    virtual ~IModel() = default;

                    /**
                     * @brief Initialise le modèle.
                     */
                    virtual void init() = 0;

                    // Setters

                    /**
                     * @brief Définit la position du modèle.
                     * @param position Vecteur de position dans l'espace.
                     */
                    virtual void setPosition(const Vector3 &position) = 0;

                    /**
                     * @brief Définit l'échelle du modèle.
                     * @param scale Facteur d'échelle.
                     */
                    virtual void setScale(const float &scale) = 0;

                    /**
                     * @brief Définit la rotation du modèle.
                     * @param rotation Vecteur représentant la rotation (pitch, yaw, roll).
                     */
                    virtual void setRotation(const Vector3 &rotation) = 0;

                    /**
                     * @brief Définit la couleur du modèle.
                     * @param color Couleur à appliquer au modèle.
                     */
                    virtual void setColor(const Color &color) = 0;

                    // Getters

                    /**
                     * @brief Récupère la position actuelle du modèle.
                     * @return Vecteur de position.
                     */
                    virtual Vector3 getPosition() const = 0;

                    /**
                     * @brief Récupère l'échelle actuelle du modèle.
                     * @return Valeur d'échelle.
                     */
                    virtual float getScale() const = 0;

                    /**
                     * @brief Récupère la rotation actuelle du modèle.
                     * @return Vecteur de rotation.
                     */
                    virtual Vector3 getRotation() const = 0;

                    /**
                     * @brief Récupère la couleur actuelle du modèle.
                     * @return Couleur appliquée au modèle.
                     */
                    virtual Color getColor() const = 0;

                    /**
                     * @brief Met à jour l'état du modèle (par exemple animation ou déplacement).
                     * @param deltaUnits Temps ou facteur de déplacement depuis la dernière mise à jour.
                     */
                    virtual void update(const float &deltaUnits) = 0;

                    /**
                     * @brief Applique une échelle supplémentaire au modèle.
                     * @param scale Facteur d'échelle à appliquer.
                     */
                    virtual void scale(const float &scale) = 0;

                    /**
                     * @brief Déplace le modèle.
                     * @param translation Vecteur de translation.
                     */
                    virtual void translate(const Vector3 &translation) = 0;

                    /**
                     * @brief Applique une rotation supplémentaire au modèle.
                     * @param rotation Vecteur de rotation.
                     */
                    virtual void rotate(const Vector3 &rotation) = 0;

                    /**
                     * @brief Affiche le modèle à l'écran.
                     */
                    virtual void render() = 0;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
