#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(&m_backTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
	m_map.Draw();
	m_enemy.Draw();
	m_boss.Draw();
	m_player.Draw();
}

void Scene::Update()
{
	m_map.Update();
	m_enemy.Update();
	m_boss.Update();
	m_player.Update();
}

void Scene::Init()
{
	m_backTex.Load("Texture/back1.png");
	m_chipTex.Load("Texture/MapChip.png");
	m_bossTex.Load("Texture/boss.png");
	m_playerTex.Load("Texture/Chara.png");
	m_map.SetTexture(&m_chipTex);
	m_boss.SetTexture(&m_bossTex);
	m_player.setTex(&m_playerTex);

	if (!m_enemy.Init())
	{
		OutputDebugStringA("Enemy texture load failed: Texture/Chara.png\n");
	}
	m_enemy.SetHp(100);
	m_enemy.SetAtk(10);
	m_enemy.SetMoveSpeed(2.0f);
	m_map.Load();
	m_map.Create();
	m_player.Init();
	m_boss.Init();
}

void Scene::Release()
{
	m_backTex.Release();
	m_chipTex.Release();
	m_bossTex.Release();
	m_playerTex.Release();
}

void Scene::ImGuiUpdate()
{
	return;

	ImGui::SetNextWindowPos(ImVec2(20, 20), ImGuiSetCond_Once);
	ImGui::SetNextWindowSize(ImVec2(200, 100), ImGuiSetCond_Once);

	// デバッグウィンドウ
	if (ImGui::Begin("Debug Window"))
	{
		ImGui::Text("FPS : %d", APP.m_fps);
	}
	ImGui::End();
}