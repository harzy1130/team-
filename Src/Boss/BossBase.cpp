#include "BossBase.h"

void BossBase::MatrixUpdate()
{
	m_pos += m_move;
	m_draw = m_pos;

	m_sMat = Math::Matrix::CreateScale(m_xScale, m_yScale, 1);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
}
