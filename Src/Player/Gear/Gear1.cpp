#include"Gear1.h"
#include"Player/PlayerBase.h"
#include"Effect/Effect.h"
#include"UI/UIManager.h"
#include"Enemy/EnemyBase.h"
#include"Boss/BossBase.h"

void C_Gear1::Init(C_PlayerBase* a_pPlayer, C_UIManager* a_pUIManager)
{
	C_GearBase::Init(a_pPlayer, a_pUIManager);
	m_status.atk = m_atkStatus;
	m_activeCDMax = 60 * 8;
	m_activeTimerMax = 60 * 4;
	m_bHavePassiveCD = true;
	m_bWaitActiveTimer = true;
	m_pEffect = a_pPlayer->GetEffect();
}

void C_Gear1::OnceActiveGear()
{
	m_pEffect->SetAtkUp(m_activeTimerMax / 60, m_atkBonusP);
}

void C_Gear1::WhileActiveGear()
{
	if (m_bWhileActive)
	{
		m_status.crtRate = m_crtBonus;
	}
	m_pUIManager->AddPlayerStatusChip("CrtUp", &m_gearIconTex, &m_activeTimerMax, &m_activeTimer);
	C_GearBase::WhileActiveGear();
}

void C_Gear1::EndActiveGear()
{
	C_GearBase::EndActiveGear();
	m_status.atk = m_atkStatus;
	m_status.crtRate = 0;
}

float C_Gear1::OnBossHitGear(C_BossBase* a_pBoss)
{
	a_pBoss->TakeDamage(m_onHitDmg);
	return m_onHitDmg;
}

float C_Gear1::OnEnemyHitGear(EnemyBase* a_pEnemy)
{
	a_pEnemy->TakeDamage(m_onHitDmg, m_pPlayer->GetPos().x);
	return m_onHitDmg;
}

float C_Gear1::OnHurtEnemyGear(EnemyBase* a_pEnemy)
{
	a_pEnemy->TakeDamage(m_onHurtDmg, m_pPlayer->GetPos().x);
	m_pUIManager->CreateDamageUI((int)m_onHurtDmg, a_pEnemy->GetPos(), m_pPlayer->GetPos().x < a_pEnemy->GetPos().x, false );
	return m_onHurtDmg;
}

float C_Gear1::OnHurtBossGear(C_BossBase* a_pBoss)
{
	a_pBoss->TakeDamage(m_onHurtDmg);
	m_pUIManager->CreateDamageUI((int)m_onHurtDmg, a_pBoss->GetPos(), m_pPlayer->GetPos().x < a_pBoss->GetPos().x, false );	
	return m_onHurtDmg;
}

