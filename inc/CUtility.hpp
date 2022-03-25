#ifndef CUTILITY_CTRL_HPP
#define CUTILITY_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <math.h>
#include "CInputCtrl.hpp"

namespace GameEngine{

    enum eQuadrantType : uint8_t {
        eFirst = 0,
        eSecond,
        eThird,
        eFourth,
    };

    class CUtility {
    public:
        CUtility() = delete;
        ~CUtility() = delete;
        CUtility(const CUtility&) = delete;
        CUtility& operator =(const CUtility&) = delete;
        CUtility(CUtility&&) = delete;
        CUtility& operator =(CUtility&&) = delete;

        static void setOriginToCenter(sf::Sprite& sprite);
        static float getDistanceBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);
        static eQuadrantType findQuadrant(const sf::Vector2f& p1, const sf::Vector2f& p2);
        static float angleBetweenTwoPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);
        static sf::Vector2f getMovementRatio(const sf::Vector2f& p1, const sf::Vector2f& p2);
    }; 

    void CUtility::setOriginToCenter(sf::Sprite& sprite)
    {
        sf::FloatRect gb = sprite.getLocalBounds();
        sprite.setOrigin(gb.width / 2, gb.height / 2);
    } 

    float CUtility::getDistanceBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        return std::sqrt(std::pow((p2.x - p1.x), 2) + std::pow((p2.y - p1.y), 2));
    }

    eQuadrantType CUtility::findQuadrant(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        auto x = p1.x - p2.x;
        auto y = p1.y - p2.y;

        if(y >= 0){
            if(x >= 0){
                return eQuadrantType::eFirst;
            } else {
                return eQuadrantType::eSecond;
            }
        } else{
            if(x >= 0){
                return eQuadrantType::eThird;
            } else {
                return eQuadrantType::eFourth;
            }
        }
    }

    float CUtility::angleBetweenTwoPoints(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        float height = p2.y - p1.y;
        float base = p2.x - p1.x;
        auto angleRad = atan2f(height, base);        
        auto angleDeg = (180/3.14) * angleRad;
        
        return angleDeg;
    }

    sf::Vector2f CUtility::getMovementRatio(const sf::Vector2f& p1, const sf::Vector2f& p2)
    {
        auto angle = angleBetweenTwoPoints(p1, p2);    
        float xVel = std::cos(angle);
        float yVel = std::sin(angle);

        return {xVel, yVel};
    }
    
}

#endif