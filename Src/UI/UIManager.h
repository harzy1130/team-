#pragma once
#include"PlayerUI.h"

class C_PlayerUI;
class C_UIManager
{
public:
	C_UIManager() {};
	~C_UIManager() {};

	void Init(C_PlayerBase* a_pPlayerBase);
	void Update();
	void Draw();

	void Release();
private:
	C_PlayerUI m_playerUI;

};
