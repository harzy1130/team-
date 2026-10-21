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
		if (m_game.GetGameState() == GameState::Game) m_enemy.DrawVision();
		m_player.Draw();
		if (m_game.GetGameState() == GameState::Game)
		{
			m_enemy.Draw();
			m_boss.Draw();
		}
	}
}

void Scene::Update()
{
	if (m_game.GetGameState() != GameState::Game) m_enemyWasInGame = false;
	if (m_game.GetGameState() == GameState::Select || m_game.GetGameState() == GameState::Game)
	{
		m_camera.Update();
		m_map.Update();
		m_player.Update();
		m_camera.SetTurget(m_player.GetPos());
		if (m_game.GetGameState() == GameState::Game)
		{
            // Detect both normal stage entry and the GUI's direct stage switch.
            if (!m_enemyWasInGame || m_enemyStage != m_map.GetStageNum())
            {
                if (!m_enemy.PlaceOnMap(&m_map))
                    OutputDebugStringA("Enemy spawn failed: no ground below spawn column.\n");
                m_enemyStage = m_map.GetStageNum();
                m_enemyWasInGame = true;
            }
            m_enemy.SetTarget(m_player.GetAFlg() ? &m_player : nullptr);
			m_enemy.Update();
			m_boss.Update();
		}
	}
	// Clear pursuit when changing stages, titles, or results.
	const auto previousState = m_game.GetGameState();
	m_game.Update();
	if (previousState != m_game.GetGameState()) m_enemy.ResetAwareness();
}

void Scene::Init()
{
	m_enemyWasInGame = false;
	m_enemyStage = -1;
	m_backTex.Load("Texture/back1.png");
	m_chipTex.Load("Texture/MapChip.png");
	m_bossTex.Load("Texture/boss.png");
	m_playerTex.Load("Texture/Chara.png");
	m_map.SetTexture(&m_chipTex);
	m_boss.SetTexture(&m_bossTex);
	m_player.SetTex(&m_playerTex);
	m_camera.Init();

	m_game.SetMapData(&m_map);
	m_game.SetPlayerData(&m_player);

	if (!m_enemy.Init(&m_hit, &m_camera))
	{
		OutputDebugStringA("Enemy texture load failed: Texture/Chara.png\n");
	}

	m_enemy.SetVisionMap(&m_map);
	m_enemy.SetVision(320.0f, 60.0f, 560.0f);
	m_enemy.SetChaseSpeed(4.0f);
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
	m_gui.Draw(m_player, m_enemy, m_game, m_map);
}