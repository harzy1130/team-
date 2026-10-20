#include "SkillBase.h"
#include"Player/PlayerBase.h"
void C_Skill::Init()
{
	m_skillCDMax=120;
	m_skillCD = 0;
	m_bSkillMotion = false;
	m_count = 10;
}

void C_Skill::Update()
{
	UpdateCooldown();
	if (m_bSkillMotion) {
		m_count--;
		if (m_count <= 0) {
			m_bSkillMotion = false;
			m_count = 10;
			m_PB->SetSkillMoveX(0);
		}
	}
}

void C_Skill::Draw(Math::Vector2 a_pos)
{

}

void C_Skill::SkillActivation(C_PlayerBase*a_pPlayer)
{
	if (m_bSkillMotion)return;
	if (!m_skillCD <= 0)return;
	m_bSkillMotion = true;
	m_CurrentState = S_State::Casting;
	m_PB = a_pPlayer;
	m_PB->SetSkillMoveX(40*m_PB->GetScaleX());
	m_skillCD = SkillHasteCalc(m_skillCDMax,m_PB);
	return;
}


float C_Skill::SkillHasteCalc(float a_CDMax, C_PlayerBase *a_pPlayer)
{
	return a_CDMax * (100.0f / (100.0f + a_pPlayer->GetSkillHaste()));
}
	

void C_Skill::UpdateCooldown()
{
	m_skillCD--;
	if (m_skillCD <= 0) {
		m_skillCD = 0;
	}
	
}

