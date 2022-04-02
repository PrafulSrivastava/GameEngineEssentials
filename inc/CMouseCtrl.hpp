#ifndef CMOUSE_CTRL_HPP
#define CMOUSE_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <memory>
#include <thread>
#include "CInputCtrl.hpp"
#include "CUtility.hpp"
#include "CEntityWrapper.hpp"

namespace GameEngine
{
    using MouseParamType = int32_t;
    using MouseAction = std::function<void(MouseParamType)>;
    using Entity = CEntityWrapper<sf::Sprite>;
    class CMouseCtrl : public CInputCtrl<sf::Mouse::Button, MouseAction, MouseParamType>
    {
    public:
        CMouseCtrl() = default;
        ~CMouseCtrl() = default;
        CMouseCtrl(const CMouseCtrl &) = delete;
        CMouseCtrl &operator=(const CMouseCtrl &) = delete;
        CMouseCtrl(CMouseCtrl &&) = delete;
        CMouseCtrl &operator=(CMouseCtrl &&) = delete;

        // add sprite to list and monitor it.
        [[nodiscard]] int32_t subscribeToMouseCtrl(Entity &);
        // remove sprite from monitor list
        void unsubscribe(int32_t subId);
        // Begin the main loop
        void runMainLoop();
        // lock objects with a click
        void lockObjectOnClick(int32_t subId);
        // lock objects with a click hold
        void lockObjectOnClickTillHold(int32_t subId);
        // unlock objects from a click
        void unlockObjectOnClick(int32_t);
        // unlock objects from a click hold
        void unlockObjectOnHoldRelease(int32_t);
        // make a sprite's vision cone follow mouse marker
        void lockObjectVisionOnMarker(int32_t, sf::Vector2f);
        // make a sprite's vision cone unfollow mouse marker
        void unlockObjectVisionOnMarker(int32_t);
        // make a sprite move to mouse marker
        void chaseMarker(int32_t);
        // stop a sprite moving towards mouse marker
        void stopChasingMarker(int32_t);
        // Set Marker pos for a subscriber
        void setMarkerPos(int32_t, sf::Vector2f);

    private:
        float getDistanceBetweenPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        float angleBetweenTwoPoints(const sf::Vector2f &p1, const sf::Vector2f &p2);
        int32_t findQuadrant(const sf::Vector2f &p1, const sf::Vector2f &p2);
        int32_t getSpriteIndex(int32_t);

        std::vector<bool> m_followMarker;
        std::vector<bool> m_lockOnCursor;
        std::vector<bool> m_moveToMarker;
        std::vector<sf::Vector2f> m_markerPos;
        std::vector<std::reference_wrapper<Entity>> m_sprites;
        int32_t m_followMarkerSubId{InvalidIndex};
    };

    int32_t CMouseCtrl::subscribeToMouseCtrl(Entity &sprite)
    {
        if (getSpriteIndex(sprite.getEntityId()) == InvalidIndex)
        {
            m_followMarkerSubId++;
            m_followMarker.push_back(false);
            m_lockOnCursor.push_back(false);
            m_moveToMarker.push_back(false);
            m_markerPos.push_back({0, 0});
            m_sprites.push_back(std::ref(sprite));

            return m_followMarkerSubId;
        }

        return InvalidIndex;
    }

    void CMouseCtrl::unsubscribe(int32_t subId)
    {
        stopChasingMarker(subId);
        unlockObjectVisionOnMarker(subId);
        unlockObjectOnClick(subId);
    }

    void CMouseCtrl::runMainLoop()
    {
        int32_t index = 0;

        for (auto spriteRefWrap : m_sprites)
        {
            auto markerPos = m_markerPos[index];
            if (m_followMarker[index])
            {
                if (m_lockOnCursor[index])
                {
                    spriteRefWrap.get().setPosition(CUtility::getMarkerCoordinatesWrtWindow(static_cast<sf::Vector2f>(sf::Mouse::getPosition())));
                }
                else
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
    }

    int32_t CMouseCtrl::getSpriteIndex(int32_t id)
    {
        for (auto i{0}; i < m_sprites.size(); i++)
        {
            if (m_sprites[i].get().getEntityId() == id)
            {
                return i;
            }
        }

        return -1;
    }

    void CMouseCtrl::lockObjectOnClick(int32_t subId)
    {
        if (CUtility::isMarkerOnElement(m_sprites[subId].get(), static_cast<sf::Vector2f>(sf::Mouse::getPosition())))
        {
            m_lockOnCursor[subId] = true;
        }
    }

    void CMouseCtrl::lockObjectVisionOnMarker(int32_t subId, sf::Vector2f markerPos)
    {
        m_followMarker[subId] = true;
        m_markerPos[subId] = markerPos;
    }

    void CMouseCtrl::unlockObjectVisionOnMarker(int32_t subId)
    {
        m_followMarker[subId] = false;
    }

    void CMouseCtrl::chaseMarker(int32_t subId)
    {
        m_moveToMarker[subId] = true;
    }

    void CMouseCtrl::stopChasingMarker(int32_t subId)
    {
        m_moveToMarker[subId] = false;
    }

    void CMouseCtrl::setMarkerPos(int32_t subId, sf::Vector2f markerPos)
    {
        m_markerPos[subId] = markerPos;
    }

    void CMouseCtrl::unlockObjectOnClick(int32_t subId)
    {
        m_lockOnCursor[subId] = false;
    }

}

#endif