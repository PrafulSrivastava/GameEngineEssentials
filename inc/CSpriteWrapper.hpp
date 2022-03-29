#ifndef CSPRITE_WRAPPER_HPP
#define CSPRITE_WRAPPER_HPP

#include <SFML/Graphics.hpp>
#include <functional>
#include <vector>
#include "Globals.hpp"

namespace GameEngine
{
    class CSpriteWrapper : public sf::Sprite
    {
    public:
        CSpriteWrapper();
        ~CSpriteWrapper() = default;
        CSpriteWrapper(const CSpriteWrapper &) = default;
        CSpriteWrapper &operator=(const CSpriteWrapper &) = default;
        CSpriteWrapper(CSpriteWrapper &&) = default;
        CSpriteWrapper &operator=(CSpriteWrapper &&) = default;

        int32_t getSpriteId();
    private:
        int32_t m_Id{};
    };

    CSpriteWrapper::CSpriteWrapper()
        : m_Id(CUtility::getFreshElementId())
    {
    }

    int32_t CSpriteWrapper::getSpriteId()
    {
        return m_Id;
    }
}

#endif