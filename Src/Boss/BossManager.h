#pragma once
#include"BossBase.h"
#include"Boss.h"
#include"Boss2.h"
#include"Boss3.h"

class C_BossBase;
class C_BossManager
{
public:
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer, int a_bossNum);
	void Update() {	if (m_pBoss != nullptr)m_pBoss->Update();}
	void Draw() { if (m_pBoss != nullptr)m_pBoss->Draw(); }
	void Release() { if (m_pBoss != nullptr)m_pBoss->Release();m_pBoss.reset();}
	
	void SetBoss(int a_bossNum);
	
	C_BossBase* GetBoss() { return m_pBoss.get(); }
private:
	std::unique_ptr<C_BossBase> m_pBoss = nullptr;
	C_Hit* m_pHit = nullptr;
	C_Camera* m_pCamera = nullptr;
	C_PlayerBase* m_pPlayer = nullptr;
};