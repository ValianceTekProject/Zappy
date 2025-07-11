/*
** EPITECH PROJECT, 2024
** zappyGood
** File description:
** APlayerModel.hpp
*/

#pragma once

#include "AModel.hpp"
#include "Player.hpp"

#include <map>

namespace zappy {
    namespace gui {
        namespace raylib {
            class APlayerModel : public AModel {
                public:
                    enum class State {
                        IDLE,
                        WALK,
                        EJECT
                    };

                    APlayerModel(const int &id);
                    virtual ~APlayerModel() override = default;

                    virtual void init() override;

                    // Setters
                    void setGamePosition(const Vector2 &position) { _gamePosition = position; }

                    void setHeadOrigin(const Vector3 &origin) { _headOrigin = origin; }

                    // Getters
                    int getId() const { return _id; }
                    State getState() const { return _state; }

                    Vector2 getGamePosition() const { return _gamePosition; }

                    game::Orientation getOrientation() const { return _orientation; }
                    Vector3 getOrientationRotation() const;
                    Vector3 getHeadOrigin() const;

                    virtual void look(const game::Orientation &orientation);
                    void lookLeft();
                    void lookRight();

                    virtual void update(const float &deltaUnits) override;

                    virtual void idle();
                    virtual void walk();
                    virtual void eject();

                protected:
                    virtual void _initModel(const std::string &modelPath) override;

                    int _id;

                    State _state;

                    Vector2 _gamePosition;
                    game::Orientation _orientation;

                    Vector3 _headOrigin;

                    Vector3 _northRotation;

                    ModelAnimation *_modelAnimations;

                    int _animsCount;
                    unsigned int _animCurrentFrame;
                    float _frameAccumulator;

                    std::map<State, int> _animationIndexMap;
                    std::map<State, float> _animationFrameSpeedMap;
            };
        } // namespace raylib
    } // namespace gui
} // namespace zappy
