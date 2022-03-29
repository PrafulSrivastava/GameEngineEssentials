#ifndef CUTILITY_CTRL_HPP
#define CUTILITY_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <math.h>
#include "CInputCtrl.hpp"
#include "Globals.hpp"

namespace GameEngine
{

    enum eQuadrantType : int32_t
    {
        eFirst = 0,
        eSecond,
        eThird,
        eFourth,
    };

    class CUtility
    {
    public:
        CUtility() = delete;
        ~CUtility() = delete;
        CUtility(const CUtility &) = delete;
        CUtility &operator=(const CUtility &) = delete;
        CUtility(CUtility &&) = delete;
        CUtility &operator=(CUtility &&) = delete;

        static void setWindow(std::shared_ptr<sf::RenderWindow>);
        static void setOriginToCenter(sf::Sprite &);
        static sf::Vector2f getMouseCoordinatesWrtWindow();
        static sf::Vector2f getTransformationRatioForWindow();
        static sf::Vector2f getMarkerCoordinatesWrtWindow(sf::Vector2f);
        static bool isMarkerOnElement(sf::Sprite &, sf::Vector2f);
        static float getDistanceBetweenPoints(const sf::Vector2f &, const sf::Vector2f &);
        static eQuadrantType findQuadrant(const sf::Vector2f &, const sf::Vector2f &);
        static float angleBetweenTwoPoints(const sf::Vector2f &, const sf::Vector2f &);
        static sf::Vector2f getMovementRatio(const sf::Vector2f &, const sf::Vector2f &);
        static int32_t getElementCount();
        static int32_t getFreshElementId();

    private:
        static int32_t globalElementCount;
        static std::shared_ptr<sf::RenderWindow> window;
    };

    int32_t CUtility::globalElementCount = 0;
    std::shared_ptr<sf::RenderWindow> CUtility::window = nullptr;

    void CUtility::setWindow(std::shared_ptr<sf::RenderWindow> win)
    {
        window = win;
    }

    void CUtility::setOriginToCenter(sf::Sprite &sprite)
    {
        sf::FloatRect gb = sprite.getLocalBounds();
        sprite.setOrigin(gb.width / 2, gb.height / 2);
    }

    sf::Vector2f CUtility::getMouseCoordinatesWrtWindow()
    {
        return static_cast<sf::Vector2f>(sf::Mouse::getPosition(*window));
    }

    sf::Vector2f CUtility::getTransformationRatioForWindow()
    {
        auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
        auto mousePosWindow = getMouseCoordinatesWrtWindow();

        return {mousePos.x - mousePosWindow.x, mousePos.y - mousePosWindow.y};
    }

    sf::Vector2f CUtility::getMarkerCoordinatesWrtWindow(sf::Vector2f markerPos)
    {
        auto ratio = getTransformationRatioForWindow();
        return {markerPos.x - ratio.x, markerPos.y - ratio.y};
    }

    bool CUtility::isMarkerOnElement(sf::Sprite &sprite, sf::Vector2f markerPos)
    {
        auto rect = sprite.getGlobalBounds();
        markerPos = getMarkerCoordinatesWrtWindow(markerPos);
        return rect.contains(markerPos);
    }

    float CUtility::getDistanceBetweenPoints(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        return std::sqrt(std::pow((p2.x - p1.x), 2) + std::pow((p2.y - p1.y), 2));
    }

    eQuadrantType CUtility::findQuadrant(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        auto x = p1.x - p2.x;
        auto y = p1.y - p2.y;

        if (y >= 0)
        {
            if (x >= 0)
            {
                return eQuadrantType::eFirst;
            }
            else
            {
                return eQuadrantType::eSecond;
            }
        }
        else
        {
            if (x >= 0)
            {
                return eQuadrantType::eThird;
            }
            else
            {
                return eQuadrantType::eFourth;
            }
        }
    }

    float CUtility::angleBetweenTwoPoints(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        float height = p2.y - p1.y;
        float base = p2.x - p1.x;
        auto angleRad = atan2f(height, base);
        auto angleDeg = (Utility::TotalDegrees / Utility::Pie) * angleRad;

        return angleDeg;
    }

    sf::Vector2f CUtility::getMovementRatio(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        auto angle = angleBetweenTwoPoints(p1, p2);
        float xVel = std::cos(angle);
        float yVel = std::sin(angle);

        return {xVel, yVel};
    }

    int32_t CUtility::getElementCount()
    {
        return globalElementCount;
    }

    int32_t CUtility::getFreshElementId()
    {
        return ++globalElementCount;
    }

}

#endif