#ifndef CATTRIBUTE_HPP
#define CATTRIBUTE_HPP

#include <bitset>
#include "CEntityWrapper.hpp"

namespace GameEngine::Attribute
{
    constexpr auto AttributeCount = 13;

    enum class AttributeTypes : uint8_t
    {
        reflects = 0,
        fades,
        tail,
        explodes,
        fluid,
        spins,
        obeysGravity = 7,
        expands,
        shrinks,
        multiplies,
        penetrable,
        penetrative,
        destructive = 13
    };

    template <typename Entity>
    class CAttribute
    {
    public:
        CAttribute() = default;
        ~CAttribute() = default;
        CAttribute(const CAttribute &) = delete;
        CAttribute &operator=(const CAttribute &) = delete;
        CAttribute(CAttribute &&) = delete;
        CAttribute &operator=(CAttribute &&) = delete;
        void setAttribute(AttributeTypes);
        bool getAttributeState(AttributeTypes);
        void applyAttributes(CEntityWrapper<Entity> &);

    private:
        std::bitset<AttributeCount> m_attributeBitSet;
    };

    template <typename Entity>
    void CAttribute<Entity>::setAttribute(AttributeTypes type)
    {
        m_attributeBitSet.set(static_cast<uint8_t>(type));
    }

    template <typename Entity>
    bool CAttribute<Entity>::getAttributeState(AttributeTypes type)
    {
        return (m_attributeBitSet[static_cast<uint8_t>(type)] == 1) ? true : false;
    }

    template <typename Entity>
    void CAttribute<Entity>::applyAttributes(CEntityWrapper<Entity> &entity)
    {
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::fades)])
        {
            auto color = entity.getFillColor();
            if (color.a == 0)
            {
                entity.setLiveStatus(false);
            }
            else
            {
                color.a--;
                entity.setFillColor(color);
            }
        }
        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::tail)])
        {
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::explodes)])
        {
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::fluid)])
        {
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::spins)])
        {
            entity.rotate(Projectile::SpinAngle);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::obeysGravity)])
        {
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::expands)])
        {
            entity.scale(Projectile::EnlargeSizeRatio);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::shrinks)])
        {
            entity.scale(Projectile::ShrinkSizeRatio);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::multiplies)])
        {
            entity.scale(Projectile::ShrinkSizeRatio);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::penetrable)])
        {
            entity.scale(Projectile::ShrinkSizeRatio);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::penetrative)])
        {
            entity.scale(Projectile::ShrinkSizeRatio);
        }

        if (m_attributeBitSet[static_cast<uint8_t>(AttributeTypes::destructive)])
        {
        }
    }
}

#endif