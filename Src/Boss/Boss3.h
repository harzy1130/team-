#pragma once
#include "BossBase.h"
#include "BossParts.h"

class C_BossParts;
class C_Boss3 : public C_BossBase
{
public:
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer) override;
	void Update() override;
	void Draw() override;
	void Release() override;
	void Move() override;

private:
	void MainBodyUpdate();
	void TailInit();
	void TailUpdate();
	void ScissorInit();
	void ScissorUpdate();

	C_BossParts m_tail1;
	C_BossParts m_tail2;
	C_BossParts m_needle;
	C_BossParts m_scissor1;
	C_BossParts m_scissor2;
	float m_tail1Angle = 0;
	float m_tail2Angle = 0;

	bool m_dbgKey = false;
};

