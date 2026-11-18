#pragma once
#include"Pch.h"
#include"Constants/Constants.h"
#include"Player/PlayerBase.h"
#include"Player/Gear/GearBase.h"

using namespace std;
class C_PlayerUI
{
public:
	C_PlayerUI() {};
	~C_PlayerUI() {};
	void Init(C_PlayerBase* a_pPlayer);
	void Update();
	void Draw();

	void DrawPlayerHpGauge();
	void UpdatePlayerHpGauge();
	void DrawPlayerSkill();
	void DrawPlayerGear(C_GearBase* a_pGear, Math::Vector2 a_pos);
	void DrawPlayerStatusBar();
	void AddStatusChip(string a_tag, KdTexture* a_pTex, float* a_maxTime, float* a_time);

	void DrawGrowEffect();
	void UpdateGrowEffect();
	void AddGrowEffect(int a_addres, Math::Vector2 a_pos);

	void Release();
private:
	//PlayerのHPゲージ======================================
	KdTexture m_playerHpFrameTex;
	KdTexture m_playerHpGaugeTex;
	KdTexture m_playerHpHeadTex;
	KdTexture m_playerHpTailTex;
	KdTexture m_playerHpBaseTex;
	
	float m_bufDmg = 0;
	float m_bufHeal = 0;
	float m_hpOffset = 0;
	float m_hpHeadOffset = 0;
	int m_hpScaleLine = 20;
	//======================================================
	//PlayerのSkillゲージ===================================
	KdTexture m_playerSkillFrameTex;
	KdTexture m_playerSkillBaseTex;
	KdTexture m_playerSkillCDTex;
	KdTexture m_playerSkillGaugeTex;
	KdTexture* m_pPlayerSkillIconTex;
	//======================================================
	//Playerのステータスバー================================
	map<string, S_StatusChip> m_statusBar;
	vector<string> m_statusBarOrder;
	// =====================================================
	//レンダーテクスチャ====================================
	KdTexture m_renderTex;
	//======================================================
	//発光エフェクト用の変数================================
	KdTexture m_growTex;
	
	map<int, S_Grow> m_grow;
	//座標変数==============================================
	Math::Vector2 m_hpGaugePos{ -500.0f, -300.0f };
	Math::Vector2 m_skillGaugePos{ 0, -275 };
	Math::Vector2 m_gearAPos{ -100, -275 };
	Math::Vector2 m_gearBPos{ 100, -275 };
	Math::Vector2 m_statusBarHeadPos{ -600, -270 };

	//======================================================
	//参照する別クラスのポインタ
	C_PlayerBase* m_pPlayer;
};