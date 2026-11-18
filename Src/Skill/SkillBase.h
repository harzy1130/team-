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
class C_SkillBase
{
public:
	C_SkillBase() {};
	~C_SkillBase() {};

	virtual void Init(C_PlayerBase* a_pPlayer);
	virtual void Update();
	virtual void Draw();

	void UpdateCooldown();

	//スキル発動用
	//virtual void SkillActivation();
	virtual void OnceActiveSkill();
	virtual void WhileActiveSkill();
	virtual void EndActiveSkill();

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
	float m_countMax;
	bool m_bWaitActiveTimer = false;		//アクティブの効果が終了してから、アクティブのクールダウンを開始する場合にTrueにする
	KdTexture* m_pTexture;
	KdTexture* m_pIconTexture;
	S_State m_CurrentState;//スキル状態

	//参照クラスポインタ
	C_PlayerBase *m_pPlayer;

	//計算
	float SkillHasteCalc(float a_CDMax, C_PlayerBase *a_pPlayer);
	
};

