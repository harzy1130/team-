#pragma once
#include"Constants/Constants.h"

class C_PlayerBase;
class C_GearBase
{
public:
	C_GearBase() {};
	C_GearBase(std::string a_texturePath)
	{
		m_gearIconTex.Load(a_texturePath);
	};
	~C_GearBase() 
	{
		m_gearIconTex.Release();
	};

	virtual void Init(C_PlayerBase* a_pPlayer);
	virtual void Draw();

	virtual void OnceActiveGear();
	virtual void WhileActiveGear();
	virtual void EndActiveGear();

	virtual bool PassiveTiming();
	virtual void OncePassiveGear();
	virtual void WhilePassiveGear();
	virtual void EndPassiveGear();

	virtual void UpdateCD();

	virtual float ActiveCD()final { return m_activeCD; }
	virtual float ActiveCDMax()final { return m_activeCDMax; }
	virtual S_Status GetGearStatus()final { return m_status; }
	//呼び出し側での書き換えを防ぐため、constにキャストする必要がある
	//const関数で返すと、呼び出し元でconstを付ける必要があるため、constを外して返す
	virtual KdTexture* GetGearIconTex() const final { return const_cast<KdTexture*>(&m_gearIconTex); }
protected:
	//装備が持つ増加ステータス
	S_Status m_status;

	//クールダウンなど
	float m_activeCD;
	float m_activeCDMax;
	float m_activeTimer;
	float m_activeTimerMax;
	bool m_bGearActive;
	bool m_bHaveActiveCD;

	float m_passiveCD;
	float m_passiveCDMax;
	float m_passiveTimer;
	float m_passiveTimerMax;
	bool m_bGearPassive;
	bool m_bHavePassiveCD;
	//装備アイコン
	KdTexture m_gearIconTex;
	//プレイヤーを参照するためのポインタ
	C_PlayerBase* m_pPlayer = nullptr;

private:
};