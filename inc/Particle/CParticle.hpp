#ifndef CPARTICLE_HPP
#define CPARTICLE_HPP

#include "CUtility.hpp"

namespace GameEngine::Particle
{
    class CParticle : public CEntityWrapper<sf::CircleShape>
    {
    public:
        float mDirection, mTime, mAngle, mRadius;
        int8_t mSide;
        sf::Vector2f mVelocity;
        sf::Vector2f mPosition;
        std::reference_wrapper<sf::Vector2f> initPos;
        sf::Color mColor;
        sf::Texture *mTexture;

    public:
        CParticle(sf::Vector2f pos, float radius, float dir, sf::Color color) : mRadius(radius), mDirection(dir), initPos(std::ref(pos)), mColor(color)
        {
            setPosition(initPos);
            mPosition = initPos;
            setRadius(mRadius);
            setFillColor(mColor);

            mAngle = CUtility::degToRads(mDirection);

            mSide = (int)CUtility::getRandomNumber(GameEngine::Particle::MIN_SIDES, GameEngine::Particle::MAX_SIDES);

            setPointCount(mSide);

            mVelocity.x = CUtility::getRandomNumber(GameEngine::Particle::MIN_PARTICLE_VELOCITY, GameEngine::Particle::MAX_PARTICLE_VELOCITY) / float(GameEngine::Particle::MAX_PARTICLE_VELOCITY + 1);
            mVelocity.y = CUtility::getRandomNumber(GameEngine::Particle::MIN_PARTICLE_VELOCITY, GameEngine::Particle::MAX_PARTICLE_VELOCITY) / float(GameEngine::Particle::MAX_PARTICLE_VELOCITY + 1);
        }

        void setTexturetoShape(sf::Texture *texture)
        {
            mTexture = texture;
            setTexture(mTexture);

            auto l1 = CUtility::getRandomNumber(GameEngine::Particle::MIN_DIM, GameEngine::Particle::MAX_DIM);
            auto l2 = CUtility::getRandomNumber(GameEngine::Particle::MIN_DIM, GameEngine::Particle::MAX_DIM);
            auto l3 = CUtility::getRandomNumber(GameEngine::Particle::MIN_DIM, GameEngine::Particle::MAX_DIM);
            auto l4 = CUtility::getRandomNumber(GameEngine::Particle::MIN_DIM, GameEngine::Particle::MAX_DIM);

            setTextureRect(sf::IntRect(l1, l2, l3, l4));
        }

        void setColortoShape()
        {
            setFillColor(mColor);
        }
    };
}

#endif // CPARTICLE_HPP