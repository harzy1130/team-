#pragma once
#include"BossBase.h"


class C_Boss : public C_BossBase {
public:
	C_Boss() {};
	~C_Boss() {};

	void Draw();
	void Update();
	void Move();

private:

};
