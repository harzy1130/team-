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
	m_camera.Update();
	m_map.Update();
	m_enemy.Update();
	m_boss.Update();
	m_player.Update();
	m_camera.SetTurget(m_player.GetPos());
}

void Scene::Init()
{
	m_backTex.Load("Texture/back1.png");
	m_chipTex.Load("Texture/MapChip.png");
	m_bossTex.Load("Texture/boss.png");
	m_playerTex.Load("Texture/Chara.png");
	m_map.SetTexture(&m_chipTex);
	m_boss.SetTexture(&m_bossTex);
	m_player.SetTex(&m_playerTex);
	m_camera.Init();

	if (!m_enemy.Init(&m_hit, &m_camera))
	{
		OutputDebugStringA("Enemy texture load failed: Texture/Chara.png\n");
	}

	m_enemy.SetHp(100);
	m_enemy.SetAtk(10);
	m_enemy.SetMoveSpeed(2.0f);
	m_map.Init(&m_camera);
	m_player.Init(&m_hit,&m_camera);
	m_boss.Init(&m_hit, &m_camera);
	m_hit.SetMapClass(&m_map);
	
}

void Scene::Release()
{
	m_backTex.Release();
	m_chipTex.Release();
	m_bossTex.Release();
	m_playerTex.Release();
	m_enemy.Release();
}

void Scene::ImGuiUpdate()
{
    m_gui.Draw(m_player, m_enemy);
}