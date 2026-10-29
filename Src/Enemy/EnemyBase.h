#pragma once
#include "hit/HitBase.h"

class C_Map;

// 左右に巡回する敵。共通の当たり判定データは HitBase を使用する。
class EnemyBase  : public HitBase
{
public:

    EnemyBase();

    // speed は固定更新 1 回あたりの移動量（ピクセル）を表す。
    // テクスチャの読み込みに失敗した場合は false を返す。
    bool Init(C_Hit* a_pHit = nullptr, C_Camera* a_pCamera = nullptr, float x = 0.0f, float y = 0.0f,
        float leftX = -200.0f, float rightX = 200.0f,
        float speed = 2.0f, const char* texturePath = "Texture/Chara.png");
    void Update();
    void CalcMat();
    void Draw();
    void Release();

    // 生存中のターゲットを設定する。プレイヤーが死亡した場合は nullptr を渡す。
    void SetTarget(const HitBase* target) { m_target = target; }
    void SetVisionMap(C_Map* map) { m_visionMap = map; }
    void SetVision(float range, float fullAngleDegrees, float loseDistance);
    void SetChaseSpeed(float speed) { m_chaseSpeed = std::max(0.0f, speed); }
    bool IsChasing() const { return m_chasing; }
    void ResetAwareness() { m_target = nullptr; m_chasing = false; }
    void DrawVision();
    // 現在のステージの地面上へ再配置し、以前の落下速度をリセットする。
    bool PlaceOnMap(C_Map* map);

    // 死亡中・無敵中・ダメージが 0 以下の場合は false を返す。
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


protected:
    // 固有行動の移動も、共通の重力・地形衝突処理で補正する。
    // 基底クラス自体には攻撃スキルを持たせない。
    virtual bool GetActionMove(float& horizontalMove) { return false; }
    bool IsGrounded() const { return m_bHitRect.bottom; }
    void FaceDirection(int direction) { m_direction = direction < 0 ? -1 : 1; }

private:
    // 重力と地形衝突を処理して移動を一度だけ適用し、横方向の衝突結果を返す。
    bool MoveWithCollision(float horizontalMove);
    float TraceVision(float directionX, float directionY, float distance) const;
    bool CanSeeTarget() const;
    const HitBase* m_target = nullptr;
    C_Map* m_visionMap = nullptr;
    bool m_chasing = false;
    float m_visionRange = 320.0f;
    float m_visionHalfAngle = 0.523598776f; // 視野の全角は 60 度
    float m_loseDistance = 560.0f;
    float m_chaseSpeed = 4.0f;
    KdTexture m_texture;

    Math::Matrix m_mat;
    bool m_bAlive = false;
    float m_spawnX = 0.0f;
    float m_spawnSearchY = 0.0f; // この高さより下にある地面を探す。
    float m_leftX = -200.0f;
    float m_rightX = 200.0f;
    int m_hp = 100;
    int m_atk = 10;
    float m_moveSpeed = 2.0f;
    int m_direction = 1; // 1: 右向き、-1: 左向き
    int m_invincibleFrames = 0;
    int m_hurtFrames = 0;
    float m_knockbackX = 0.0f;
    float m_velocityY = 0.0f;
};
