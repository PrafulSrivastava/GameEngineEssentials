#ifndef CEXPLOSION_HPP
#define CEXPLOSION_HPP

#include <iostream>
#include "CUtility.hpp"
#include "CParticle.hpp"
#include "CAttribute.hpp"

namespace GameEngine::Particle
{
    enum eShapeType : uint8_t
    {
        eGeometryType = 0,
        eTextureType,
    };
    template <typename Entity, typename NotifyAction>
    class CExplosion : public Entity
    {
    public:
        CExplosion(GameEngine::CEntityWrapper<Entity> &);
        ~CExplosion() = default;
        CExplosion(const CExplosion &) = delete;
        CExplosion &operator=(const CExplosion &) = delete;
        CExplosion(CExplosion &&) = delete;
        CExplosion &operator=(CExplosion &&) = delete;

        constexpr void Explode(float, NotifyAction, float velocity = GameEngine::Particle::EXPLOSION_LEVEL);

        constexpr void initialize(sf::Color color = sf::Color::Red, int32_t count = GameEngine::Particle::MAX_PARTICLES, float size = GameEngine::Particle::MIN_PARTICLE_SIZE);

        constexpr void initialize(sf::Texture *texture = nullptr, int32_t count = GameEngine::Particle::MAX_PARTICLES, float size = GameEngine::Particle::MIN_PARTICLE_SIZE, sf::Color color = sf::Color::Red);

        GameEngine::GameType::shortBool getExplosionStatus();
        void setAttribute(Attribute::AttributeTypes type, GameEngine::CEntityWrapper<Entity> obj);
        std::vector<CParticle> getParticle();

    private:
        std::reference_wrapper<GameEngine::CEntityWrapper<Entity>> m_body;
        std::vector<CParticle> m_particles;
        sf::Vector2f m_initialPosition;
        GameEngine::GameType::shortBool m_isExplosionActive;
        Attribute::CAttribute<Entity> m_attributes;
        sf::Texture m_texture;
    };

    template <typename Entity, typename NotifyAction>
    CExplosion<Entity, NotifyAction>::CExplosion(GameEngine::CEntityWrapper<Entity> &obj) : m_body(obj)
    {
    }

    template <typename Entity, typename NotifyAction>
    constexpr void CExplosion<Entity, NotifyAction>::initialize(sf::Color color, int32_t count, float size)
    {
        float dir = 0;
        float step = Utility::TotalDegrees / float(count);

        for (int i = 0; i < count; i++)
        {
            m_particles.push_back(CParticle{m_body.get().getPosition(), size, dir, color});

            m_particles[i].setColortoShape();

            dir += step;
        }
        m_isExplosionActive.set();
        m_initialPosition = std::ref(m_body.get().getPosition());
    }

    template <typename Entity, typename NotifyAction>
    constexpr void CExplosion<Entity, NotifyAction>::initialize(sf::Texture *texture, int32_t count, float size, sf::Color color)
    {

        float dir = 0;
        float step = Utility::TotalDegrees / float(count);
        auto pos = m_body.get().getPosition();

        m_particles.reserve(count);

        for (int i = 0; i < count; i++)
        {
            m_particles.push_back(CParticle{pos, size, dir, color});
            // m_particles.emplace_back(pos, size, dir, color);

            m_particles[i].setTexturetoShape(texture);

            dir += step;
        }
        m_isExplosionActive.set();
        m_initialPosition = std::ref(m_body.get().getPosition());

        std::cout << m_particles.capacity() << std::endl;
    }

    template <typename Entity, typename NotifyAction>
    void CExplosion<Entity, NotifyAction>::setAttribute(Attribute::AttributeTypes type, GameEngine::CEntityWrapper<Entity> obj)
    {
        m_attributes.setAttribute(type);

        m_attributes.applyAttributes(obj);
        m_attributes.applyAttributes(m_body.get());
    }

    template <typename Entity, typename NotifyAction>
    constexpr void CExplosion<Entity, NotifyAction>::Explode(float destination, NotifyAction callback, float velocity)
    {
        // if (m_isExplosionActive.count())
        {
            for (auto &it : m_particles)
            {
                it.mPosition.x += cos(it.mAngle) * it.mVelocity.x * velocity;
                it.mPosition.y += sin(it.mAngle) * it.mVelocity.y * velocity;

                if (it.mPosition.x >= m_initialPosition.x + destination || it.mPosition.y >= m_initialPosition.y + destination || it.mPosition.x <= m_initialPosition.x - destination || it.mPosition.y <= m_initialPosition.y - destination)
                {
                    setAttribute(Attribute::AttributeTypes::fades, it);
                    m_isExplosionActive.flip();

                    // it.setPosition(it.mPosition.x, it.mPosition.y);
                    // callback(it);
                }

                else
                {
                    it.setPosition(it.mPosition.x, it.mPosition.y);
                    callback(it);
                }
            }
        }
    }

    template <typename Entity, typename NotifyAction>
    GameEngine::GameType::shortBool CExplosion<Entity, NotifyAction>::getExplosionStatus()
    {
        return m_isExplosionActive;
    }

    template <typename Entity, typename NotifyAction>
    std::vector<CParticle> CExplosion<Entity, NotifyAction>::getParticle()
    {
        return m_particles;
    }
}
#endif