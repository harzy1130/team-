#include "Pch.h"
#include "HitBase.h"
#include"hit.h"

namespace
{
    // ローカル座標の矩形を指定位置のワールド座標へ変換し、整数の RECT として返す。
    RECT WorldRect(const RECT& rect, const Math::Vector2& pos)
    {
        return RECT{static_cast<LONG>(pos.x + rect.left),
            static_cast<LONG>(pos.y + rect.top),
            static_cast<LONG>(pos.x + rect.right),
            static_cast<LONG>(pos.y + rect.bottom)};
    }

    // 小数の位置を保ったまま二つの矩形の重なりを判定する。辺の接触だけでは衝突としない。
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

void HitBase::DrawDebugRect(Math::Color* color)
{
    SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
    SHADER.m_spriteShader.DrawCircle(m_draw.x, m_draw.y, 5, color);

    SHADER.m_spriteShader.DrawLine(m_draw.x + m_rect.left, m_draw.y + m_rect.bottom, m_draw.x + m_rect.right, m_draw.y + m_rect.bottom, color);
    SHADER.m_spriteShader.DrawLine(m_draw.x + m_rect.left, m_draw.y + m_rect.top, m_draw.x + m_rect.right, m_draw.y + m_rect.top, color);
    SHADER.m_spriteShader.DrawLine(m_draw.x + m_rect.left, m_draw.y + m_rect.top, m_draw.x + m_rect.left, m_draw.y + m_rect.bottom, color);
    SHADER.m_spriteShader.DrawLine(m_draw.x + m_rect.right, m_draw.y + m_rect.top, m_draw.x + m_rect.right, m_draw.y + m_rect.bottom, color);
}

// 中心からの半幅・半高さを指定して、ローカル座標の当たり判定矩形を設定する。
void HitBase::SetRect(long halfWidth, long halfHeight)
{
    halfWidth = std::max(0L, halfWidth);
    halfHeight = std::max(0L, halfHeight);
    m_rect = RECT{-halfWidth, halfHeight, halfWidth, -halfHeight};
}
// 不規則な矩形用。-はつけなくてもよい。
void HitBase::SetRect(long left, long top, long right, long bottom)
{
    left = abs(left);
    top = abs(top);
    right = abs(right);
    bottom = abs(bottom);
    m_rect = { -left, top, right, -bottom };
}

void HitBase::ResetHitRect()
{
    m_bHitRect = { false,false,false,false };
}

void HitBase::SetHitLeft(float a_x)
{
    m_pos.x = a_x - m_rect.left;
    m_move.x = 0;
    m_bHitRect.left = true;
}

void HitBase::SetHitTop(float a_y)
{
    m_pos.y = a_y - m_rect.top;
    m_move.y = 0;
    m_bHitRect.top = true;
}

void HitBase::SetHitRight(float a_x)
{
    m_pos.x = a_x - m_rect.right;
    m_move.x = 0;
    m_bHitRect.right = true;
}

void HitBase::SetHitBottom(float a_y)
{
    m_pos.y = a_y - m_rect.bottom;
    m_move.y = 0;
    m_bHitRect.bottom = true;
}

// 現在位置での当たり判定矩形を返す。RECT の座標は整数に変換される。
RECT HitBase::GetNowRect() const
{
    return WorldRect(m_rect, m_pos);
}

// 移動後の予測位置での当たり判定矩形を返す。RECT の座標は整数に変換される。
RECT HitBase::GetFutureRect() const
{
    return WorldRect(m_rect, GetFuturePos());
}

// 現在位置で相手と矩形が重なるかを調べる。辺が接するだけの場合は false を返す。
bool HitBase::IsHit(const HitBase& other) const
{
    return Overlaps(m_rect, m_pos, other.m_rect, other.m_pos);
}

// 双方が移動量を適用した後に、矩形が重なるかどうかを予測する。
bool HitBase::IsFutureHit(const HitBase& other) const
{
    return Overlaps(m_rect, GetFuturePos(), other.m_rect, other.GetFuturePos());
}

// 移動量を現在位置に一度だけ適用し、適用済みの移動量を 0 に戻す。
void HitBase::ApplyMove()
{
    m_pos += m_move;
    m_move = Math::Vector2(0.0f, 0.0f);
}
