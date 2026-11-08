#include"UIManager.h"

void C_UIManager::Init(C_PlayerBase* a_pPlayerBase)
{
	
	m_playerUI.Init(a_pPlayerBase);
}

void C_UIManager::Update()
{
	m_playerUI.Update();
}

void C_UIManager::Draw()
{
	m_playerUI.Draw();
}

void C_UIManager::Release()
{
	m_playerUI.Release();
}
