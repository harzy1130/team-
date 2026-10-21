#pragma once
#include "hit/HitBase.h"


class C_Map;

// Horizontal patrol enemy; shared collision data comes from HitBase.
class EnemyBase  : public HitBase
{
public:

    EnemyBase();

    // speed: pixels per Update (the existing game uses a fixed-frame loop).
    // Returns false if the texture could not be loaded.
    bool Init(C_Hit* a_pHit = nullptr, C_Camera* a_pCamera = nullptr, float x = 0.0f, float y = 0.0f,
        float leftX = -200.0f, float rightX = 200.0f,
        float speed = 2.0f, const char* texturePath = "Texture/Chara.png");
    void Update();
    void CalcMat();
    void Draw();
    void Release();

    // Scene supplies a live target, or nullptr when the player is dead.
    void SetTarget(const HitBase* target) { m_target = target; }
    void SetVisionMap(C_Map* map) { m_visionMap = map; }
    void SetVision(float range, float fullAngleDegrees, float loseDistance);
    void SetChaseSpeed(float speed) { m_chaseSpeed = std::max(0.0f, speed); }
    bool IsChasing() const { return m_chasing; }
    void ResetAwareness() { m_target = nullptr; m_chasing = false; }
    void DrawVision();
    // Reposition above the current stage floor and clear old falling velocity.
    bool PlaceOnMap(C_Map* map);

    // Returns false when dead, invulnerable, or damage is not positive.
    bool TakeDamage(int damage, float attackFromX);
    bool IsInvincible() const { return m_invincibleFrames > 0; }
    bool IsHurt() const { return m_hurtFrames > 0; }

    void SetPatrolRange(float leftX, float rightX);
    void SetHp(int hp);
    void SetAtk(int atk);
    void SetMoveSpeed(float speed);

    int GetHp() const { return m_hp; }
    int GetAtk() const { return m_atk; }
    float GetMoveSpeed() const { return m_moveSpeed; }
    bool GetAlive() const { return m_bAlive; }


private:
    // 重力と地形衝突を処理して移動を一度だけ適用し、横方向の衝突結果を返す。
    bool MoveWithCollision(float horizontalMove);
    float TraceVision(float directionX, float directionY, float distance) const;
    bool CanSeeTarget() const;
    const HitBase* m_target = nullptr;
    C_Map* m_visionMap = nullptr;
    bool m_chasing = false;
    float m_visionRange = 320.0f;
    float m_visionHalfAngle = 0.523598776f; // 60 degrees total
    float m_loseDistance = 560.0f;
    float m_chaseSpeed = 4.0f;
    KdTexture m_texture;

    Math::Matrix m_mat;
    bool m_bAlive = false;
    float m_spawnX = 0.0f;
    float m_leftX = -200.0f;
    float m_rightX = 200.0f;
    int m_hp = 100;
    int m_atk = 10;
    float m_moveSpeed = 2.0f;
    int m_direction = 1; // 1: right, -1: left
    int m_invincibleFrames = 0;
    int m_hurtFrames = 0;
    float m_knockbackX = 0.0f;
    float m_velocityY = 0.0f;
};
