#include "Pch.h"
#include "HitBase.h"

namespace
{
    RECT WorldRect(const RECT& rect, const Math::Vector2& pos)
    {
        return RECT{static_cast<LONG>(pos.x + rect.left),
            static_cast<LONG>(pos.y + rect.top),
            static_cast<LONG>(pos.x + rect.right),
            static_cast<LONG>(pos.y + rect.bottom)};
    }

    bool Overlaps(const RECT& a, const Math::Vector2& ap,
        const RECT& b, const Math::Vector2& bp)
    {
        // Compare in floating point so subpixel positions are preserved.
        const float left = std::max(ap.x + a.left, bp.x + b.left);
        const float right = std::min(ap.x + a.right, bp.x + b.right);
        const float bottom = std::max(ap.y + a.bottom, bp.y + b.bottom);
        const float top = std::min(ap.y + a.top, bp.y + b.top);
        return left < right && bottom < top;
    }
}

void HitBase::SetRect(long halfWidth, long halfHeight)
{
    halfWidth = std::max(0L, halfWidth);
    halfHeight = std::max(0L, halfHeight);
    m_rect = RECT{-halfWidth, halfHeight, halfWidth, -halfHeight};
}

RECT HitBase::GetNowRect() const
{
    return WorldRect(m_rect, m_pos);
}

RECT HitBase::GetFutureRect() const
{
    return WorldRect(m_rect, GetFuturePos());
}

bool HitBase::IsHit(const HitBase& other) const
{
    return Overlaps(m_rect, m_pos, other.m_rect, other.m_pos);
}

bool HitBase::IsFutureHit(const HitBase& other) const
{
    return Overlaps(m_rect, GetFuturePos(), other.m_rect, other.GetFuturePos());
}

void HitBase::ApplyMove()
{
    m_pos += m_move;
    m_move = Math::Vector2(0.0f, 0.0f);
}
