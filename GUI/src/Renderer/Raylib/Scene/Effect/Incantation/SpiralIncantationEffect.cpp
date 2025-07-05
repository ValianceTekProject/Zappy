/*
** EPITECH PROJECT, 2024
** zappy
** File description:
** SpiralIncantationEffect.cpp
*/

#include "SpiralIncantationEffect.hpp"

constexpr float animationSpeed = 50.f;

zappy::gui::raylib::SpiralIncantationEffect::SpiralIncantationEffect(const int &playerId, const float &duration, const Color &color) :
    AIncantationEffect(playerId, duration, color),
    _tile( {0, 0} ),
    _animationTime(0.f)
{}

void zappy::gui::raylib::SpiralIncantationEffect::update(const float &deltaUnits)
{
    AEffect::update(deltaUnits);

    _animationTime += deltaUnits;

    if (_animationTime >= animationSpeed) {
        const int numCycles = static_cast<int>(_animationTime / animationSpeed);

        this->_animationTime -= numCycles * animationSpeed;
    }
}

void zappy::gui::raylib::SpiralIncantationEffect::render(const Vector3 &position) const
{
    const float progress = _animationTime / animationSpeed;

    _renderSpiralParticles(position, progress);
}

void zappy::gui::raylib::SpiralIncantationEffect::_renderSpiralParticles(const Vector3& center, float progress) const
{
    constexpr int numParticles = 50;
    constexpr float particleSize = 0.03125f;
    constexpr float heightMax  = 0.8f;
    constexpr float radiusMax  = 0.4f;
    constexpr float radiusMin  = 0.2f;
    constexpr float TWO_PI  = PI * 2.0f;
    constexpr float turns  = 5.0f;

    for (int i = 0; i < numParticles; i++) {
        float initialOffset    = (static_cast<float>(i) / numParticles) * TWO_PI;
        float particleProgress = fmodf(progress + static_cast<float>(i) / numParticles, 1.0f);
        float angle  = particleProgress * TWO_PI * turns + initialOffset;
        float height = particleProgress * heightMax;
        float radius = radiusMin + (radiusMax - radiusMin) * (1.0f - particleProgress * heightMax);

        Vector3 p = center;
        p.x += cosf(angle) * radius;
        p.z += sinf(angle) * radius;
        p.y += height;

        float sizeVariation = 1.0f + 0.1f * sinf(TWO_PI * particleProgress);
        float sizeDecay = powf(1.0f - particleProgress, 0.25f);
        float size = particleSize * sizeVariation * sizeDecay;

        DrawSphere(p, size, _color);
    }
}

bool zappy::gui::raylib::SpiralIncantationEffect::isAt(int x, int y) const
{
    return static_cast<int>(_tile.x) == x && static_cast<int>(_tile.y) == y;
}
