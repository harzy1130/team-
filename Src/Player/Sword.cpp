#include "Sword.h"
#include "Player1.h"
#include"Boss/BossBase.h"
#include"Enemy/EnemyBase.h"

void C_Sword::Init(C_Player1* a_player, C_Hit* a_pHit, C_Camera* a_pCamera)
{
	m_gap = { 100,0 };
	SetPlayer(a_player);
	SetHitClass(a_pHit);
	SetCamera(a_pCamera);
	m_hitRange = 60;
}

void C_Sword::Update()
{
	m_gap.x = 100 * m_pPlayer->GetScaleX();
	m_pos = m_pPlayer->GetPos();
	m_pos += m_gap;
	m_draw = m_pos - m_pCamera->GetPos();
	if(m_pPlayer->GetAtkFlg()&&!m_bAtkHit)
	{
		// UŒ‚Žž‚Ìˆ—
		ResetAlreadyHit();
		m_hitRange = 60;
		NewCircleObjects({0,0}, 50.0f);
		m_bAtkHit = true;
	}
	else
	{
		DeleteCircleObjects(GetCircleObjectsCount()-1);
		m_bAtkHit = false;
		m_hitRange = 0;
		
	}
	if(m_bAtkHit)
	{
		m_pHit->BossHitCheck(this);
		m_pHit->EnemyHitCheck(this);
		
	}
}

void C_Sword::IfHitBoss(C_BossBase* a_pBoss)
{
	a_pBoss->TakeDamage((int)m_pPlayer->CrtDmgCalc());
}

void C_Sword::IfHitEnemy(EnemyBase* a_pEnemy)
{
	a_pEnemy->TakeDamage((int)m_pPlayer->CrtDmgCalc(), m_pPlayer->GetPos().x);
}
