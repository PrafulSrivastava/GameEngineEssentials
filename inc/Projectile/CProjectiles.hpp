#ifndef CPROJECTILES_HPP
#define CPROJECTILES_HPP

#include <bitset>
#include <iostream>
#include "Globals.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"
#include <SFML/Graphics.hpp>

namespace GameEngine::Projectile
{
    using Dimension = float;

    enum class AttributeTypes : uint8_t
    {
        reflects = 0,
        fades,
        tail,
        exposive,
        fluid,
        spins,
        obeyGravity,
        expands,
        shrinks,
        disperses,
    };

    enum class ObstructionTypes : uint8_t
    {
        reflective = 0,
        destructive,
        penetrable
    };

    class CProjectile : public CEntityWrapper<sf::CircleShape>
    {
    public:
        CProjectile(
            Dimension radius,
            Dimension sides,
            sf::Color fillColor = sf::Color::Red,
            Dimension outlineThickness = 1,
            sf::Color outlineColor = sf::Color::White);
        ~CProjectile() = default;
        CProjectile(const CProjectile &) = delete;
        CProjectile &operator=(const CProjectile &) = delete;
        CProjectile(CProjectile &&) = delete;
        CProjectile &operator=(CProjectile &&) = delete;
        // spawn a projectile
        void spawn(std::vector<uint8_t>, sf::Vector2f, double);
        // shoot it from source to destination
        void shoot(double angle);
        // execute all functionalities at one place
        void runMainLoop();
        // callback for collision
        void onCollision(ObstructionTypes, float);

    private:
        void applyAttributes();
        // is reflectable
        // is dissappearable
        // has tail
        // explosive
        // is fluid
        // spins
        // follows gravity
        // becomes larger
        // becomes smaller
        // spreads into smaller chunks on collision.
        std::bitset<Projectile::AttributeCount> m_attributeBitSet;
        Dimension m_velocityX{0};
        Dimension m_velocityY{0};
        float m_direction{0};
        float m_directionOld{0};
        eQuadrantType m_quadrant{eQuadrantType::eUnknown};
        bool m_isShot{false};
        sf::Clock m_CollisionMonitor;
    };

    CProjectile::CProjectile(Dimension radius, Dimension sides, sf::Color fillColor, Dimension outlineThickness, sf::Color outlineColor)
    {
        setRadius(radius);
        setRotation(0);
        setPointCount(sides);
        setFillColor(fillColor);
        setOutlineThickness(outlineThickness);
        setOutlineColor(outlineColor);
        setOrigin(CUtility::getCentreForShape(getGlobalBounds()));
    }

    void CProjectile::spawn(std::vector<uint8_t> attributesStatus, sf::Vector2f pos, double velocity)
    {
        if (attributesStatus.size() == m_attributeBitSet.size())
        {
            for (uint8_t index{0}; index < attributesStatus.size(); index++)
            {
                m_attributeBitSet[index] = attributesStatus[index];
            }
        }

        m_velocityX = velocity;
        m_velocityY = velocity;
        setPosition(pos);
    }

    void CProjectile::shoot(double angle)
    {
        m_direction = angle;
        m_isShot = true;
        setLiveStatus(true);
    }

    void CProjectile::onCollision(ObstructionTypes obstruction, float angleOfObstruction)
    {
        switch (obstruction)
        {
        case ObstructionTypes::reflective:
            // temporary soultion
            if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::reflects)] == 1)
            {
                if (m_CollisionMonitor.getElapsedTime().asMilliseconds() >= Projectile::CollisionTimeSeparation)
                {
                    m_CollisionMonitor.restart();
                    if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::reflects)] == 1)
                    {
                        m_quadrant = CUtility::getQuadrantAfterReflection(m_direction, angleOfObstruction);
                        m_direction = CUtility::getAngleForQuadrant(m_direction, m_quadrant);
                    }
                }
            }
            break;
        case ObstructionTypes::destructive:
            break;
        case ObstructionTypes::penetrable:
            break;
        }
    }

    void CProjectile::applyAttributes()
    {
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::fades)])
        {
            auto color = getFillColor();
            if (color.a == 0)
            {
                m_isShot = false;
            }
            else
            {
                color.a--;
                setFillColor(color);
            }
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::tail)])
        {
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::exposive)])
        {
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::fluid)])
        {
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::spins)])
        {
            rotate(Projectile::SpinAngle);
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::obeyGravity)])
        {
            if (std::sin(CUtility::degToRads(m_direction)) > 0)
            {
                m_velocityY += (Projectile::GravityCoefficient);
            }
            else
            {
                m_velocityY -= (Projectile::GravityCoefficient);
            }
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::expands)])
        {
            scale(Projectile::EnlargeSizeRatio);
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::shrinks)])
        {
            scale(Projectile::ShrinkSizeRatio);
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::disperses)])
        {
        }
    }

    void CProjectile::runMainLoop()
    {
        applyAttributes();

        if (m_isShot)
        {
            auto ratio = CUtility::getMovementRatio(m_direction);
            auto xMotion = ratio.x * m_velocityX;
            auto yMotion = ratio.y * m_velocityY;
            if (m_directionOld != m_direction)
            {
                m_quadrant = CUtility::findQuadrant(m_direction);
                m_directionOld = m_direction;
            }
            move(xMotion, yMotion);
        }
        else
        {
            setLiveStatus(false);
        }
    }
}

#endif