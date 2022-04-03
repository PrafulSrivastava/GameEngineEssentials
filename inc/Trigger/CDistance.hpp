#ifndef CDISTANCE_HPP
#define CDISTANCE_HPP

#include "CUtility.hpp"
#include "Globals.hpp"

namespace GameEngine
{
    namespace Trigger
    {
        template <typename Entity, typename NotifyAction>
        class CDistance
        {
            struct DistanceInfo
            {
                DistanceInfo(GameType::Distance Magnitude, GameType::Distance thetaXY, GameType::Distance thetaZ, Component comp, NotifyAction callback, GameType::shortBool isContinuous) : isContinuous(isContinuous), magnitude(Magnitude), thetaxy(thetaXY), thetaz(thetaZ), mComponent(std::move(comp)), notifyAction(std::move(callback))
                {
                }

                GameType::shortBool isContinuous;
                Component mComponent;
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

            uint32_t RegisterDistanceNotification(GameType::Distance, GameType::Distance, GameType::Distance, Entity &, NotifyAction, GameType::shortBool);
            void RemoveDistanceNotification(uint32_t);

            GameType::shortBool isDistanceTriggerRegistered();
            void StartMoving();

        private:
            std::vector<DistanceInfo> m_NotifyActions;
            std::vector<std::reference_wrapper<Entity>> m_listRegistered;
            std::vector<Entity> m_prevPosition;

            void OnCheckDistance();
        };

        template <typename Entity, typename NotifyAction>
        uint32_t CDistance<Entity, NotifyAction>::RegisterDistanceNotification(GameType::Distance Magnitude, GameType::Distance thetaXY, GameType::Distance thetaZ, Entity &obj, NotifyAction callback, GameType::shortBool isContinuous)
        {
            m_NotifyActions.push_back(DistanceInfo(Magnitude, thetaXY, thetaZ, CUtility::getComponent(Magnitude, thetaXY, thetaZ, Component(obj.getPosition().x, obj.getPosition().y, 0)), callback, isContinuous));

            m_prevPosition.push_back(obj);
            m_listRegistered.push_back(obj);

            return m_listRegistered.size() - 1;
        }

        template <typename Entity, typename NotifyAction>
        void CDistance<Entity, NotifyAction>::RemoveDistanceNotification(uint32_t subId)
        {
            m_prevPosition.erase(m_prevPosition.begin() + subId);
            m_listRegistered.erase(m_listRegistered.begin() + subId);
            m_NotifyActions.erase(m_NotifyActions.begin() + subId);
        }

        template <typename Entity, typename NotifyAction>
        void CDistance<Entity, NotifyAction>::StartMoving()
        {
            OnCheckDistance();
        }

        template <typename Entity, typename NotifyAction>
        void CDistance<Entity, NotifyAction>::OnCheckDistance()
        {
            for (auto i = 0; i < m_listRegistered.size(); i++)
            {
                if (!CUtility::Compare(m_prevPosition[i].getPosition(), m_listRegistered[i].get().getPosition()))
                {
                    if (CUtility::Compare(m_listRegistered[i].get().getPosition().x, m_NotifyActions[i].mComponent.xComponent) && CUtility::Compare(m_listRegistered[i].get().getPosition().y, m_NotifyActions[i].mComponent.yComponent))
                    {
                        m_NotifyActions[i].notifyAction();
                        if (m_NotifyActions[i].isContinuous.count())
                        {
                            m_NotifyActions[i].mComponent = CUtility::getComponent(m_NotifyActions[i].magnitude, m_NotifyActions[i].thetaxy, m_NotifyActions[i].thetaz, Component(m_listRegistered[i].get().getPosition().x, m_listRegistered[i].get().getPosition().y, 0));
                        }

                        else
                        {
                            RemoveDistanceNotification(i);
                        }
                    }
                    else
                    {
                        m_prevPosition[i] = m_listRegistered[i];
                    }
                }
            }
        }

        template <typename Entity, typename NotifyAction>
        GameType::shortBool CDistance<Entity, NotifyAction>::isDistanceTriggerRegistered()
        {
            return !m_listRegistered.empty();
        }

    }
}

#endif