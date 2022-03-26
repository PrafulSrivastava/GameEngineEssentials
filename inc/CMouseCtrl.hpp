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
        // make a sprite's vision cone follow mouse marker
        uint8_t lockObjectVisionOnMarker(sf::Sprite &, sf::Vector2f);
        // make a sprite's vision cone unfollow mouse marker
        void unlockObjectVisionOnMarker(uint8_t);
        // make a sprite move to mouse marker
        void moveObjectToMarker(uint8_t subId);
        // stop a sprite moving towards mouse marker
        void stopChasingMarker(uint8_t subId);
        // 3d left-right-up-down follow mouse
        void runMainLoop();
        void setMarkerPos(uint8_t, sf::Vector2f);

    private:
        float getDistanceBetweenPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        float angleBetweenTwoPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        uint8_t findQuadrant(const sf::Vector2f &p1, const sf::Vector2f &p2);

        std::vector<bool> m_followMarker;
        std::vector<bool> m_moveToMarker;
        std::vector<sf::Vector2f> m_markerPos;
        std::vector<std::reference_wrapper<sf::Sprite>> m_sprites;
        int8_t m_followMarkerSubId{-1};
    };

    void CMouseCtrl::runMainLoop()
    {

        uint8_t index = 0;

        for (auto spriteRefWrap : m_sprites)
        {
            auto markerPos = m_markerPos[index];
            if (m_followMarker[index])
            {
                auto spritePos = spriteRefWrap.get().getPosition();
                auto angle = CUtility::angleBetweenTwoPoints(spritePos, markerPos);
                spriteRefWrap.get().setRotation(angle);
                if (m_moveToMarker[index])
                {

                    if (spritePos != markerPos)
                    {
                        auto ratio = CUtility::getMovementRatio(spritePos, markerPos);
                        spriteRefWrap.get().setPosition(spritePos.x + ratio.x, spritePos.y + ratio.y);
                    }
                }
            }
        }
    }

    uint8_t CMouseCtrl::lockObjectVisionOnMarker(sf::Sprite &sprite, sf::Vector2f makerPos)
    {
        m_followMarkerSubId++;
        m_followMarker.push_back(true);
        m_moveToMarker.push_back(false);
        m_markerPos.push_back(makerPos);
        m_sprites.push_back(std::ref(sprite));

        return m_followMarkerSubId;
    }

    void CMouseCtrl::unlockObjectVisionOnMarker(uint8_t subId)
    {
        m_followMarker[subId] = false;
    }

    void CMouseCtrl::moveObjectToMarker(uint8_t subId)
    {
        m_moveToMarker[subId] = true;
    }

    void CMouseCtrl::stopChasingMarker(uint8_t subId)
    {
        m_moveToMarker[subId] = false;
    }

    void CMouseCtrl::setMarkerPos(uint8_t subId, sf::Vector2f markerPos)
    {
        m_markerPos[subId] = markerPos;
    }

}

#endif