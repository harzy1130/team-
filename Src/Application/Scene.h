#pragma once
#include"Boss/Boss.h"
#include"Enemy/Enemy1.h"
#include"Player/PlayerBase.h"
#include"Map/Map.h"
#include"Camera/Camera.h"
#include"hit/hit.h"

class Scene
{
private:
	KdTexture m_playerTex;
	KdTexture m_bossTex;
	KdTexture m_chipTex;
	KdTexture m_backTex;

	C_PlayerBase m_player;
	Boss m_boss;
	Enemy1 m_enemy;
	Map m_map;
	C_Camera m_camera;
	C_Hit m_hit;

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

