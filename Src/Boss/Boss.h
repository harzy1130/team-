#pragma once
#include"BossBase.h"


class C_Boss : public C_BossBase {
public:
	C_Boss() {};
	~C_Boss() {};

	void Draw();
	void Update();
	void Move();
	void Walk();
	void Charge();
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera);
private:

};
