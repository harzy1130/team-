#pragma once
#include"BossBase.h"

class Boss : public BossBase {
public:
	Boss() {};
	~Boss() {};

	void Draw();
	void Update();
	void Move();
	void MatrixUpdate();
	void Init();

private:
	
};
