#pragma once
#include"GameStateManager/GameStateManager.h"
#include "GUI/CharacterGUI.h"
#include"Boss/Boss.h"
#include"Enemy/EnemyManager.h"
#include"Player/PlayerBase.h"
#include"Map/Map.h"
#include"Camera/Camera.h"
#include"hit/hit.h"

class Scene
{
private:
	KdTexture m_playerTex;
	KdTexture m_playerAtkTex;
	KdTexture m_bossTex;
	KdTexture m_chipTex;
	KdTexture m_backTex;

	C_GameStateManager m_game;
	C_PlayerBase m_player;
	C_Boss m_boss;
	EnemyManager m_enemies;
	C_Map m_map;
	C_Camera m_camera;
	C_Hit m_hit;
    CharacterGUI m_gui;

public:

	// ‰Šúİ’è
	void Init();

	// ‰ğ•ú
	void Release();

	// XVˆ—
	void Update();

	// •`‰æˆ—
	void Draw2D();

	// GUIˆ—
	void ImGuiUpdate();

private:

	Scene() {}

public:
	static Scene& GetInstance()
	{
		static Scene instance;
		return instance;
	}
};

#define SCENE Scene::GetInstance()

