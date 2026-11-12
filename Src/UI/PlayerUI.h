#pragma once
#include"Pch.h"
#include"Constants/Constants.h"
#include"Player/PlayerBase.h"

class C_PlayerUI
{
public:
	C_PlayerUI() {};
	~C_PlayerUI() {};
	void Init(C_PlayerBase* a_pPlayer);
	void Update();
	void Draw();

	void DrawPlayerHpGauge();
	void DrawPlayerSkill();
	void DrawPlayerGear(C_GearBase* a_pGear, Math::Vector2 a_pos);

	void Release();
private:
	//PlayerのHPゲージのテクスチャ==========================
	KdTexture m_playerHpFrameTex;
	KdTexture m_playerHpGaugeTex;
	KdTexture m_playerHpHeadTex;
	KdTexture m_playerHpTailTex;
	KdTexture m_playerHpBaseTex;
	//======================================================
	//PlayerのSkillゲージのテクスチャ=======================
	KdTexture m_playerSkillFrameTex;
	KdTexture m_playerSkillBaseTex;
	KdTexture m_playerSkillCDTex;
	KdTexture m_playerSkillGaugeTex;
	KdTexture m_renderTex;
	KdTexture* m_pPlayerSkillIconTex;
	//======================================================
	 
	//数字変数
	Math::Vector2 m_hpGaugePos{-500.0f, -300.0f};
	Math::Vector2 m_skillGaugePos{  0, -275 };

	C_PlayerBase* m_pPlayer;
};