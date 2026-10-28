#include "effect.h"
#include"Player/PlayerBase.h"
#include"Enemy/EnemyBase.h"
void C_Effect::InitPlayer(C_PlayerBase* a_pPlayer)
{
	m_baseAtk = a_pPlayer->GetATK();
	m_baseDef = a_pPlayer->GetDEF();
	m_bPoison = true;
	m_bCombustion = false;
	m_bSutan = false;
	m_bSuppress = false;
	m_bAtkUp = false;
	m_bDefUp = false;
	m_bAtkDown = false;
	m_bDefDown = false;
	m_poisonTime = 10;
	m_combustionTime = 0;
	m_sutanTime = 0;
	m_atkUpTime = 0;
	m_defUpTime = 0;
	m_atkDownTime = 0;
	m_defDownTime = 0;
}

void C_Effect::InitEnemy(EnemyBase* a_pEnemy)
{
	m_baseAtk = a_pEnemy->GetAtk();
	m_bPoison = false;
	m_bCombustion = false;
	m_bSutan = false;
	m_bSuppress = false;
	m_bAtkUp = false;
	m_bAtkDown = false;
	m_poisonTime = 0;
	m_combustionTime = 0;
	m_sutanTime = 0;
	m_atkUpTime = 0;
	m_defUpTime = 0;
	m_atkDownTime = 0;
}

void C_Effect::UpdatePlayer(C_PlayerBase* a_pPlayer)
{
	if (!a_pPlayer->GetAFlg())return;
	

	if (m_bPoison)//毒
	{
		if(m_poisonCount<60)
		{
			m_poisonCount++;
		}
		else
		{
			a_pPlayer->SetHP(a_pPlayer->GetHP() - 1);
			m_poisonCount = 0;
		}
		m_poisonTime -= 1.0f / 60.0f;
		if (m_poisonTime <= 0)
		{
			m_poisonTime = 0;
			m_poisonCount = 0;
			m_bPoison = false;
		}
	}
	if (m_bCombustion)//炎上
	{
		if(m_combustionCount<60)
		{
			m_combustionCount++;
		}
		else
		{
			a_pPlayer->SetHP(a_pPlayer->GetHP() - 3);
			m_combustionCount = 0;
		}
		m_combustionTime -= 1.0f / 60.0f;
		if (m_combustionTime <= 0)
		{
			m_combustionTime = 0;
			m_combustionCount = 0;
			m_bCombustion = false;
		}
	}
	if(m_bSutan)//スタン
	{
		m_sutanTime -= 1.0f / 60.0f;
		if (m_sutanTime <= 0)
		{
			m_sutanTime = 0;
			m_bSutan = false;
		}
	}
	if (m_bSuppress)//スキル封印
	{
		m_suppressTime -= 1.0f / 60.0f;
		if (m_suppressTime <= 0)
		{
			m_suppressTime = 0;
			m_bSuppress = false;
		}
	}
	if (m_bAtkUp)//攻撃力上昇
	{
		m_atkUpTime -= 1.0f / 60.0f;
		a_pPlayer->SetATK(m_baseAtk * 1.5f);
		if(m_atkUpTime <= 0)
		{
			m_atkUpTime = 0;
			a_pPlayer->SetATK(m_baseAtk);
			m_bAtkUp = false;
		}
	}
	if (m_bDefUp)//防御力上昇
	{
		m_defUpTime -= 1.0f / 60.0f;
		a_pPlayer->SetDEF(m_baseDef * 1.5f);
		if(m_defUpTime <= 0)
		{
			m_defUpTime = 0;
			a_pPlayer->SetDEF(m_baseDef);
			m_bDefUp = false;
		}
	}
	if (m_bAtkDown)//攻撃力低下
	{
		m_atkDownTime -= 1.0f / 60.0f;
		a_pPlayer->SetATK(m_baseAtk * 0.5f);
		if (m_atkDownTime <= 0)
		{
			m_atkDownTime = 0;
			a_pPlayer->SetATK(m_baseAtk);
			m_bAtkDown = false;
		}
	}
	if(m_bDefDown)//防御力低下
	{
		m_defDownTime -= 1.0f / 60.0f;
		a_pPlayer->SetDEF(m_baseDef * 0.5f);
		if (m_defDownTime <= 0)
		{
			m_defDownTime = 0;
			a_pPlayer->SetDEF(m_baseDef);
			m_bDefDown = false;
		}
	}
	if (m_bAtkUp && m_bAtkDown)//元のステータスに戻す
	{
		a_pPlayer->SetATK(m_baseAtk);
	}
	if (m_bDefUp && m_bDefDown)//元のステータスに戻す
	{
		a_pPlayer->SetDEF(m_baseDef);
	}
}

void C_Effect::UpdateEnemy(EnemyBase* a_pEnemy)
{
	if (!a_pEnemy->GetAlive())return;


	if (m_bPoison)//毒
	{
		a_pEnemy->SetHp(a_pEnemy->GetHp() - 0.1f);
		m_poisonTime -= 1.0f / 60.0f;
		if (m_poisonTime <= 0)
		{
			m_poisonTime = 0;
			m_bPoison = false;
		}
	}
	if (m_bCombustion)//炎上
	{
		a_pEnemy->SetHp(a_pEnemy->GetHp() - 0.2f);
		m_combustionTime -= 1.0f / 60.0f;
		if (m_combustionTime <= 0)
		{
			m_combustionTime = 0;
			m_bCombustion = false;
		}
	}
	if (m_bSutan)//スタン
	{
		m_sutanTime -= 1.0f / 60.0f;
		if (m_sutanTime <= 0)
		{
			m_sutanTime = 0;
			m_bSutan = false;
		}
	}
	if (m_bSuppress)//スキル封印
	{
		m_suppressTime -= 1.0f / 60.0f;
		if (m_suppressTime <= 0)
		{
			m_suppressTime = 0;
			m_bSuppress = false;
		}
	}
	if (m_bAtkUp)//攻撃力上昇
	{
		m_atkUpTime -= 1.0f / 60.0f;
		a_pEnemy->SetAtk(m_baseAtk * 1.5f);
		if (m_atkUpTime <= 0)
		{
			m_atkUpTime = 0;
			a_pEnemy->SetAtk(m_baseAtk);
			m_bAtkUp = false;
		}
	}
	if (m_bAtkDown)//攻撃力低下
	{
		m_atkDownTime -= 1.0f / 60.0f;
		a_pEnemy->SetAtk(m_baseAtk * 0.5f);
		if (m_atkDownTime <= 0)
		{
			m_atkDownTime = 0;
			a_pEnemy->SetAtk(m_baseAtk);
			m_bAtkDown = false;
		}
	}
	if(m_bAtkUp&&m_bAtkDown)
	{
		a_pEnemy->SetAtk(m_baseAtk);
	}
}
