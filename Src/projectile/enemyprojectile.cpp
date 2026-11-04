#include "Pch.h"
#include "enemyprojectile.h"
#include "Map/Map.h"
#include "Player/PlayerBase.h"
#include <cmath>

namespace
{
    // 弾の幅・高さを含めた線分判定で、高速移動時のすり抜けを防ぐ。
    bool Sweep(const Math::Vector2& start, const Math::Vector2& move,
        const RECT& body, float left, float bottom, float right, float top, float& time)
    {
        float entry = 0.0f, exit = 1.0f;
        const auto axis = [&](float p, float d, float lo, float hi)
        {
            if (std::abs(d) < 0.0001f) return p >= lo && p <= hi;
            float a = (lo - p) / d, b = (hi - p) / d;
            if (a > b) std::swap(a, b);
            entry = std::max(entry, a); exit = std::min(exit, b);
            return entry <= exit;
        };
        if (!axis(start.x, move.x, left - body.right, right - body.left)
            || !axis(start.y, move.y, bottom - body.top, top - body.bottom)) return false;
        time = entry;
        return true;
    }
}

bool C_EnemyProjectile::Init(const Math::Vector2& position, const Math::Vector2& direction,
    float speed, float damage, int lifetimeFrames)
{
    m_alive = false;
    const float length = direction.Length();
    if (!std::isfinite(position.x) || !std::isfinite(position.y) || !std::isfinite(length)
        || length < 0.001f || !std::isfinite(speed) || speed <= 0.0f
        || !std::isfinite(damage) || damage <= 0.0f || lifetimeFrames <= 0) return false;
    SetPos(position.x, position.y);
    m_velocity = direction * (speed / length);
    m_angle = std::atan2(m_velocity.y, m_velocity.x);
    const float cosine = std::abs(std::cos(m_angle)), sine = std::abs(std::sin(m_angle));
    SetRect(static_cast<long>(std::ceil(14.0f * cosine + 4.0f * sine)),
        static_cast<long>(std::ceil(14.0f * sine + 4.0f * cosine)));
    m_damage = damage; m_frames = lifetimeFrames; m_alive = true;
    return true;
}

void C_EnemyProjectile::Update(C_Map* map, C_PlayerBase* player)
{
    if (!m_alive) return;
    if (map == nullptr || --m_frames < 0) { m_alive = false; return; }
    float wallTime = 2.0f;
    const auto end = m_pos + m_velocity;
    const int firstCol = static_cast<int>(std::floor((std::min(m_pos.x, end.x) + m_rect.left - LEFT_EDGE + CHIP_RADIUS) / CHIP_SIZE));
    const int lastCol = static_cast<int>(std::floor((std::max(m_pos.x, end.x) + m_rect.right - LEFT_EDGE + CHIP_RADIUS) / CHIP_SIZE));
    const int firstRow = static_cast<int>(std::floor((TOP_EDGE + CHIP_RADIUS - std::max(m_pos.y, end.y) - m_rect.top) / CHIP_SIZE));
    const int lastRow = static_cast<int>(std::floor((TOP_EDGE + CHIP_RADIUS - std::min(m_pos.y, end.y) - m_rect.bottom) / CHIP_SIZE));
    // マップ外に出た弾は、負の添字を渡さず破棄する。
    if (firstCol < 0 || lastCol >= MAP_WIDTH || firstRow < 0 || lastRow >= MAP_HEIGHT)
    { m_alive = false; return; }
    for (int row = firstRow; row <= lastRow; ++row)
        for (int col = firstCol; col <= lastCol; ++col)
        {
            if (!map->GetCollisionFlg(row, col)) continue;
            const auto tile = map->GetPosition(row, col);
            float time = 0.0f;
            if (Sweep(m_pos, m_velocity, m_rect, tile.x - CHIP_RADIUS, tile.y - CHIP_RADIUS,
                tile.x + CHIP_RADIUS, tile.y + CHIP_RADIUS, time)) wallTime = std::min(wallTime, time);
        }
    float playerTime = 2.0f;
    if (player != nullptr && player->GetAFlg())
        Sweep(m_pos, m_velocity, m_rect, player->GetHitLeft(), player->GetHitBottom(),
            player->GetHitRight(), player->GetHitTop(), playerTime);
    // 同時刻の場合は壁を優先し、壁越しにダメージを与えない。
    if (playerTime <= 1.0f && playerTime < wallTime)
    {
        m_pos += m_velocity * playerTime;
        player->TakeDamage(m_damage);
        m_alive = false;
    }
    else if (wallTime <= 1.0f) { m_pos += m_velocity * wallTime; m_alive = false; }
    else m_pos = end;
    if (m_frames == 0) m_alive = false;
}

void C_EnemyProjectile::Draw(C_Camera* camera, KdTexture* texture) const
{
    if (!m_alive || texture == nullptr) return;
    const auto draw = camera != nullptr ? m_pos - camera->GetPos() : m_pos;
    // 元画像は左向きなので、右向きに直してから射出角度を適用する。
    const auto matrix = Math::Matrix::CreateScale(-1.0f, 1.0f, 1.0f)
        * Math::Matrix::CreateRotationZ(m_angle)
        * Math::Matrix::CreateTranslation(draw.x, draw.y, 0.0f);
    SHADER.m_spriteShader.SetMatrix(matrix);
    SHADER.m_spriteShader.DrawTex(texture, Math::Rectangle(0, 0, 32, 12), 1.0f);
}
