#pragma once

// World coordinates use +Y upwards. m_rect is relative to m_pos.
class HitBase
{
public:
    virtual ~HitBase() = default;

    void SetPos(float x, float y) { m_pos = Math::Vector2(x, y); }
    void SetMove(float x, float y) { m_move = Math::Vector2(x, y); }
    void SetRect(long halfWidth, long halfHeight);
    Math::Vector2 GetPos() const { return m_pos; }
    Math::Vector2 GetMove() const { return m_move; }
    Math::Vector2 GetFuturePos() const { return m_pos + m_move; }
    RECT GetRect() const { return m_rect; }
    RECT GetNowRect() const;
    RECT GetFutureRect() const;

    float GetHitTop() const { return m_pos.y + m_rect.top; }
    float GetHitBottom() const { return m_pos.y + m_rect.bottom; }
    float GetHitLeft() const { return m_pos.x + m_rect.left; }
    float GetHitRight() const { return m_pos.x + m_rect.right; }

    // Only positive-area overlap counts as a hit; touching edges do not.
    bool IsHit(const HitBase& other) const;
    // Predict overlap after both objects apply their pending moves.
    bool IsFutureHit(const HitBase& other) const;
    // Apply once after any collision correction, then clear the pending move.
    void ApplyMove();

protected:
    RECT m_rect{};
    Math::Vector2 m_pos{0.0f, 0.0f};
    Math::Vector2 m_move{0.0f, 0.0f};
};
