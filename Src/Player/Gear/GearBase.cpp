#include "GearBase.h"
#include"Player/PlayerBase.h"


void C_GearBase::Init(C_PlayerBase* a_pPlayer)
{
	m_status = { 0,0,0,0,0,0,0,0 };
	m_pPlayer = a_pPlayer;
	m_activeCDMax = 0;
	m_activeCD = 0;
	m_activeTimerMax = 0;
	m_activeTimer = 0;
	m_bHaveActiveCD = true;
	m_bWhileActive = false;

	m_passiveCDMax = 0;
	m_passiveCD = 0;
	m_passiveTimerMax = 0;
	m_passiveTimer = 0;
	m_bHavePassiveCD = true;
	m_bWhilePassive = false;
}

void C_GearBase::Draw()
{
		
}

void C_GearBase::OnceActiveGear()
{
	if (m_bPassiveOnly)return;
	if (m_activeCD > 0)return;
	m_bWhileActive = true;
	m_activeTimer = m_activeTimerMax;
	if (!m_bWaitActiveTimer) 
	{
		m_activeCD = m_pPlayer->CalcSkillHaste(m_activeCDMax);
	}
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
	if (!m_bWhileActive)return;
	m_bWhileActive = false;
	if (m_bWaitActiveTimer)
	{
		m_activeCD = m_pPlayer->CalcSkillHaste(m_activeCDMax);
	}
}

bool C_GearBase::PassiveTiming()
{
	if (m_bWhilePassive || m_passiveCD > 0)
	{
		return true;
	}
	return false;
}

void C_GearBase::OncePassiveGear()
{
	m_bWhilePassive = true;
	m_passiveTimer = m_passiveTimerMax;
	if(!m_bWaitPassiveTimer)
	{
		m_passiveCD = m_pPlayer->CalcSkillHaste(m_passiveCDMax);
	}
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
	if (!m_bWhilePassive)return;
	m_bWhilePassive = false;
	if(m_bWaitPassiveTimer)
	{
		m_passiveCD = m_pPlayer->CalcSkillHaste(m_passiveCDMax);
	}
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

float C_GearBase::GetCalcActiveCDMax()
{
	return m_pPlayer->CalcSkillHaste(m_activeCDMax);	
}

float C_GearBase::GetCalcPassiveCDMax()
{
	return m_pPlayer->CalcSkillHaste(m_passiveCDMax); 
}

