/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** APlayerAnimAction.hpp
*/

#pragma once

#include "APlayerAction.hpp"
#include "IEffect.hpp"

#include <memory>

namespace zappy {
    namespace gui {
        namespace raylib {
            class APlayerAnimAction : public APlayerAction {
                public:
                    APlayerAnimAction(
                        const ssize_t &animationId,
                        const int &playerId,
                        const ActionType &actionType,
                        std::unique_ptr<IEffect> effect,
                        const float &timeUnit,
                        const float &elapsedTime = 0.f
                    );
                    virtual ~APlayerAnimAction() override = default;

                    ssize_t getAnimationId() const { return _animationId; }

                    virtual void update(const float &deltaUnits, APlayerModel &player) override;

                    bool hasEffectEnded() { return _effect->hasEnded(); }

                    virtual void render(const Vector3 &position);

                    IEffect *getEffect() const { return _effect.get(); }

                protected:
                    std::unique_ptr<IEffect> _effect;
                    float _effectDuration = 0.f;
                    float _effectElapsed = 0.f;

                private:
                    ssize_t _animationId;
            };
        }
    }
}
