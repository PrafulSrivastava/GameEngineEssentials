#ifndef CKEYBOARD_CTRL_HPP
#define CKEYBOARD_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>

namespace GameEngine{

    enum class eKeyBinding : uint8_t {
        oneToOne = 1,
        manyToOne
    };

    constexpr auto InvalidIndex = -1;

    template<typename Key, typename Action, eKeyBinding Binding = eKeyBinding::oneToOne>
    class CKeyboardCtrl{
    public:
        CKeyboardCtrl() = default;
        ~CKeyboardCtrl() = default;
        CKeyboardCtrl(const CKeyboardCtrl&) = delete;
        CKeyboardCtrl& operator =(const CKeyboardCtrl&) = delete;
        CKeyboardCtrl(CKeyboardCtrl&&) = delete;
        CKeyboardCtrl& operator =(CKeyboardCtrl&&) = delete;

        void mapKeyToAction(Key, Action);
        void executeAction(const Key&);

    private:
        int8_t getIndexOfKey(const Key& key);

        std::vector<Key> m_keys;
        std::vector<std::vector<Action>> m_callbcks;
        uint8_t m_keyIndex{0};
    };

    template<typename Key, typename Action, eKeyBinding Binding>
    void CKeyboardCtrl<Key, Action, Binding>::mapKeyToAction(Key key, Action callback)
    {
        auto index = getIndexOfKey(key);
        if(index == InvalidIndex){            
            m_keys.push_back(std::move(key));
            m_callbcks.push_back({std::move(callback)});              
            m_keyIndex++;
        } else if(Binding != eKeyBinding::oneToOne) {
            m_callbcks[index].push_back(std::move(callback));
        }      
    }

    template<typename Key, typename Action, eKeyBinding Binding>
    void CKeyboardCtrl<Key, Action, Binding>::executeAction(const Key& key)
    {
        auto index = getIndexOfKey(key);
        if(index == InvalidIndex){
            return ;
        }
        std::for_each(m_callbcks[index].cbegin(), m_callbcks[index].cend(), [&](const Action &callback){ callback(); });
    }

    template<typename Key, typename Action, eKeyBinding Binding>
    int8_t CKeyboardCtrl<Key, Action, Binding>::getIndexOfKey(const Key& key)
    {
        auto itr = std::find(m_keys.cbegin(), m_keys.cend(), key);
        if(itr != m_keys.cend()){
            return std::distance(m_keys.cbegin(), itr);
        }
        else{
            return InvalidIndex;
        }
    }
}

#endif