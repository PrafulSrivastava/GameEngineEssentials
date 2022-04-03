#ifndef CSPRITE_WRAPPER_HPP
#define CSPRITE_WRAPPER_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "Globals.hpp"

namespace GameEngine
{
    template <typename Entity>
    class CEntityWrapper : public Entity
    {
    public:
        CEntityWrapper(float directionOfPerpendicular = 0);
        ~CEntityWrapper() = default;
        CEntityWrapper(const CEntityWrapper &) = default;
        CEntityWrapper &operator=(const CEntityWrapper &) = default;
        CEntityWrapper(CEntityWrapper &&) = default;
        CEntityWrapper &operator=(CEntityWrapper &&) = default;

        int32_t getEntityId();
        bool isActive();
        void setLiveStatus(bool);
        float getDirectionOfPerpendicular();

    private:
        int32_t m_Id{};
        GameType::shortBool m_isActive;
        float m_directionOfPerpendicular;
    };

    template <typename Entity>
    CEntityWrapper<Entity>::CEntityWrapper(float directionOfPerpendicular)
        : m_Id(CUtility::getFreshElementId()), m_directionOfPerpendicular(directionOfPerpendicular)
    {
        m_isActive.set(0);
    }

    template <typename Entity>
    bool CEntityWrapper<Entity>::isActive()
    {
        return m_isActive.all();
    }

    template <typename Entity>
    void CEntityWrapper<Entity>::setLiveStatus(bool status)
    {
        (status)? m_isActive.set(0): m_isActive.reset(0);
    }

    template <typename Entity>
    int32_t CEntityWrapper<Entity>::getEntityId()
    {
        return m_Id;
    }

    template <typename Entity>
    float CEntityWrapper<Entity>::getDirectionOfPerpendicular()
    {
        return m_directionOfPerpendicular;
    }
}

#endif