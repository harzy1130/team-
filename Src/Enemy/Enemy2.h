#pragma once
#include "EnemyBase.h"
#include "Skill/EnemySkill.h"
#include "projectile/enemyprojectile.h"

class C_PlayerBase;
class C_Map;

// 蠍座の小型敵。通常射撃と、接近時に一度だけ使う後方跳躍を管理する。
class Enemy2 : public EnemyBase
{
public:
    struct SpawnSettings
    {
        float x = 896.0f;
        float y = 0.0f;
        float leftX = 800.0f;
        float rightX = 1056.0f;
    };
    struct AttackSettings
    {
        float range = 480.0f;
        float projectileSpeed = 10.0f;
        int windupFrames = 24;
        int releaseFrames = 6;
        int recoveryFrames = 12;
        int cooldownFrames = 72;
    };

    static bool PreloadSprites();
    bool Init(C_Hit* hit, C_Camera* camera, C_Map* map = nullptr);
    bool Init(C_Hit* hit, C_Camera* camera, C_Map* map, const SpawnSettings& spawn);
    void Update(C_PlayerBase* player, bool active);
    void Draw() override;
    void Release() override;
    void SetDamageTestEnabled(bool enabled) { m_damageTestEnabled = enabled; }
    void SetAttackSettings(const AttackSettings& settings);
    void SetRetreatSettings(const C_EnemyBackstepSkill::Settings& settings) { m_retreat.Init(settings); }
    bool HasUsedRetreat() const { return m_retreat.HasUsed(); }
    bool IsRetreating() const { return m_retreat.IsActive(); }
    bool IsShooting() const { return m_shotState != ShotState::Ready; }
    std::size_t GetProjectileCount() const { return m_projectiles.size(); }

protected:
    bool GetActionMove(float& horizontalMove) override;

private:
    enum class ShotState { Ready, Windup, Release, Recovery };
    void CancelShot();
    void FireNeedle();
    void UpdateAnimation(const Math::Vector2& previousPos);
    AttackSettings m_attack;
    C_EnemyBackstepSkill m_retreat;
    C_PlayerBase* m_player = nullptr;
    C_Map* m_map = nullptr;
    KdTexture m_needleTexture;
    std::vector<C_EnemyProjectile> m_projectiles;
    ShotState m_shotState = ShotState::Ready;
    Math::Vector2 m_aimPoint{0.0f, 0.0f};
    int m_shotFrames = 0;
    int m_cooldown = 0;
    int m_shotDirection = 1;
    bool m_firePending = false;
    int m_spriteFrame = 0;
    int m_animationTicks = 0;
    bool m_damageTestEnabled = false;
    bool m_testHitWasDown = false;
};
