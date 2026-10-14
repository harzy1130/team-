#pragma once

// Standalone horizontal patrol enemy for the skeleton framework.
class Enemy
{
public:
    Enemy();

    // speed: pixels per Update (the existing game uses a fixed-frame loop).
    // Returns false if the texture could not be loaded.
    bool Init(float x = 0.0f, float y = 0.0f,
        float leftX = -200.0f, float rightX = 200.0f,
        float speed = 2.0f, const char* texturePath = "Texture/Chara.png");
    void Update();
    void CalcMat();
    void Draw();
    void Release();

    void SetPatrolRange(float leftX, float rightX);
    void SetHp(int hp);
    void SetAtk(int atk);
    void SetDef(int def);
    void SetMoveSpeed(float speed);

    int GetHp() const { return m_hp; }
    int GetAtk() const { return m_atk; }
    int GetDef() const { return m_def; }
    float GetMoveSpeed() const { return m_moveSpeed; }
    void SetPos(float x, float y) { m_pos = Math::Vector2(x, y); }
    Math::Vector2 GetPos() const { return m_pos; }
    bool GetAlive() const { return m_bAlive; }

private:
    KdTexture m_texture;
    Math::Vector2 m_pos;
    Math::Matrix m_mat;
    bool m_bAlive = false;
    float m_leftX = -200.0f;
    float m_rightX = 200.0f;
    int m_hp = 100;
    int m_atk = 10;
    int m_def = 5;
    float m_moveSpeed = 2.0f;
    int m_direction = 1; // 1: right, -1: left
};
