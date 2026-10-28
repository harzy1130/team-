#pragma once
#include"BossBase.h"

enum State {
	stance,
	walk,
	jumpCharge,
	dashCharge,
};


class C_Boss : public C_BossBase {
public:
	C_Boss() {};
	~C_Boss() {};

	void Draw();
	void Update();
	void Move();

	void ChangeChargeStance();
	void ChargeStance();

	void StartWalk();
	void Walk();

	void StartDashCharge();
	void DashCharge();

	void StartJumpCharge();
	void JumpCharge();
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera,C_PlayerBase* a_pPlayer);
private:
	int m_chargeWait;
	float m_chargeRange = CHIP_SIZE * 10;
};
