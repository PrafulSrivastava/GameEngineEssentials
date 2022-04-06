#ifndef CDISTANCE_HPP
#define CDISTANCE_HPP

#include "CUtility.hpp"
#include "Globals.hpp"
#include "CEntityWrapper.hpp"

namespace GameEngine::Trigger
{
    constexpr auto InvalidIndex = -1;
    template <typename Entity, typename NotifyAction>
    class CDistance
    {
        struct DistanceInfo
        {
            DistanceInfo(GameType::Distance Magnitude, GameType::Distance thetaXY, GameType::Distance thetaZ, sf::Vector3f comp, NotifyAction callback, GameType::shortBool isContinuous) : isContinuous(isContinuous), magnitude(Magnitude), thetaxy(thetaXY), thetaz(thetaZ), component(std::move(comp)), notifyAction(std::move(callback))
            {
            }

            GameType::shortBool isContinuous;
            sf::Vector3f component;
            NotifyAction notifyAction;
            GameType::Distance magnitude, thetaxy, thetaz;
        };

    public:
        CDistance() = default;
        ~CDistance() = default;
        CDistance(const CDistance &) = delete;
        CDistance &operator=(const CDistance &) = delete;
        CDistance(CDistance &&) = delete;
        CDistance &operator=(CDistance &&) = delete;

        [[nodiscard]] uint32_t registerForDistanceNotification(GameType::Distance, GameType::Distance, GameType::Distance, CEntityWrapper<Entity> &, NotifyAction, GameType::shortBool);
        void unregisterFromDistanceNotification(uint32_t);
        void monitorDistance();
        int32_t getIndexOfKey(int32_t);

    private:
        int32_t m_subId{0};
        std::vector<DistanceInfo> m_NotifyActions;
        std::vector<int32_t> m_listsubIds;
        std::vector<std::reference_wrapper<CEntityWrapper<Entity>>> m_listRegistered;
        std::vector<sf::Vector2f> m_prevPosition;
    };

    template <typename Entity, typename NotifyAction>
    uint32_t CDistance<Entity, NotifyAction>::registerForDistanceNotification(GameType::Distance Magnitude, GameType::Distance thetaXY, GameType::Distance thetaZ, CEntityWrapper<Entity> &obj, NotifyAction callback, GameType::shortBool isContinuous)
    {

        auto destination = CUtility::getResultantPosition(Magnitude, thetaXY, thetaZ, {obj.getPosition().x, obj.getPosition().y, 0});
        m_NotifyActions.push_back(DistanceInfo(Magnitude, thetaXY, thetaZ, destination, callback, isContinuous));

        m_prevPosition.push_back(obj.getPosition());
        m_listRegistered.push_back(obj);

        m_subId++;
        m_listsubIds.push_back(m_subId);

        return m_subId;
    }

    template <typename Entity, typename NotifyAction>
    void CDistance<Entity, NotifyAction>::unregisterFromDistanceNotification(uint32_t subId)
    {
        auto index = getIndexOfKey(subId);

        if (index != InvalidIndex)
        {
            m_prevPosition.erase(m_prevPosition.begin() + index);
            m_listRegistered.erase(m_listRegistered.begin() + index);
            m_NotifyActions.erase(m_NotifyActions.begin() + index);
            m_listsubIds.erase(m_listsubIds.begin() + index);
        }
    }

    template <typename Entity, typename NotifyAction>
    int32_t CDistance<Entity, NotifyAction>::getIndexOfKey(int32_t subId)
    {
        auto itr = std::find(m_listsubIds.cbegin(), m_listsubIds.cend(), subId);
        if (itr != m_listsubIds.cend())
        {
            return std::distance(m_listsubIds.cbegin(), itr);
        }
        else
        {
            return InvalidIndex;
        }
    }

    template <typename Entity, typename NotifyAction>
    void CDistance<Entity, NotifyAction>::monitorDistance()
    {
        for (auto i = 0; i < m_listRegistered.size(); i++)
        {
            if (m_prevPosition[i] != m_listRegistered[i].get().getPosition())
            {

                if (CUtility::compareApproximately(m_listRegistered[i].get().getPosition(), {m_NotifyActions[i].component.x, m_NotifyActions[i].component.y}))
                {
                    m_NotifyActions[i].notifyAction();
                    if (m_NotifyActions[i].isContinuous.all())
                    {
                        m_NotifyActions[i].component = CUtility::getResultantPosition(m_NotifyActions[i].magnitude, m_NotifyActions[i].thetaxy, m_NotifyActions[i].thetaz, {m_listRegistered[i].get().getPosition().x, m_listRegistered[i].get().getPosition().y, 0});
                    }
                    else
                    {
                        unregisterFromDistanceNotification(i);
                    }
                }
                else
                {
                    m_prevPosition[i] = m_listRegistered[i].get().getPosition();
                }
            }
        }
    }
}

#endif