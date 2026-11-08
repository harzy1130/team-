#pragma once
#include"Constants/Constants.h"
//スキル状態定数==============================
enum S_State
{
	Ready,
	Casting,
	Active,
	Recovering
};

class C_PlayerBase;
class C_Skill
{
public:
	C_Skill() {};
	~C_Skill() {};

	virtual void Init();
	virtual void Update();
	virtual void Draw(Math::Vector2 a_pos);
	virtual void SkillActivation(C_PlayerBase*a_pPlayer );

	//ゲッター
	virtual bool GetbSkillMotion() { return m_bSkillMotion; }
	virtual float GetSkillCD() { return m_skillCD; }
	virtual float GetSkillCDMax() { return m_skillCDMax; }
	virtual float GetSkillCalcCD(C_PlayerBase* a_pPlayer) { return SkillHasteCalc(m_skillCDMax, a_pPlayer); }

	//セッター
	virtual void SetIconTexture(KdTexture* a_pTex) { m_pIconTexture = a_pTex; }
	virtual void SetTexture(KdTexture* a_pTex) { m_pTexture = a_pTex; }
protected:

	//スキル管理
	float m_skillCD;
	float m_skillCDMax;
	bool m_bSkillMotion;
	float m_count;
	KdTexture* m_pTexture;
	KdTexture* m_pIconTexture;
	S_State m_CurrentState;
	C_PlayerBase *m_PB;

	//計算
	float SkillHasteCalc(float a_CDMax, C_PlayerBase *a_pPlayer);
	void UpdateCooldown();
};

