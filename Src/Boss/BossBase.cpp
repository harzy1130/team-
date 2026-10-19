#include "BossBase.h"

void C_BossBase::MatrixUpdate()
{
	m_pos += m_move;
	m_draw = m_pos - m_pCamera->GetPos();

	m_sMat = Math::Matrix::CreateScale(m_xScale, m_yScale, 1);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
}

void C_BossBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera)
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_draw = { 0,0 };
	m_bAlive = true;
	m_xScale = 1;
	m_yScale = 1;
	m_angle = 0;

	m_sMat = Math::Matrix::CreateScale(m_xScale, m_yScale, 0);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
	m_pHit = a_pHit;
	m_pCamera = a_pCamera;
}
