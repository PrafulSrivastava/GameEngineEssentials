#ifndef CKEYBOARD_CTRL_HPP
#define CKEYBOARD_CTRL_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "CInputCtrl.hpp"

namespace GameEngine::Controller
{
    template <typename Input>
    class CKeyboardCtrl : public CInputCtrl<sf::Keyboard::Key, std::function<void(Input)>, Input, eKeyBindingType::manyToOne>
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