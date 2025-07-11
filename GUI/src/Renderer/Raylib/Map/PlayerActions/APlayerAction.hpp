/*
** EPITECH PROJECT, 2024
** Zappy
** File description:
** APlayerAction.hpp
*/

#pragma once

#include "IPlayerAction.hpp"

namespace zappy {
    namespace gui {
        namespace raylib {
            class APlayerAction : public IPlayerAction
            {
                public:
                    APlayerAction(
                        const int &playerId,
                        const ActionType &actionType,
                        const float &timeUnit,
                        const float &elapsedTime = 0.f
                    );
                    virtual ~APlayerAction() override = default;

                    int getPlayerId() const override { return _playerId; }
                    ActionType getActionType() const override { return _actionType; }

                    bool hasActionStarted() const override { return _hasStarted; }
                    void startAction() override { _hasStarted = true; }

                    virtual void update(const float &deltaUnits, APlayerModel &player) override;

                    virtual bool ActionWillEnd(const float &deltaUnits) const override;
                    virtual void finishAction(const float &deltaUnits, APlayerModel &player) override;

                protected:
                    int _playerId;

                    ActionType _actionType;

                    float _timeUnit;
                    float _elapsedTime;

                private:
                    bool _hasStarted;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
