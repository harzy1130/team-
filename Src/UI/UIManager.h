#pragma once
#include"PlayerUI.h"
#include"NumberUI.h"
#include"DamageUI.h"
#include"EffectDmgUI.h"

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

	//ダメージUI生成
	void CreateDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);
	//敵ダメージUI生成
	void CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);

	//エフェクトダメージUI生成
	void CreatePoisonDamageUI(float a_Damage, Math::Vector2 a_Pos);

private:
	C_PlayerUI m_playerUI;
	C_DamageUI m_damageUI;
	C_EffectDmgUI m_effectDmgUI;
	//画像
	KdTexture m_numberTex;
	KdTexture m_numberFrameTex;
};
