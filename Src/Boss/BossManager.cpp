#include "BossManager.h"

void C_BossManager::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer, int a_bossNum)
{
	m_pHit = a_pHit;
	m_pCamera = a_pCamera;
	m_pPlayer = a_pPlayer;
	if (m_pBoss != nullptr)m_pBoss->Release();
	m_pBoss.reset();
	switch (a_bossNum)
	{
	case 1:
		m_pBoss = std::make_unique<C_Boss>();
		break;
	case 2:
		m_pBoss = std::make_unique<C_Boss2>();
		break;
	default:
		m_pBoss = nullptr;
		return;
	}
	if (m_pBoss != nullptr)m_pBoss->Init(a_pHit, a_pCamera, a_pPlayer);
}

void C_BossManager::SetBoss(int a_bossNum)
{
	if (m_pBoss != nullptr)m_pBoss->Release();
	m_pBoss.reset();
	switch (a_bossNum)
	{
	case 1:
		m_pBoss = std::make_unique<C_Boss>();
		break;
	case 2:
		m_pBoss = std::make_unique<C_Boss2>();
		break;
	default:
		m_pBoss = nullptr;
		return;
	}
	if (m_pBoss != nullptr)m_pBoss->Init(m_pHit, m_pCamera, m_pPlayer);
	m_pHit->SetBossClass(m_pBoss.get());
}
