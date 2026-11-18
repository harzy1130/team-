#include "DashSkill.h"
#include"Player/PlayerBase.h"
void C_DashSkill::Init(C_PlayerBase* a_pPlayer)
{
	C_SkillBase::Init(a_pPlayer);
	m_skillCDMax = 60 * 5;
	m_countMax = 10;
	m_bWaitActiveTimer = false;
}

void C_DashSkill::Draw()
{

}

//void C_DashSkill::SkillActivation()
//{
//	if (m_bSkillMotion)return;
//	if (m_skillCD > 0)return;
//	m_pPlayer->SetSkillMoveX(40 * m_pPlayer->GetScaleX());
//	m_skillCD = SkillHasteCalc(m_skillCDMax, m_pPlayer);
//}

void C_DashSkill::OnceActiveSkill()
{
	if(!m_pPlayer) return;
	if (m_bSkillMotion)return;
	if(m_skillCD > 0)return;
	C_SkillBase::OnceActiveSkill();
	m_pPlayer->SetSkillMoveX(40 * m_pPlayer->GetScaleX());
	m_skillCD = SkillHasteCalc(m_skillCDMax, m_pPlayer);
}

void C_DashSkill::WhileActiveSkill()
{
	C_SkillBase::WhileActiveSkill();
}

void C_DashSkill::EndActiveSkill()
{
	C_SkillBase::EndActiveSkill();
	m_pPlayer->SetSkillMoveX(0);
}
