#pragma once
#include "EnemyBase.h"
#include "Skill/EnemySkill.h"

class C_PlayerBase;

// Enemy1 固有の初期設定と使用スキルを管理する。
// 共通の移動・描画・当たり判定・各種アクセサーは基底クラスから継承する。
class Enemy1 : public EnemyBase
{
public:
    // 同じ種類の敵でも、配置と巡回範囲は個体ごとに設定する。
    struct SpawnSettings
    {
        float x = LEFT_EDGE + 2.5f * PTEX_SIZE;
        float y = 0.0f; // 地面探索の基準となる高さ
        float leftX = LEFT_EDGE + PTEX_SIZE + PTEX_SIZE / 4.0f;
        float rightX = LEFT_EDGE + 6.0f * PTEX_SIZE - PTEX_SIZE / 2.0f;
    };
    
    // ゲーム開始前に一度だけコマを整え、ステージ再入場でも結果を保持する。
    static bool PreloadSprites();
    bool Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map = nullptr);
    bool Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Map* map, const SpawnSettings& spawn);
    void SetDamageTestEnabled(bool enabled) { m_damageTestEnabled = enabled; }
    
    void Update();
    void Draw() override;
    void Release() override;
    bool IsNormalAttacking() const { return m_normalState != NormalState::Ready; }
    bool IsAttackWarningActive() const override
    {
        return m_openingSkill.IsWindingUp() || m_openingSkill.IsDashing()
            || m_normalState == NormalState::Windup || m_normalState == NormalState::Strike;
    }
    bool IsRetreating() const { return m_spacingState == SpacingState::Retreat; }
    bool IsWaitingToAttack() const { return m_spacingState == SpacingState::Wait; }
    // Scene から有効状態とターゲットを受け取り、ステージ状態はこのクラスで管理する。
    void Update(C_PlayerBase* target, bool active);
    bool HasUsedOpening() const { return m_openingSkill.HasUsedOpening(); }
    bool IsOpeningWindup() const { return m_openingSkill.IsWindingUp(); }
    bool IsOpeningDash() const { return m_openingSkill.IsDashing(); }
    void SetOpeningSkill(const C_EnemySkill::DashSettings& settings) { m_openingSkill.Init(settings); }
protected:
    bool GetActionMove(float& horizontalMove) override;

private:
    enum class NormalState { Ready, Windup, Strike, Recovery };
    // 攻撃後に距離を取り、再攻撃が可能になるまでは追跡を止める。
    enum class SpacingState { Approach, Retreat, Wait };
    void BeginRetreat();
    void ResetCombatSpacing();
    bool UpdateSpacingMove(float& horizontalMove);
    bool HasRetreatGround(float horizontalMove) const;
    SpacingState m_spacingState = SpacingState::Approach;
    int m_retreatFrames = 0;
    int m_retreatDirection = -1;
    bool InNormalAttackRange() const;
    void CancelNormalAttack();
    void UpdateAnimation(const Math::Vector2& previousPos);
    NormalState m_normalState = NormalState::Ready;
    int m_normalFrames = 0;
    int m_normalCooldown = 0;
    int m_normalDirection = 1;
    bool m_normalHitPending = false;
    int m_spriteFrame = 0;
    int m_animationTicks = 0;
    int m_visualRecovery = 0;
    bool m_wasVisualAction = false;
    bool m_wasVisualWindup = false;
    C_EnemySkill m_openingSkill;
    C_PlayerBase* m_player = nullptr;
    C_Map* m_map = nullptr;
    bool m_wasActive = false;
    int m_stage = -1;
    bool m_damageTestEnabled = true;
    bool m_testHitWasDown = false;
};
