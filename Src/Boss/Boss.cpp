#include"Boss.h"

void C_Boss::Draw()
{
	if (!m_bAlive)return;
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(0, 0, 250, 250), 1.0f);
}

void C_Boss::Update()
{
	if (!m_bAlive)return;
	Move();
	MatrixUpdate();
}

void C_Boss::Move()
{
	m_angle += 2;
	if (m_angle >= 360)m_angle -= 360;
	float rad = DirectX::XMConvertToRadians(m_angle);
	m_move.x = cosf(rad) * BOSS_SPEED;
	m_move.y = sinf(rad) * BOSS_SPEED;
}

