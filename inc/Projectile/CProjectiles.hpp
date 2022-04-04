#ifndef CPROJECTILES_HPP
#define CPROJECTILES_HPP

#include <bitset>
#include <iostream>
#include "Globals.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"
#include <SFML/Graphics.hpp>
#include "CAttribute.hpp"

namespace GameEngine::Projectile
{
    using Dimension = float;
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
        void spawn(sf::Vector2f, double);
        // shoot it from source to destination
        void shoot(double angle);
        // execute all functionalities at one place
        void runMainLoop();
        // callback for collision
        void onCollision(Attribute::AttributeTypes, float);
        void setAttribute(Attribute::AttributeTypes);
        bool isShot();
        void setShotStatus(bool);

    private:
        sf::Vector2f m_velocity;
        float m_direction{0};
        float m_directionOld{0};
        eQuadrantType m_quadrant{eQuadrantType::eUnknown};
        GameType::shortBool m_isShot;
        sf::Clock m_CollisionMonitor;
        Attribute::CAttribute<sf::CircleShape> m_attributes;
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

    bool CProjectile::isShot()
    {
        return m_isShot.all();
    }

    void CProjectile::setShotStatus(bool status)
    {
        (status) ? m_isShot.set(0) : m_isShot.reset(0);
    }

    void CProjectile::setAttribute(Attribute::AttributeTypes type)
    {
        m_attributes.setAttribute(type);
    }

    void CProjectile::spawn(sf::Vector2f pos, double velocity)
    {
        m_velocity.x = velocity;
        m_velocity.y = velocity;
        setPosition(pos);
    }

    void CProjectile::shoot(double angle)
    {
        m_direction = angle;
        m_isShot.set(0);
        m_isActive.set(0);
    }

    void CProjectile::onCollision(Attribute::AttributeTypes obstruction, float angleOfObstruction)
    {
        switch (obstruction)
        {
        case Attribute::AttributeTypes::reflects:
            // temporary soultion
            if (m_CollisionMonitor.getElapsedTime().asMilliseconds() >= Projectile::CollisionTimeSeparation)
            {
                m_CollisionMonitor.restart();
                if (m_attributes.getAttributeState(Attribute::AttributeTypes::reflects))
                {
                    m_quadrant = CUtility::getQuadrantAfterReflection(m_direction, angleOfObstruction);
                    m_direction = CUtility::getAngleForQuadrant(m_direction, m_quadrant);
                }
            }
            break;
        case Attribute::AttributeTypes::destructive:
            break;
        case Attribute::AttributeTypes::penetrable:
            break;
        }
    }

    void CProjectile::runMainLoop()
    {
        m_attributes.applyAttributes(*this);
        if (!m_isActive.all())
        {
            m_isShot.reset(0);
        }
        if (m_isShot.all())
        {
            auto ratio = CUtility::getMovementRatio(m_direction);
            auto xMotion = ratio.x * m_velocity.x;
            auto yMotion = ratio.y * m_velocity.y;
            if (m_directionOld != m_direction)
            {
                m_quadrant = CUtility::findQuadrant(m_direction);
                m_directionOld = m_direction;
            }
            move(xMotion, yMotion);
        }
    }
}

#endif