#ifndef CTIMER_HPP
#define CTIMER_HPP

#include <bitset>
#include <unordered_map>
#include <condition_variable>
#include <mutex>

namespace GameEngine
{
    namespace Trigger
    {
        template <typename Time, typename NotifyAction>
        class CTimer
        {
            struct NotifyCallback
            {
                GameType::shortBool isContinuous;
                uint32_t timerValue, timerBackup;
                NotifyAction notifyAction;
            };

        public:
            CTimer();
            ~CTimer() = default;
            CTimer(const CTimer &) = delete;
            CTimer &operator=(const CTimer &) = delete;
            CTimer(CTimer &&) = delete;
            CTimer &operator=(CTimer &&) = delete;

            // Registering for the timer notification
            uint32_t registerForTimerNotification(uint32_t, NotifyAction, GameType::shortBool);
            // To stop the timer.. Mandatory once we created the object of this class
            void stopTimer();
            // To remove particular timer id from the list
            void unregisterFromTimerNotification(uint32_t);

        private:
            std::unique_ptr<std::thread> m_timerThread, m_notifierThread;
            std::unordered_map<uint8_t, NotifyCallback> m_notifyCallback;
            GameType::shortBool m_IsIntervalFinished, m_TimerStop;            
            uint8_t m_subscriptionId;
            std::condition_variable m_cvInterval;
            std::mutex m_mutexInterval;

            // Starts the timer.. it will sleep for some time and notify
            void startTimer();
            // Performs the respective functionality once timer value finished
            void notifyTimeIntervals();
        };

        template <typename Time, typename NotifyAction>
        CTimer<Time, NotifyAction>::CTimer() : m_IsIntervalFinished(0), m_TimerStop(0), m_subscriptionId(0)
        {
            m_timerThread = std::make_unique<std::thread>(&CTimer::startTimer, this);
            m_notifierThread = std::make_unique<std::thread>(&CTimer::notifyTimeIntervals, this);
        }

        template <typename Time, typename NotifyAction>
        void CTimer<Time, NotifyAction>::stopTimer()
        {
            m_notifyCallback.clear();

            m_TimerStop.set();
            m_IsIntervalFinished.set();
            m_cvInterval.notify_all();

            if (nullptr != m_timerThread && m_timerThread->joinable())
            {
                m_timerThread->join();
            }
            if (nullptr != m_notifierThread && m_notifierThread->joinable())
            {
                m_notifierThread->join();
            }
        }

        template <typename Time, typename NotifyAction>
        void CTimer<Time, NotifyAction>::startTimer()
        {
            while (!m_TimerStop.all())
            {
                if (!m_IsIntervalFinished.all())
                {
                    std::unique_lock<std::mutex> lock(m_mutexInterval);
                    std::this_thread::sleep_for(Time(GameEngine::Trigger::TimeInterval));
                    m_IsIntervalFinished.set();
                }
                m_cvInterval.notify_all();
            }
        }

        template <typename Time, typename NotifyAction>
        void CTimer<Time, NotifyAction>::notifyTimeIntervals()
        {
            while (!m_TimerStop.all())
            {
                {
                    std::unique_lock<std::mutex> lock(m_mutexInterval);
                    m_cvInterval.wait(lock, [&]
                                      { return m_IsIntervalFinished.all(); });
                    m_IsIntervalFinished.reset();
                }

                for (auto &it : m_notifyCallback)
                {
                    it.second.timerValue -= GameEngine::Trigger::TimeInterval;
                    if (it.second.timerValue == 0)
                    {
                        it.second.notifyAction();
                        if (!it.second.isContinuous.all())
                            m_notifyCallback.erase(it.first);

                        else
                            it.second.timerValue = it.second.timerBackup;
                    }
                }
            }
        }

        template <typename Time, typename NotifyAction>
        uint32_t CTimer<Time, NotifyAction>::registerForTimerNotification(uint32_t time, NotifyAction callback, GameType::shortBool isContinuous)
        {
            m_subscriptionId++;
            m_notifyCallback[m_subscriptionId].isContinuous = isContinuous;
            m_notifyCallback[m_subscriptionId].timerBackup = time;
            m_notifyCallback[m_subscriptionId].timerValue = std::move(time);
            m_notifyCallback[m_subscriptionId].notifyAction = std::move(callback);

            return m_subscriptionId;
        }

        template <typename Time, typename NotifyAction>
        void CTimer<Time, NotifyAction>::unregisterFromTimerNotification(uint32_t subId)
        {
            m_notifyCallback.erase(subId);
        }

    }

}

#endif