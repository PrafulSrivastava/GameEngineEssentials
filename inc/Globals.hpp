#ifndef GLOBALS_HPP
#define GLOBALS_HPP

#include <SFML/Graphics.hpp>
#include <bitset>

namespace GameEngine
{
    namespace GameType
    {
        using shortBool = std::bitset<1>;
        using Distance = float;
    }
    namespace Utility
    {
        constexpr auto TotalDegrees = 360;
        constexpr auto ComparisonThreshold = 10;
        const auto Transparent = sf::Color(255, 255, 255, 128);
    }
    namespace Projectile
    {
        constexpr auto AttributeCount = 10;
        constexpr auto SpinAngle = 10;
        const sf::Vector2f EnlargeSizeRatio = {1.001f, 1.001f};
        const sf::Vector2f ShrinkSizeRatio = {0.998f, 0.998f};
        constexpr auto CollisionTimeSeparation = 50;
        constexpr auto GravityCoefficient = 0.005f;
    }

    namespace Trigger
    {
        constexpr auto TimeInterval = 1;
    }

}

#endif