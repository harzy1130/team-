#pragma once
#include"PlayerBase.h"
#include"Sword.h"
class C_Player1:public C_PlayerBase
{
public:
	C_Player1() {};
	~C_Player1(){};

	void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor* a_pCursor)override;
	void Update()override;
	void Draw()override;


private:
	C_Sword m_sword;
};
