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

//ゲッター
	//アクティブ
	virtual float GetActiveCD()final { return m_activeCD; }
	virtual float GetActiveCDMax()final { return m_activeCDMax; }
	virtual float GetCalcActiveCDMax()final;
	virtual float GetActiveTimer()final { return m_activeTimer; }
	virtual float GetActiveTimerMax()final { return m_activeTimerMax; }
	virtual bool GetWhileActive()final { return m_bWhileActive; }
	virtual bool GetHaveActiveCD()final { return m_bHaveActiveCD; }
	virtual bool GetWaitActiveTimer()final { return m_bWaitActiveTimer; }
	
	//パッシブ
	virtual float GetPassiveCD()final { return m_passiveCD; }
	virtual float GetPassiveCDMax()final { return m_passiveCDMax; }
	virtual float GetCalcPassiveCDMax()final;
	virtual float GetPassiveTimer()final { return m_passiveTimer; }
	virtual float GetPassiveTimerMax()final { return m_passiveTimerMax; }
	virtual bool GetWhilePassive()final { return m_bWhilePassive; }
	virtual bool GetHavePassiveCD()final { return m_bHavePassiveCD; }
	virtual bool GetWaitPassiveTimer()final { return m_bWaitPassiveTimer; }
	virtual bool GetPassiveOnly()final { return m_bPassiveOnly; }

	virtual S_Status GetGearStatus()final { return m_status; }
	//呼び出し側での書き換えを防ぐため、constにキャストする必要がある
	//const関数で返すと、呼び出し元でconstを付ける必要があるため、constを外して返す
	virtual KdTexture* GetGearIconTex() const final { return const_cast<KdTexture*>(&m_gearIconTex); }
protected:
	//装備が持つ増加ステータス
	S_Status m_status;

//クールダウンなど
	//アクティブ用
	float m_activeCD;
	float m_activeCDMax;
	float m_activeTimer;
	float m_activeTimerMax;
	bool m_bWhileActive;			//アクティブの効果が発動中かどうか
	bool m_bHaveActiveCD = true;
	bool m_bWaitActiveTimer = false;	//アクティブの効果が終了してから、アクティブのクールダウンを開始する場合にTrueにする
	//パッシブ用
	float m_passiveCD;
	float m_passiveCDMax;
	float m_passiveTimer;
	float m_passiveTimerMax;
	bool m_bWhilePassive;			//パッシブの効果が発動中かどうか
	bool m_bHavePassiveCD = true;	
	bool m_bWaitPassiveTimer = false;	//パッシブの効果が終了してから、パッシブのクールダウンを開始する場合にTrueにする
	//装備効果がパッシブのみの場合にTrueにして、アクティブのクールダウンを無効化する
	bool m_bPassiveOnly = false;
	//装備アイコン
	KdTexture m_gearIconTex;
	//プレイヤーを参照するためのポインタ
	C_PlayerBase* m_pPlayer = nullptr;

private:
};