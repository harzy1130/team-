#include "Pch.h"
#include "enemy.h"

Enemy::Enemy()
{
    m_pos = Math::Vector2(0.0f, 0.0f);
    m_mat = Math::Matrix::Identity;
    m_bAlive = false;
}

bool Enemy::Init(float x, float y, float leftX, float rightX,
    float speed, const char* texturePath)
{
    Release();
    SetPos(x, y);
    m_direction = 1;
    SetPatrolRange(leftX, rightX);
    SetMoveSpeed(speed);

    if (texturePath == nullptr || !m_texture.Load(texturePath))
    {
        return false;
    }

    m_bAlive = true;
    CalcMat();
    return true;
}

void Enemy::SetPatrolRange(float leftX, float rightX)
{
    m_leftX = std::min(leftX, rightX);
    m_rightX = std::max(leftX, rightX);
    m_pos.x = std::max(m_leftX, std::min(m_pos.x, m_rightX));
}

void Enemy::SetHp(int hp)
{
    m_hp = std::max(0, hp);
}

void Enemy::SetAtk(int atk)
{
    m_atk = std::max(0, atk);
}

void Enemy::SetDef(int def)
{
    m_def = std::max(0, def);
}

void Enemy::SetMoveSpeed(float speed)
{
    m_moveSpeed = std::max(0.0f, speed);
}

void Enemy::Update()
{
    if (!m_bAlive) return;

    m_pos.x += m_moveSpeed * static_cast<float>(m_direction);

    if (m_pos.x >= m_rightX)
    {
        m_pos.x = m_rightX;
        m_direction = -1;
    }
    else if (m_pos.x <= m_leftX)
    {
        m_pos.x = m_leftX;
        m_direction = 1;
    }

}

void Enemy::CalcMat()
{
    // Turn 180 degrees around Y when moving left (horizontal sprite flip).
    const float angle = (m_direction < 0) ? DirectX::XM_PI : 0.0f;
    m_mat = Math::Matrix::CreateRotationY(angle)
        * Math::Matrix::CreateTranslation(m_pos.x, m_pos.y, 0.0f);
}

void Enemy::Draw()
{
    if (!m_bAlive) return;

    // Refresh the matrix in case SetPos was called after Update.
    CalcMat();
    const auto& info = m_texture.GetInfo();
    SHADER.m_spriteShader.SetMatrix(m_mat);
    SHADER.m_spriteShader.DrawTex(&m_texture,
        Math::Rectangle(0, 0, static_cast<int>(info.Width),
            static_cast<int>(info.Height)), 1.0f);
}

void Enemy::Release()
{
    m_bAlive = false;
    m_texture.Release();
}
