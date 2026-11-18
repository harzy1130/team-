#include "SkillBase.h"
#include"Player/PlayerBase.h"
void C_SkillBase::Init(C_PlayerBase* a_pPlayer)
{
	m_pPlayer = a_pPlayer;
	m_skillCD = 0;
	m_bSkillMotion = false;
	m_count = 10;
}

void C_SkillBase::Update()
{
	UpdateCooldown();
	if (m_bSkillMotion) {
		m_count--;
		if (m_count <= 0) {
			m_bSkillMotion = false;
			m_count = 10;
			m_pPlayer->SetSkillMoveX(0);
		}
	}
}

void C_SkillBase::Draw()
{

}

//void C_SkillBase::SkillActivation()
//{
//	if (m_bSkillMotion)return;
//	if (m_skillCD > 0)return;
//	m_bSkillMotion = true;
//	m_CurrentState = S_State::Casting;
//	m_pPlayer->SetSkillMoveX(40 * m_pPlayer->GetScaleX());
//	m_skillCD = SkillHasteCalc(m_skillCDMax, m_pPlayer);
//	return;
//}

void C_SkillBase::OnceActiveSkill()
{
	if (m_bSkillMotion)return;
	if (m_skillCD > 0)return;
	m_count = m_countMax;
	m_bSkillMotion = true;
	if (!m_bWaitActiveTimer) {
		m_skillCD = SkillHasteCalc(m_skillCDMax, m_pPlayer);
	}
	return;
}

void C_SkillBase::WhileActiveSkill()
{
	m_count--;
	if (m_count <= 0) {
		EndActiveSkill();
		m_count = 0;
	}
}

void C_SkillBase::EndActiveSkill()
{
	if(!m_bSkillMotion)return;
	m_bSkillMotion = false;
	if (m_bWaitActiveTimer) {
		m_skillCD = SkillHasteCalc(m_skillCDMax, m_pPlayer);
	}
}



//ŒvŽZ
float C_SkillBase::SkillHasteCalc(float a_CDMax, C_PlayerBase *a_pPlayer)
{
	return a_CDMax * (100.0f / (100.0f + a_pPlayer->GetSkillHaste()));
}
	

void C_SkillBase::UpdateCooldown()
{
	m_skillCD--;
	if (m_skillCD <= 0) {
		m_skillCD = 0;
	}

}

