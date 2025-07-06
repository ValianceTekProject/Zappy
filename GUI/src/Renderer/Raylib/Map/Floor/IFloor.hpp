/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** IFloor.hpp
*/

#pragma once

#include "Orientation.hpp"
#include "Map.hpp"
#include "AssetPaths.hpp"
#include "APlayerModel.hpp"
#include "Movement.hpp"

#include <raylib.h>

namespace zappy {
    namespace gui {
        namespace raylib {
            class IFloor {
                public:
                    /**
                     * @brief Destructeur virtuel par défaut.
                     */
                    virtual ~IFloor() = default;

                    /**
                     * @brief Initialise les ressources nécessaires au sol.
                     */
                    virtual void init() = 0;

                    /**
                     * @brief Met à jour les données du sol (si nécessaire).
                     */
                    virtual void update() const = 0;

                    /**
                     * @brief Affiche le sol à l'écran.
                     */
                    virtual void render() const = 0;

                    /**
                     * @brief Définit la largeur de la carte.
                     * @param width Largeur (en tuiles).
                     */
                    virtual void setWidth(const size_t &width) = 0;

                    /**
                     * @brief Définit la hauteur de la carte.
                     * @param height Hauteur (en tuiles).
                     */
                    virtual void setHeight(const size_t &height) = 0;

                    /**
                     * @brief Définit la taille d'une tuile.
                     * @param tileSize Taille d'une tuile en unités.
                     */
                    virtual void setTileSize(const float &tileSize) = 0;

                    /**
                     * @brief Récupère la largeur de la carte.
                     * @return Largeur (en tuiles).
                     */
                    virtual size_t getWidth() const = 0;

                    /**
                     * @brief Récupère la hauteur de la carte.
                     * @return Hauteur (en tuiles).
                     */
                    virtual size_t getHeight() const = 0;

                    /**
                     * @brief Récupère la taille d'une tuile.
                     * @return Taille d'une tuile en unités.
                     */
                    virtual float getTileSize() const = 0;

                    /**
                     * @brief Calcule le vecteur de décalage à appliquer en fonction de l'orientation.
                     * @param orientation Orientation du joueur.
                     * @return Vecteur 3D correspondant au décalage.
                     */
                    virtual Vector3 getGapFromOrientation(const game::Orientation &orientation) = 0;

                    /**
                     * @brief Calcule le vecteur pointant vers le nord selon l'orientation donnée.
                     * @param orientation Orientation actuelle.
                     * @return Vecteur 3D pointant vers le nord.
                     */
                    virtual Vector3 getNorthVector(const game::Orientation &orientation) = 0;

                    /**
                     * @brief Convertit des coordonnées 2D (x, y) en coordonnées 3D.
                     * @param x Coordonnée X sur la carte.
                     * @param y Coordonnée Y sur la carte.
                     * @return Coordonnées 3D correspondantes.
                     */
                    virtual Vector3 get3DCoords(const int &x, const int &y) const = 0;

                    /**
                     * @brief Crée une structure de translation pour un joueur donné.
                     * @param player Joueur concerné.
                     * @param x Coordonnée X de destination.
                     * @param y Coordonnée Y de destination.
                     * @param timeUnit Temps de déplacement.
                     * @return Translation à effectuer.
                     */
                    virtual Translation createTranslation(const APlayerModel &player, const int &x, const int &y, const int &timeUnit) = 0;

                    /**
                     * @brief Applique une translation à un joueur en mettant à jour sa position.
                     * @param deltaUnits Temps écoulé depuis la dernière frame.
                     * @param translationVector Vecteur de déplacement.
                     * @param destination Position finale visée.
                     * @param player Joueur à déplacer.
                     */
                    virtual void translate(const float &deltaUnits, const Vector3 &translationVector, Vector3 &destination, APlayerModel &player) = 0;
            };
        }
    } // namespace gui
} // namespace zappy
