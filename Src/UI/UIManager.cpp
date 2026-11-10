#include"UIManager.h"

void C_UIManager::Init(C_PlayerBase* a_pPlayerBase, C_Camera* a_pCamera)
{
	std::string path = "Src/UI/Texture/NumberUI/";
	m_numberTex.Load(path + "number.png");
	m_damageUI.Init(&m_numberTex, a_pCamera);
	m_playerUI.Init(a_pPlayerBase);
	
}

void C_UIManager::Update()
{
	m_playerUI.Update();
	m_damageUI.Update();
}

void C_UIManager::Draw()
{
	m_playerUI.Draw();
	m_damageUI.Draw();
}

void C_UIManager::Release()
{
	m_numberTex.Release();
	m_playerUI.Release();
	m_damageUI.Release();
}

void C_UIManager::CreateDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical)
{
	m_damageUI.CreateDamageUI(a_Damage, a_Pos, a_movingDirectionX, a_critical);
}
