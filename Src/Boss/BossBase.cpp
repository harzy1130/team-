#include "BossBase.h"

void C_BossBase::MatrixUpdate()
{
	m_pos += m_move;
	m_move.x = 0;
	m_draw = m_pos - m_pCamera->GetPos();

	float x, y;
	x = fabs(m_scale.x) / m_scale.x;
	y = fabs(m_scale.y) / m_scale.y;
	m_sMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 1);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x + m_gap.x * x, m_draw.y + m_gap.y * y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
}

void C_BossBase::Gravity()
{
	m_move.y -= GRAVITY;
}

void C_BossBase::MapCollision()
{
	if (m_move.y > CHIP_SIZE)m_move.y = CHIP_SIZE;
	if (m_pHit != nullptr) m_pHit->MapHitCheck(this);
	MatrixUpdate();
}

void C_BossBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera)
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_draw = { 0,0 };
	m_bAlive = true;
	m_angle = 0;
	m_animationCnt = 0;

	m_sMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);

}
