#include "GearBase.h"
#include"Player/PlayerBase.h"


void C_GearBase::Init(C_PlayerBase* a_pPlayer)
{
	m_status = { 0,0,0,0,0,0,0,0 };
	m_pPlayer = a_pPlayer;
	m_activeCDMax = 300;
	m_activeCD = 0;
	m_activeTimerMax = 300;
	m_activeTimer = 0;
	m_bGearActive = false;
	m_bHaveActiveCD	 = true;

	m_passiveCDMax = 0;
	m_passiveCD = 0;
	m_passiveTimerMax = 0;
	m_passiveTimer = 0;
	m_bGearPassive = false;
	m_bHavePassiveCD = true;
}

void C_GearBase::Draw()
{
		
}

void C_GearBase::OnceActiveGear()
{
	if(m_bGearActive || m_activeCD > 0)return;
	m_bGearActive = true;
	m_activeCD = m_activeCDMax;
	m_activeTimer = m_activeTimerMax;
}

void C_GearBase::WhileActiveGear()
{
	m_activeTimer--;
	if(m_activeTimer < 0)
	{
		m_activeTimer = 0;
		EndActiveGear();
	}
}

void C_GearBase::EndActiveGear()
{
	m_bGearActive = false;
}

bool C_GearBase::PassiveTiming()
{
	if (m_bGearPassive || m_passiveCD > 0)
	{
		return true;
	}
	return false;
}

void C_GearBase::OncePassiveGear()
{
	m_bGearPassive = true;
	m_passiveCD = m_passiveCDMax;
	m_passiveTimer = m_passiveTimerMax;
}

void C_GearBase::WhilePassiveGear()
{
	m_passiveTimer--;
	if(m_passiveTimer < 0)
	{
		m_passiveTimer = 0;
		EndPassiveGear();
	}
}

void C_GearBase::EndPassiveGear()
{
	m_bGearPassive = false;
}


void C_GearBase::UpdateCD()
{
	if (!m_bHaveActiveCD)goto ActiveCDSkip;
	m_activeCD--;
	if(m_activeCD < 0)
	{
		m_activeCD = 0;
	}
ActiveCDSkip:;

	if(!m_bHavePassiveCD)goto PassiveCDSkip;
	m_passiveCD--;
	if(m_passiveCD < 0)
	{
		m_passiveCD = 0;
	}
PassiveCDSkip:;
}

