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

    enum eQuadrantType : uint8_t
    {
        eFirst = 0,
        eSecond,
        eThird,
        eFourth,
        eUnknown = 120,
    };

    const double Pi = std::acos(-1);
    struct Component
    {
        Component(float x, float y, float z) : xComponent(x), yComponent(y), zComponent(z) {}
        Component() {}
        float xComponent, yComponent, zComponent;
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
        static sf::Vector2f getCentreForShape(sf::FloatRect);
        static sf::Vector2f getMouseCoordinatesWrtWindow();
        static sf::Vector2f getTransformationRatioForWindow();
        static sf::Vector2f getMarkerCoordinatesWrtWindow(sf::Vector2f);
        static bool isMarkerOnElement(sf::Sprite &, sf::Vector2f);
        static float getDistanceBetweenPoints(const sf::Vector2f &, const sf::Vector2f &);
        static eQuadrantType findQuadrant(float angle);
        static float angleBetweenTwoPoints(const sf::Vector2f &, const sf::Vector2f &);
        static sf::Vector2f getMovementRatio(const sf::Vector2f &, const sf::Vector2f &);
        static sf::Vector2f getMovementRatio(double);
        static int32_t getElementCount();
        static int32_t getFreshElementId();
        static float degToRads(float);
        static float radsToDeg(float);
        static eQuadrantType getQuadrantAfterReflection(float, float);
        static float getAngleForQuadrant(float, eQuadrantType);
        static Component getComponent(float Magnitude, float thetaXY, float thetaZ, Component origin);
        static bool Compare(float, float);
        static bool Compare(sf::Vector2f P1, sf::Vector2f P2);

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

    sf::Vector2f CUtility::getCentreForShape(sf::FloatRect gb)
    {
        return {gb.width / 2, gb.height / 2};
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

    eQuadrantType CUtility::findQuadrant(float angle)
    {
        float x = std::cos(degToRads(angle));
        float y = std::sin(degToRads(angle));
        auto quadrant = eQuadrantType::eUnknown;
        if (y >= 0)
        {
            if (x >= 0)
            {
                quadrant = eQuadrantType::eFirst;
            }
            else
            {
                quadrant = eQuadrantType::eSecond;
            }
        }
        else
        {
            if (x >= 0)
            {
                quadrant = eQuadrantType::eThird;
            }
            else
            {
                quadrant = eQuadrantType::eFourth;
            }
        }
        return quadrant;
    }

    float CUtility::angleBetweenTwoPoints(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        float height = p2.y - p1.y;
        float base = p2.x - p1.x;
        auto angleRad = atan2f(height, base);
        auto angleDeg = radsToDeg(angleRad);

        return angleDeg;
    }

    float CUtility::degToRads(float deg)
    {
        return (Pi / (Utility::TotalDegrees / 2)) * deg;
    }

    float CUtility::radsToDeg(float rads)
    {
        return ((Utility::TotalDegrees / 2) / Pi) * rads;
    }

    sf::Vector2f CUtility::getMovementRatio(const sf::Vector2f &p1, const sf::Vector2f &p2)
    {
        auto angle = angleBetweenTwoPoints(p1, p2);

        float xVel = std::cos(angle);
        float yVel = std::sin(angle);

        return {xVel, yVel};
    }

    sf::Vector2f CUtility::getMovementRatio(double angle)
    {
        float xVel = std::cos(degToRads(angle));
        float yVel = std::sin(degToRads(angle));

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

    eQuadrantType CUtility::getQuadrantAfterReflection(float angleObject1, float angleObject2)
    {
        return findQuadrant(std::abs(angleObject1 - angleObject2));
    }

    float CUtility::getAngleForQuadrant(float angle, eQuadrantType type)
    {
        switch (type)
        {
        case eQuadrantType::eFirst:
            return angle;
        case eQuadrantType::eSecond:
            return (Utility::TotalDegrees / 2) - angle;
        case eQuadrantType::eThird:
            return angle - (Utility::TotalDegrees / 2);
        case eQuadrantType::eFourth:
            return angle * (-1);
        }
    }

    Component CUtility::getComponent(float Magnitude, float thetaXY, float thetaZ, Component origin = Component(0, 0, 0))
    {
        return Component(Magnitude * sin(degToRads(thetaZ)) * cos(degToRads(thetaXY)) + origin.xComponent, Magnitude * sin(degToRads(thetaZ)) * sin(degToRads(thetaXY)) + origin.yComponent, Magnitude * cos(degToRads(thetaZ)) + origin.zComponent);
    }

    bool CUtility::Compare(float p1, float p2)
    {
        return abs(ceil(p2) - ceil(p1)) <= Utility::ComparisonThreshold;
    }

    bool CUtility::Compare(sf::Vector2f P1, sf::Vector2f P2)
    {
        return P1.x == P2.x && P1.y == P2.y;
    }
}

#endif