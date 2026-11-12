#pragma once
#include "GearBase.h"

class C_Gear2 :public C_GearBase
{
public:
	C_Gear2() : C_GearBase("Texture/HealGearIcon.png") {};
	~C_Gear2() {};
	void Init(C_PlayerBase* a_pPlayer)override;

	bool PassiveTiming()override;
	void OncePassiveGear()override;
	void WhilePassiveGear()override;
	void EndPassiveGear()override;
public:
	const float m_hpRegeneStatus = 0.5f;
	const float m_heal = 3;
	const float m_defBonus = 50;
};