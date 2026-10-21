#pragma once
#include "hit/HitBase.h"

// 敵専用の開幕攻撃。使用状態は敵ごとに個別に保持する。
// プレイヤー用の SkillBase とは独立して管理する。
class C_EnemySkill
{
public:
    struct DashSettings
    {
        float speed = 14.0f;           // 固定更新 1 回あたりの移動量（ピクセル）
        int windupFrames = 24;
        int dashFrames = 20;
        float activationRange = 320.0f;
        float damageMultiplier = 1.0f;
    };

    void Init(const DashSettings& settings);
    void ResetEncounter();             // 戦闘終了やステージ切り替え時に使用状態をリセットする。
    void Cancel();                     // 中断しても、同じ戦闘中は再使用できない。
    bool Update(const HitBase& owner, const HitBase* target,
        bool engaged, bool grounded, float& horizontalMove);
    bool TryHit(const HitBase& owner, const Math::Vector2& previousPos, const HitBase& target);
    void StopAtWall();

    bool HasUsedOpening() const { return m_used; }
    bool IsWindingUp() const { return m_state == State::Windup; }
    bool IsDashing() const { return m_damageActive; }
    int GetDirection() const { return m_direction; }
    float GetSpeed() const { return m_settings.speed; }
    float GetDamageMultiplier() const { return m_settings.damageMultiplier; }

private:
    enum class State { Ready, Windup, Dashing, Spent };
    DashSettings m_settings;
    State m_state = State::Ready;
    int m_frames = 0;
    int m_direction = 1;
    bool m_used = false;
    bool m_hit = false;
    bool m_damageActive = false;
};
