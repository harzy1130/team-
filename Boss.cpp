#include"Boss.h"

void Boss::Draw()
{
	if (!m_bAlive)return;
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(0, 0, 250, 250), 1.0f);
}

void Boss::Update()
{
	if (!m_bAlive)return;
	Move();
	MatrixUpdate();
}

void Boss::Move()
{
	m_angle += 2;
	if (m_angle >= 360)m_angle -= 360;
	float rad = DirectX::XMConvertToRadians(m_angle);
	m_move.x = cosf(rad) * MOVE_SPEED;
	m_move.y = sinf(rad) * MOVE_SPEED;
}

void Boss::MatrixUpdate()
{
	m_pos += m_move;
	m_draw = m_pos;

	m_sMat = Math::Matrix::CreateScale(m_xScale, m_yScale, 1);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
}

void Boss::Init()
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_draw = { 0,0 };
	m_bAlive = true;
	m_xScale = 1;
	m_yScale = 1;
	m_angle = 0;

	m_sMat = Math::Matrix::CreateScale(m_xScale,m_yScale,0);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x,m_draw.y,0);
	m_mat = m_sMat * m_rMat * m_tMat;
}
