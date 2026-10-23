#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(&m_backTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
	m_game.Draw();
	if (m_game.GetGameState() == GameState::Select || m_game.GetGameState() == GameState::Game)
	{
		m_map.Draw();
		if (m_game.GetGameState() == GameState::Game) m_enemies.DrawVision();
		m_player.Draw();
		if (m_game.GetGameState() == GameState::Game)
		{
			m_enemies.Draw();
			m_boss.Draw();
		}
	}
}

void Scene::Update()
{
	const bool inGame = m_game.GetGameState() == GameState::Game;
	if (m_game.GetGameState() == GameState::Select || inGame)
	{
		m_camera.Update();
		m_map.Update();
		m_player.Update();
		m_camera.SetTurget(m_player.GetPos());
	}
	m_enemies.Update(m_player.GetAFlg() ? &m_player : nullptr, inGame);
	if (inGame) m_boss.Update();
	m_game.Update();
}

void Scene::Init()
{
	m_backTex.Load("Texture/back1.png");
	m_chipTex.Load("Texture/MapChip.png");
	m_bossTex.Load("Texture/boss.png");
	m_playerTex.Load("Texture/Chara.png");
	m_playerAtkTex.Load("Texture/atk1.png");
	m_map.SetTexture(&m_chipTex);
	m_boss.SetTexture(&m_bossTex);
	m_player.SetTex(&m_playerTex);
	m_player.SetAtkTex(&m_playerAtkTex);
	m_camera.Init();

	m_game.SetMapData(&m_map);
	m_game.SetPlayerData(&m_player);


	m_map.Init(&m_camera);
	m_player.Init(&m_hit,&m_camera);
	m_boss.Init(&m_hit, &m_camera);
    m_hit.SetMapClass(&m_map);
    if (!m_enemies.Init(&m_hit, &m_camera, &m_map))
        OutputDebugStringA("Enemy manager initialization failed.\n");
	
}

void Scene::Release()
{
	m_backTex.Release();
	m_chipTex.Release();
	m_bossTex.Release();
	m_playerTex.Release();
	m_playerAtkTex.Release();
	m_enemies.Release();
}

void Scene::ImGuiUpdate()
{
	m_gui.Draw(m_player, m_enemies, m_game, m_map);
}