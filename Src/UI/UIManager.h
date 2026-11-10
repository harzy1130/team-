#pragma once
#include"PlayerUI.h"
#include"NumberUI.h"
#include"DamageUI.h"

class C_PlayerUI;
class C_NumberUI;
class C_UIManager
{
public:
	C_UIManager() {};
	~C_UIManager() {};

	void Init(C_PlayerBase* a_pPlayerBase, C_Camera* a_pCamera);
	void Update();
	void Draw();

	void Release();

	//É_ÉÅÅ[ÉWUIê∂ê¨
	void CreateDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);

private:
	C_PlayerUI m_playerUI;
	C_DamageUI m_damageUI;
	//âÊëú
	KdTexture m_numberTex;
};
