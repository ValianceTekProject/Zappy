/*
** EPITECH PROJECT, 2025
** Zappy
** File description:
** IObserver
*/

#pragma once

namespace zappy {

    namespace observer {

        /**
         * @class IObserver
         * @brief Interface pour les observateurs qui réagissent à des notifications.
         */
        class IObserver {

        public:
            /**
             * @brief Destructeur virtuel par défaut.
             */
            virtual ~IObserver() = default;

            /**
             * @brief Méthode appelée lors d'une notification.
             * @param signal Le signal ou l'événement transmis à l'observateur.
             */
            virtual void onNotify(int signal) = 0;
        };

    }  // namespace observer

}  // namespace zappy
