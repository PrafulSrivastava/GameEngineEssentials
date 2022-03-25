#ifndef CMOUSE_CTRL_HPP
#define CMOUSE_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include <memory>
#include "CInputCtrl.hpp"
#include "CUtility.hpp"

namespace GameEngine{

    using Action = std::function<void()>;

    class CMouseCtrl : public CInputCtrl<sf::Mouse::Button, Action> {
    public:
        CMouseCtrl() = default;
        ~CMouseCtrl() = default;
        CMouseCtrl(const CMouseCtrl&) = delete;
        CMouseCtrl& operator =(const CMouseCtrl&) = delete;
        CMouseCtrl(CMouseCtrl&&) = delete;
        CMouseCtrl& operator =(CMouseCtrl&&) = delete;

        //lock objects with a click
        void lockObjectOnClick(sf::Sprite&);
        //lock objects with a click hold
        void lockObjectOnClickTillHold(sf::Sprite&);
        //unlock objects from a click
        void unlockObjectOnClick(sf::Sprite&);
        //unlock objects from a click hold
        void unlockObjectOnHoldRelease(sf::Sprite&);
        //make a sprite's vision cone follow mouse cursor        
        uint8_t lockObjectVisionOnCursor(sf::Sprite&);
        //make a sprite's vision cone unfollow mouse cursor        
        void unlockObjectVisionOnCursor(uint8_t);
        //make a sprite move to mouse cursor
        void moveObjectToCursor(sf::Sprite&);
        //3d left-right-up-down follow mouse
        void setVisionOnCursor();

    private:
        float getDistanceBetweenPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);
        float angleBetweenTwoPoints(const sf::Vector2f& p1, const sf::Vector2f& p2);
        uint8_t findQuadrant(const sf::Vector2f& p1, const sf::Vector2f& p2);

        std::vector<bool> m_followCursor;
        std::vector<std::reference_wrapper<sf::Sprite>> m_sprites;
        int8_t m_followCursorSubId{-1};        
    };

    void CMouseCtrl::setVisionOnCursor(){

        for(auto spriteRefWrap : m_sprites){
            auto mousePos = static_cast<sf::Vector2f>(sf::Mouse::getPosition());
            
            auto spritePos = spriteRefWrap.get().getPosition();
            auto angle = CUtility::angleBetweenTwoPoints(spritePos, mousePos);
            spriteRefWrap.get().setRotation(angle);
        }
        
    }

    uint8_t CMouseCtrl::lockObjectVisionOnCursor(sf::Sprite& sprite)
    {
        m_followCursorSubId++;
        m_followCursor.push_back(true);
        m_sprites.push_back(std::ref(sprite));
        
        return m_followCursorSubId;
    }

    void CMouseCtrl::unlockObjectVisionOnCursor(uint8_t subId)
    {
        m_followCursor[subId] = false;
    }

}

#endif