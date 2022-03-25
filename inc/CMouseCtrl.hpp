#ifndef CMOUSE_CTRL_HPP
#define CMOUSE_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <thread>
#include "CInputCtrl.hpp"
#include "CUtility.hpp"

namespace GameEngine
{

    using Action = std::function<void()>;

    class CMouseCtrl : public CInputCtrl<sf::Mouse::Button, Action>
    {
    public:
        CMouseCtrl() = default;
        ~CMouseCtrl() = default;
        CMouseCtrl(const CMouseCtrl &) = delete;
        CMouseCtrl &operator=(const CMouseCtrl &) = delete;
        CMouseCtrl(CMouseCtrl &&) = delete;
        CMouseCtrl &operator=(CMouseCtrl &&) = delete;

        // lock objects with a click
        void lockObjectOnClick(sf::Sprite &);
        // lock objects with a click hold
        void lockObjectOnClickTillHold(sf::Sprite &);
        // unlock objects from a click
        void unlockObjectOnClick(sf::Sprite &);
        // unlock objects from a click hold
        void unlockObjectOnHoldRelease(sf::Sprite &);
        // make a sprite's vision cone follow mouse cursor
        uint8_t lockObjectVisionOnCursor(sf::Sprite &);
        // make a sprite's vision cone unfollow mouse cursor
        void unlockObjectVisionOnCursor(uint8_t);
        // make a sprite move to mouse cursor
        void moveObjectToCursor(uint8_t subId);
        // stop a sprite moving towards mouse cursor
        void stopChasingCursor(uint8_t subId);
        // 3d left-right-up-down follow mouse
        void followCursor();

    private:
        float getDistanceBetweenPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        float angleBetweenTwoPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        uint8_t findQuadrant(const sf::Vector2f &p1, const sf::Vector2f &p2);

        std::vector<bool> m_followCursor;
        std::vector<bool> m_moveToCursor;
        std::vector<std::reference_wrapper<sf::Sprite>> m_sprites;
        int8_t m_followCursorSubId{-1};
    };

    void CMouseCtrl::followCursor()
    {

        uint8_t index = 0;
        auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());

        for (auto spriteRefWrap : m_sprites)
        {

            if (m_followCursor[index])
            {
                auto spritePos = spriteRefWrap.get().getPosition();
                auto angle = CUtility::angleBetweenTwoPoints(spritePos, mousePos);
                spriteRefWrap.get().setRotation(angle);
                if (m_moveToCursor[index])
                {

                    if (spritePos != mousePos)
                    {
                        auto ratio = CUtility::getMovementRatio(spritePos, mousePos);
                        spriteRefWrap.get().setPosition(spritePos.x + ratio.x, spritePos.y + ratio.y);
                    }
                }
            }
        }
    }

    uint8_t CMouseCtrl::lockObjectVisionOnCursor(sf::Sprite &sprite)
    {
        m_followCursorSubId++;
        m_followCursor.push_back(true);
        m_moveToCursor.push_back(false);
        m_sprites.push_back(std::ref(sprite));

        return m_followCursorSubId;
    }

    void CMouseCtrl::unlockObjectVisionOnCursor(uint8_t subId)
    {
        m_followCursor[subId] = false;
    }

    void CMouseCtrl::moveObjectToCursor(uint8_t subId)
    {
        m_moveToCursor[subId] = true;
    }

    void CMouseCtrl::stopChasingCursor(uint8_t subId)
    {
        m_moveToCursor[subId] = false;
    }
}

#endif