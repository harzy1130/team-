#pragma once
#include"PlayerUI.h"
#include"NumberUI.h"
#include"DamageUI.h"
#include"EffectDmgUI.h"
#include"RecoveryUI.h"
#include "HintUI.h"
#include "Constants/Constants.h"

class C_PlayerUI;
class C_NumberUI;
class C_UIManager
{
public:
	C_UIManager() {};
	~C_UIManager() {};

	void Init(C_PlayerBase* a_pPlayerBase, C_Camera* a_pCamera);
	void Update(GameState gameState, int stage);
	void Draw();

	void Release();

	//ダメージUI生成
	void CreateDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);
	//敵ダメージUI生成
	void CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);

	//エフェクトダメージUI生成
	void CreatePoisonDamageUI(float a_Damage, Math::Vector2 a_Pos);
	void CreateFlameDamageUI(float a_Damage, Math::Vector2 a_Pos);

	//回復UI生成
	void CreateRecoveryUI(float a_Recovery, Math::Vector2 a_Pos);

	//プレイヤーステータスバーのステータスチップ生成
	void AddPlayerStatusChip(string a_tag, KdTexture* a_pTex, float* a_maxTime, float* a_time);
private:
	HintUI m_hintUI;
	C_PlayerUI m_playerUI;
	C_DamageUI m_damageUI;
	C_EffectDmgUI m_effectDmgUI;
	C_RecoveryUI m_recoveryUI;
	//画像
	KdTexture m_numberTex;
	KdTexture m_numberFrameTex;
};
