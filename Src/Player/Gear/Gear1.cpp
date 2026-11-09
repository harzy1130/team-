#include"Gear1.h"

void C_Gear1::Init(C_PlayerBase* a_pPlayer)
{
	C_GearBase::Init(a_pPlayer);
	m_status.atk = m_atkStatus;
	m_activeCDMax = 60 * 8;
	m_activeTimerMax = 60 * 5;
}

void C_Gear1::OnceActiveGear()
{
	C_GearBase::OnceActiveGear();
}

void C_Gear1::WhileActiveGear()
{
	if (m_bGearActive)
	{
		m_status.atk = m_atkStatus + m_atkBonus;
	}
	C_GearBase::WhileActiveGear();
}

void C_Gear1::EndActiveGear()
{
	C_GearBase::EndActiveGear();
	m_status.atk = m_atkStatus;
}
