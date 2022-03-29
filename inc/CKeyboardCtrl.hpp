#ifndef CKEYBOARD_CTRL_HPP
#define CKEYBOARD_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "CInputCtrl.hpp"

namespace GameEngine
{
    using ParamType = int32_t;
    using Action = std::function<void(ParamType)>;
    class CKeyboardCtrl : public CInputCtrl<sf::Keyboard::Key, Action, ParamType>
    {
    public:
        CKeyboardCtrl() = default;
        ~CKeyboardCtrl() = default;
        CKeyboardCtrl(const CKeyboardCtrl &) = delete;
        CKeyboardCtrl &operator=(const CKeyboardCtrl &) = delete;
        CKeyboardCtrl(CKeyboardCtrl &&) = delete;
        CKeyboardCtrl &operator=(CKeyboardCtrl &&) = delete;

        // get key bindings
        // combo executions

    private:
    };
}

#endif