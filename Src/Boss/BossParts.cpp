#include "BossParts.h"

void C_BossParts::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)
{
	C_BossBase::Init(a_pHit, a_pCamera, a_pPlayer);
	m_size = 1;
	if (m_pParent != nullptr)
	{
		m_bAlive = m_pParent->GetAlive();
		m_invincibleFrames = m_pParent->GetInvincibleFrames();
		m_startPos = m_pParent->GetPos();
		m_size = m_pParent->GetSize();
		m_scale = { m_size * m_lr, m_size };
	}
	m_pos = m_startPos + m_offsetPos;
}

void C_BossParts::Update()
{
	if (!m_bAlive)return;
	if (m_invincibleFrames > 0) --m_invincibleFrames;
	Move();
	if(m_pParent != nullptr)
	{
		m_bAlive = m_pParent->GetAlive();
		m_invincibleFrames = m_pParent->GetInvincibleFrames();
		m_startPos = m_pParent->GetPos();
		m_parentAngle = m_pParent->GetAngle();
		m_lr = m_pParent->GetLR();
		m_size = m_pParent->GetSize();
		m_scale = { m_size * m_lr, m_size };
	}
	m_angle = m_parentAngle + m_offsetAngle * m_lr;
	m_pos.x = m_startPos.x + cosf(ToRadians(m_parentAngle)) * m_offsetPos.x * m_lr - sinf(ToRadians(m_parentAngle)) * m_offsetPos.y;
	m_pos.y = m_startPos.y + sinf(ToRadians(m_parentAngle)) * m_offsetPos.x * m_lr + cosf(ToRadians(m_parentAngle)) * m_offsetPos.y;
	MatrixUpdate();
}