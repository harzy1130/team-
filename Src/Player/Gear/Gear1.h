#pragma once
#include "GearBase.h"

class C_Gear1 : public C_GearBase
{
public:
	C_Gear1(): C_GearBase("Texture/AtkUpGearIcon.png") {};
	~C_Gear1() {};
	void Init(C_PlayerBase* a_pPlayer)override;

	void OnceActiveGear()override;
	void WhileActiveGear()override;
	void EndActiveGear()override;

private:
	const float m_atkStatus = 20;
	const float m_atkBonus = 30;
};

