#include "Gear2.h"
#include"Player/PlayerBase.h"

void C_Gear2::Init(C_PlayerBase* a_pPlayer)
{
	C_GearBase::Init(a_pPlayer);
	m_status.hpRegene = m_hpRegeneStatus;
	m_bHavePassiveCD = false;
	m_bWaitPassiveTimer = false;
	m_bPassiveOnly = true;
}

bool C_Gear2::PassiveTiming()
{
	if (m_pPlayer->GetHitRect().bottom)return true;
	else EndPassiveGear();
	return false;
}

void C_Gear2::OncePassiveGear()
{
	if(!m_bWhilePassive)m_pPlayer->Heal(m_heal);
	C_GearBase::OncePassiveGear();
}

void C_Gear2::WhilePassiveGear()
{
	if (m_bWhilePassive) {
		m_status.def = m_defBonus;
	}
}

void C_Gear2::EndPassiveGear()
{
	m_status.def = 0;
	C_GearBase::EndPassiveGear();
}
