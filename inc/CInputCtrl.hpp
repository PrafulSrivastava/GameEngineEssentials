#ifndef CINPUT_CTRL_HPP
#define CINPUT_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

namespace GameEngine
{

    enum class eKeyBindingType : int32_t
    {
        oneToOne = 1,
        manyToOne
    };

    constexpr int32_t InvalidIndex = -1;

    template <typename Key, typename Action, typename Parameter, eKeyBindingType Binding = eKeyBindingType::oneToOne>
    class CInputCtrl
    {
    public:
        CInputCtrl() = default;
        ~CInputCtrl() = default;
        CInputCtrl(const CInputCtrl &) = delete;
        CInputCtrl &operator=(const CInputCtrl &) = delete;
        CInputCtrl(CInputCtrl &&) = delete;
        CInputCtrl &operator=(CInputCtrl &&) = delete;

        void mapKeyToAction(Key, Action, Parameter &);
        void freeUpKey(Key);
        void executeAction(const Key &);

    private:
        int32_t getIndexOfKey(const Key &key);

        std::vector<Key> m_keys;
        std::vector<std::vector<std::pair<Action, Parameter &>>> m_callbcks;
        int32_t m_keyIndex{0};
    };

    template <typename Key, typename Action, typename Parameter, eKeyBindingType Binding>
    void CInputCtrl<Key, Action, Parameter, Binding>::mapKeyToAction(Key key, Action callback, Parameter &param)
    {
        auto index = getIndexOfKey(key);
        if (index == InvalidIndex)
        {
            m_keys.push_back(std::move(key));
            m_callbcks.push_back({{callback, param}});
            m_keyIndex++;
        }
        else if (Binding != eKeyBindingType::oneToOne || m_callbcks[index].size() == 0)
        {
            m_callbcks[index].push_back({callback, param});
        }
    }

    template <typename Key, typename Action, typename Parameter, eKeyBindingType Binding>
    void CInputCtrl<Key, Action, Parameter, Binding>::executeAction(const Key &key)
    {
        auto index = getIndexOfKey(key);
        if (index == InvalidIndex)
        {
            return;
        }
        std::for_each(m_callbcks[index].cbegin(), m_callbcks[index].cend(), [&](const std::pair<Action, Parameter> &callbackPair)
                      { callbackPair.first(callbackPair.second); });
    }

    template <typename Key, typename Action, typename Parameter, eKeyBindingType Binding>
    int32_t CInputCtrl<Key, Action, Parameter, Binding>::getIndexOfKey(const Key &key)
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
    template <typename Key, typename Action, typename Parameter, eKeyBindingType Binding>
    void CInputCtrl<Key, Action, Parameter, Binding>::freeUpKey(Key key)
    {
        auto index = getIndexOfKey(key);
        if (index != InvalidIndex)
        {
            m_callbcks[index].clear();
        }
    }
}

#endif