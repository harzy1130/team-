#include "Pch.h"
#include "EnemySkill.h"
#include <cmath>

void C_EnemySkill::Init(const DashSettings& settings)
{
    m_settings = settings;
    // 既存の地形衝突で補正できるよう、突進の移動量をマップチップ半分以下に制限する。
    m_settings.speed = std::max(0.0f, std::min(settings.speed, static_cast<float>(CHIP_RADIUS)));
    m_settings.windupFrames = std::max(0, settings.windupFrames);
    m_settings.dashFrames = std::max(1, settings.dashFrames);
    m_settings.activationRange = std::max(0.0f, settings.activationRange);
    m_settings.damageMultiplier = std::max(0.0f, settings.damageMultiplier);
    ResetEncounter();
}

void C_EnemySkill::ResetEncounter()
{
    m_state = State::Ready;
    m_frames = 0;
    m_direction = 1;
    m_used = false;
    m_hit = false;
    m_damageActive = false;
}

void C_EnemySkill::Cancel()
{
    m_state = m_used ? State::Spent : State::Ready;
    m_frames = 0;
    m_damageActive = false;
}

bool C_EnemySkill::Update(const HitBase& owner, const HitBase* target,
    bool engaged, bool grounded, float& horizontalMove)
{
    m_damageActive = false;
    if (!engaged || target == nullptr)
    {
        ResetEncounter();
        return false;
    }
    if (m_state == State::Spent) return false;

    if (m_state == State::Ready)
    {
        const auto delta = target->GetPos() - owner.GetPos();
        const bool sameHeight = owner.GetHitTop() > target->GetHitBottom()
            && owner.GetHitBottom() < target->GetHitTop();
        if (!grounded || !sameHeight || delta.x * delta.x + delta.y * delta.y
            > m_settings.activationRange * m_settings.activationRange) return false;

        m_used = true;
        m_hit = false;
        m_direction = delta.x < 0.0f ? -1 : 1;
        m_state = State::Windup;
        m_frames = m_settings.windupFrames;
    }

    if (m_state == State::Windup)
    {
        horizontalMove = 0.0f;
        if (m_frames > 0) { --m_frames; return true; }
        m_state = State::Dashing;
        m_frames = m_settings.dashFrames;
    }

    horizontalMove = m_settings.speed * m_direction;
    m_damageActive = true;
    if (--m_frames <= 0) m_state = State::Spent;
    return true;
}

namespace
{
    // 地形衝突で補正された実際の移動経路を使い、ターゲットとの接触を判定する。
    // 補正前の移動量では判定せず、壁の向こうにいるターゲットへの命中を防ぐ。
    bool SweepAxis(float startMin, float startMax, float targetMin, float targetMax,
        float movement, float& entry, float& exit)
    {
        if (std::abs(movement) < 0.0001f)
            return startMin < targetMax && startMax > targetMin;
        float a = (targetMin - startMax) / movement;
        float b = (targetMax - startMin) / movement;
        if (a > b) std::swap(a, b);
        entry = std::max(entry, a);
        exit = std::min(exit, b);
        return entry < exit;
    }
}

bool C_EnemySkill::TryHit(const HitBase& owner, const Math::Vector2& previousPos,
    const HitBase& target)
{
    if (!m_damageActive || m_hit) return false;
    const auto rect = owner.GetRect();
    const auto movement = owner.GetPos() - previousPos;
    float entry = 0.0f, exit = 1.0f;
    if (!SweepAxis(previousPos.x + rect.left, previousPos.x + rect.right,
        target.GetHitLeft(), target.GetHitRight(), movement.x, entry, exit)
        || !SweepAxis(previousPos.y + rect.bottom, previousPos.y + rect.top,
            target.GetHitBottom(), target.GetHitTop(), movement.y, entry, exit)) return false;
    m_hit = true;
    return true;
}

void C_EnemySkill::StopAtWall()
{
    Cancel();
}
