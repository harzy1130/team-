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

private:
	const float m_atkStatus = 20;
	const float m_crtBonus = 100;
	const float m_atkBonusP = 30;
	
	//参照クラスポインタ
	C_Effect* m_pEffect = nullptr;
};

