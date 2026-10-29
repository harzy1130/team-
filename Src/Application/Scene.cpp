#include "main.h"
#include "Scene.h"

void Scene::Draw2D()
{
	const bool inGame = m_game.GetGameState() == GameState::Game;
	const bool inSelect = m_game.GetGameState() == GameState::Select;

	m_game.Draw();
	if (inSelect || inGame)
	{
		m_map.Draw();
		m_player.Draw();
		if (inGame)
		{
			m_enemies.Draw();
			m_enemies.DrawVision();
			m_boss.Draw();
		}
	}
	m_game.SceneChangeDraw();
}

void Scene::Update()
{
	const bool inGame   = m_game.GetGameState() == GameState::Game;
	const bool inSelect = m_game.GetGameState() == GameState::Select;

	if (inSelect || inGame)
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
	m_game.Init();

	m_backTex.Load("Texture/back1.png");
	m_chipTex.Load("Texture/MapChip.png");
	m_bossTex.Load("Texture/Capricorn.png");
	m_playerTex.Load("Texture/Chara.png");
	m_playerAtkTex.Load("Texture/atk1.png");
	m_blackTex.Load("Texture/black.png");
	m_map.SetTexture(&m_chipTex);
	m_boss.SetTexture(&m_bossTex);
	m_player.SetTex(&m_playerTex);
	m_player.SetAtkTex(&m_playerAtkTex);
	m_game.SetTexture(&m_blackTex);
	m_game.SetBackTexture(&m_backTex);
	m_camera.Init();

	m_game.SetMapData(&m_map);
	m_game.SetPlayerData(&m_player);


	m_map.Init(&m_camera);
	m_player.Init(&m_hit,&m_camera, &m_cursor);
	m_boss.Init(&m_hit, &m_camera,&m_player);
    m_hit.Init(&m_map,&m_enemies);
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
	m_blackTex.Release();
}

void Scene::ImGuiUpdate()
{
	m_gui.Draw(m_player, m_enemies, m_game, m_map);
}