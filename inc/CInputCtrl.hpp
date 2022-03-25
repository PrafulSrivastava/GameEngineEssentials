#ifndef CINPUT_CTRL_HPP
#define CINPUT_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

namespace GameEngine
{

    enum class eKeyBindingType : uint8_t
    {
        oneToOne = 1,
        manyToOne
    };

    constexpr auto InvalidIndex = -1;

    template <typename Key, typename Action, eKeyBindingType Binding = eKeyBindingType::oneToOne>
    class CInputCtrl
    {
    public:
        CInputCtrl() = default;
        ~CInputCtrl() = default;
        CInputCtrl(const CInputCtrl &) = delete;
        CInputCtrl &operator=(const CInputCtrl &) = delete;
        CInputCtrl(CInputCtrl &&) = delete;
        CInputCtrl &operator=(CInputCtrl &&) = delete;

        void mapKeyToAction(Key, Action);
        void executeAction(const Key &);

    private:
        int8_t getIndexOfKey(const Key &key);

        std::vector<Key> m_keys;
        std::vector<std::vector<Action>> m_callbcks;
        uint8_t m_keyIndex{0};
    };

    template <typename Key, typename Action, eKeyBindingType Binding>
    void CInputCtrl<Key, Action, Binding>::mapKeyToAction(Key key, Action callback)
    {
        auto index = getIndexOfKey(key);
        if (index == InvalidIndex)
        {
            m_keys.push_back(std::move(key));
            m_callbcks.push_back({std::move(callback)});
            m_keyIndex++;
        }
        else if (Binding != eKeyBindingType::oneToOne)
        {
            m_callbcks[index].push_back(std::move(callback));
        }
    }

    template <typename Key, typename Action, eKeyBindingType Binding>
    void CInputCtrl<Key, Action, Binding>::executeAction(const Key &key)
    {
        auto index = getIndexOfKey(key);
        if (index == InvalidIndex)
        {
            return;
        }
        std::for_each(m_callbcks[index].cbegin(), m_callbcks[index].cend(), [&](const Action &callback)
                      { callback(); });
    }

    template <typename Key, typename Action, eKeyBindingType Binding>
    int8_t CInputCtrl<Key, Action, Binding>::getIndexOfKey(const Key &key)
    {
        auto itr = std::find(m_keys.cbegin(), m_keys.cend(), key);
        if (itr != m_keys.cend())
        {
            return std::distance(m_keys.cbegin(), itr);
        }
        else
        {
            return InvalidIndex;
        }
    }
}

#endif