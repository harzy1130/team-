#pragma once
#include"BossBase.h"

class C_Boss : public C_BossBase {
public:
	C_Boss() {};
	~C_Boss() {};

	void Init(C_Hit* a_pHit, C_Camera* a_pCamera,C_PlayerBase* a_pPlayer)override;
	void Update()override;
	void Move()override;
	void IfHitPlayer(C_PlayerBase* a_pPlayer)override;

private:
	void ChangeChargeStance();
	void ChargeStance();

	void StartWalk();
	void Walk();

	void StartDashCharge();
	void DashCharge();

	void StartJumpCharge();
	void JumpCharge();

	void StartTired();
	void Tired();

	static const enum State {
		stance,
		walk,
		jumpCharge,
		dashCharge,
		tired,
	};
	int m_moveCnt;
	float m_backStep;
	float m_chargeRange = CHIP_SIZE * 10;
};
