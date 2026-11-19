#pragma once
#include "GearBase.h"


class C_Effect;
class C_Gear1 : public C_GearBase
{
public:
	C_Gear1(): C_GearBase("Texture/AtkUpGearIcon.png") {};
	~C_Gear1() {};
	void Init(C_PlayerBase* a_pPlayer,C_UIManager* a_pUIManager)override;

	void OnceActiveGear()override;
	void WhileActiveGear()override;
	void EndActiveGear()override;

	float OnBossHitGear(C_BossBase* a_pBoss)override;
	float OnEnemyHitGear(EnemyBase* a_pEnemy)override;
	
	float OnHurtEnemyGear(EnemyBase* a_pEnemy)override;
	float OnHurtBossGear(C_BossBase* a_pBoss)override;

private:
	const float m_atkStatus = 20;
	const float m_crtBonus = 100;
	const float m_atkBonusP = 30;

	const float m_onHitDmg = 10;

	const float m_onHurtDmg = 8;
	
	//参照クラスポインタ
	C_Effect* m_pEffect = nullptr;
};

