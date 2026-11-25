#include"UIManager.h"

void C_UIManager::Init(C_PlayerBase* a_pPlayerBase, C_Camera* a_pCamera)
{
	m_hintUI.Init(a_pPlayerBase, a_pCamera);
	std::string path = "Src/UI/Texture/NumberUI/";
	m_numberTex.Load(path + "number.png");
	m_numberFrameTex.Load(path + "numberFrame.png");
	m_damageUI.Init(&m_numberTex, &m_numberFrameTex, a_pCamera);
	m_effectDmgUI.Init(&m_numberTex, &m_numberFrameTex, a_pCamera);
	m_recoveryUI.Init(&m_numberTex, &m_numberFrameTex, a_pCamera);
	m_playerUI.Init(a_pPlayerBase);
	
}

void C_UIManager::Update(GameState gameState, int stage)
{
	const bool inGame = gameState == GameState::Game;
	m_hintUI.Update(inGame, stage);
	if (!inGame && gameState != GameState::Select) return;

	m_playerUI.Update();
	m_damageUI.Update();
	m_effectDmgUI.Update();
	m_recoveryUI.Update();
}

void C_UIManager::Draw()
{
	m_effectDmgUI.Draw();
	m_recoveryUI.Draw();
	m_damageUI.Draw();
	m_playerUI.Draw();
	m_hintUI.Draw();
}

void C_UIManager::Release()
{
	m_hintUI.Release();
	m_numberTex.Release();
	m_numberFrameTex.Release();
	m_playerUI.Release();
	m_damageUI.Release();
	m_effectDmgUI.Release();
	m_recoveryUI.Release();
}

//ダメージUI生成(値を渡すだけ)
void C_UIManager::CreateDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical)
{
	m_damageUI.CreateDamageUI(a_Damage, a_Pos, a_movingDirectionX, a_critical);
}

void C_UIManager::CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical)
{
	m_damageUI.CreateEnemyDamageUI(a_Damage, a_Pos, a_movingDirectionX, a_critical);
}

void C_UIManager::CreateGearDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical)
{
	m_damageUI.CreateGearDamageUI(a_Damage, a_Pos, a_movingDirectionX, a_critical);
}

void C_UIManager::CreatePoisonDamageUI(float a_Damage, Math::Vector2 a_Pos)
{
	m_effectDmgUI.CreatePoisonDamageUI(a_Damage, a_Pos);
}

void C_UIManager::CreateFlameDamageUI(float a_Damage, Math::Vector2 a_Pos)
{
	m_effectDmgUI.CreateFlameDamageUI(a_Damage, a_Pos);
}

void C_UIManager::CreateRecoveryUI(float a_Recovery, Math::Vector2 a_Pos)
{
	m_recoveryUI.CreateRecoveryUI(a_Recovery, a_Pos);
}

void C_UIManager::AddPlayerStatusChip(string a_tag, KdTexture* a_pTex, float* a_maxTime, float* a_time)
{
	m_playerUI.AddStatusChip(a_tag, a_pTex, a_maxTime, a_time);
}
