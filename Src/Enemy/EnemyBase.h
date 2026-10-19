#pragma once
#include "hit/HitBase.h"
#include"Camera/Camera.h"
#include"Hit/Hit.h"

class C_Hit;
class C_Camera;
// Horizontal patrol enemy; shared collision data comes from HitBase.
class EnemyBase  : public HitBase
{
public:

    EnemyBase();

    // speed: pixels per Update (the existing game uses a fixed-frame loop).
    // Returns false if the texture could not be loaded.
    bool Init(float x = 0.0f, float y = 0.0f,
        float leftX = -200.0f, float rightX = 200.0f,
        float speed = 2.0f, const char* texturePath = "Texture/Chara.png");
    void Update();
    void CalcMat();
    void Draw();
    void Release();

    // Returns false when dead, invulnerable, or damage is not positive.
    bool TakeDamage(int damage, float attackFromX);
    bool IsInvincible() const { return m_invincibleFrames > 0; }
    bool IsHurt() const { return m_hurtFrames > 0; }

    void SetPatrolRange(float leftX, float rightX);
    void SetHp(int hp);
    void SetAtk(int atk);
    void SetMoveSpeed(float speed);
    void SetHitClass(C_Hit* a_pHit);
    void SetCameraClass(C_Camera* a_pCamera);

    int GetHp() const { return m_hp; }
    int GetAtk() const { return m_atk; }
    float GetMoveSpeed() const { return m_moveSpeed; }
    bool GetAlive() const { return m_bAlive; }


private:
    KdTexture m_texture;

    Math::Matrix m_mat;
    bool m_bAlive = false;
    float m_leftX = -200.0f;
    float m_rightX = 200.0f;
    int m_hp = 100;
    int m_atk = 10;
    float m_moveSpeed = 2.0f;
    int m_direction = 1; // 1: right, -1: left
    int m_invincibleFrames = 0;
    int m_hurtFrames = 0;
    float m_knockbackX = 0.0f;

protected:
    C_Camera* m_pCamera;
    C_Hit* m_pHit;
};
