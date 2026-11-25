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
		if (inGame)
		{
			m_bossManager.Draw();
			m_enemies.Draw();
			m_enemies.DrawVision();
		}
		m_player.Draw();
		m_particle.Draw();
		m_ui.Draw();
		m_armory.Draw();
	}
	m_game.SceneTransDraw();
}

void Scene::Update()
{
	const bool inGame   = m_game.GetGameState() == GameState::Game;
	const bool inSelect = m_game.GetGameState() == GameState::Select;

	if (inSelect)m_armory.Update();
	if (inSelect || inGame)
	{
		m_camera.Update();
		m_map.Update();
		if (!m_armory.GetArmoryOpen()) {
			if (!m_bSlowMotion) {
				m_player.Update();
			}
			else if ((int)m_flame % 2 == 0)
			{
				m_player.Update();
			}
		}

		m_particle.Update();
		m_camera.SetTurget(m_player.GetPos());
	}
	if (!m_bSlowMotion)
	{
		m_enemies.Update(m_player.GetAFlg() ? &m_player : nullptr, inGame);
		m_bossManager.Update();
	}
	else if ((int)m_flame % 2 == 0)
	{
		m_enemies.Update(m_player.GetAFlg() ? &m_player : nullptr, inGame);
		m_bossManager.Update();
	}
	m_game.Update();
	m_ui.Update(static_cast<GameState>(m_game.GetGameState()), m_map.GetStageNum());
}

void Scene::Init()
{
	m_game.Init(&m_map, &m_player, &m_bossManager);
	std::string path = "Texture/";
	m_backTex.Load(path + "back1.png");
	m_chipTex.Load(path + "MapChip.png");
	m_playerTex.Load(path + "Chara.png");
	m_player1Tex.Load(path + "Player1.png");
	m_playerAtkTex.Load(path + "atk1.png");
	m_blackTex.Load(path + "black.png");
	m_skillIconTex.Load(path + "SkillIcon.png");
	m_grainTex.Load(path + "grain.png");
	m_atkUpTex.Load(path + "atkUp.png");
	m_defUpTex.Load(path + "defUp.png");
	m_smokeTex.Load(path + "smoke.png");
	m_healTex.Load(path + "heal.png");
	m_burnTex.Load(path + "burn.png");
	m_map.SetTexture(&m_chipTex);
	m_player.SetTex(&m_player1Tex);
	m_player.SetAtkTex(&m_playerAtkTex);
	m_player.SetSkillIconTex(&m_skillIconTex);
	m_game.SetTexture(&m_blackTex, &m_backTex);
	m_particle.SetTexture(&m_chipTex, &m_grainTex, &m_atkUpTex, &m_defUpTex, &m_smokeTex, &m_healTex, &m_burnTex);
	m_camera.Init();
	m_ui.Init(&m_player, &m_camera);

	m_map.Init(&m_camera);
	m_player.Init(&m_hit,&m_camera, &m_cursor, &m_ui);
	m_armory.Init();
	m_player.SetSceneClass(this);
	m_bossManager.Init(&m_hit, &m_camera, &m_player, 0);
    m_hit.Init(&m_map, &m_enemies, m_bossManager.GetBoss(), &m_player);
	m_particle.Init(&m_map, &m_player, &m_camera);
    if (!m_enemies.Init(&m_hit, &m_camera, &m_map))
        OutputDebugStringA("Enemy manager initialization failed.\n");
}

void Scene::Release()
{
	m_backTex.Release();
	m_chipTex.Release();
	m_playerTex.Release();
	m_player1Tex.Release();
	m_playerAtkTex.Release();
	m_enemies.Release();
	m_blackTex.Release();
	m_skillIconTex.Release();
	m_ui.Release();
	m_bossManager.Release();
	m_grainTex.Release();
	m_atkUpTex.Release();
	m_defUpTex.Release();
	m_smokeTex.Release();
	m_healTex.Release();
	m_burnTex.Release();
	m_armory.Release();
}

void Scene::ImGuiUpdate()
{
	m_gui.Draw(m_player, m_enemies, m_game, m_map);
}