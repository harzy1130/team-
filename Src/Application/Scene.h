#pragma once
#include"GameStateManager/GameStateManager.h"
#include "GUI/CharacterGUI.h"
#include "Particle/ParticleManager.h"
#include"Boss/BossManager.h"
#include"Enemy/EnemyManager.h"
#include"Player/Player1.h"
#include"Map/Map.h"
#include"Camera/Camera.h"
#include"hit/hit.h"
#include"Cursor/Cursor.h"
#include"UI/UIManager.h"
#include"Player/Gear/Armory.h"

class Scene
{
private:
	KdTexture m_playerTex;
	KdTexture m_player1Tex;
	KdTexture m_playerAtkTex;
	KdTexture m_chipTex;
	KdTexture m_backTex;
	KdTexture m_blackTex;
	KdTexture m_skillIconTex;
	KdTexture m_grainTex;
	KdTexture m_atkUpTex;
	KdTexture m_defUpTex;
	KdTexture m_smokeTex;
	KdTexture m_healTex;
	KdTexture m_burnTex;

	C_GameStateManager m_game;
	C_ParticleManager m_particle;
	C_Player1 m_player;
	C_BossManager m_bossManager;
	EnemyManager m_enemies;
	C_Map m_map;
	C_Camera m_camera;
	C_Hit m_hit;
    CharacterGUI m_gui;
	C_Cursor m_cursor;
	C_UIManager m_ui;
	C_Armory m_armory;

	// フレーム
	float m_flame = 0;
	// ゲームの状態
	bool m_bSlowMotion = false;

public:

	// 初期設定
	void Init();

	// 解放
	void Release();

	// 更新処理
	void Update();

	// 描画処理
	void Draw2D();

	// GUI処理
	void ImGuiUpdate();

	//スローモーションの切り替え
	void SetSlowMotion(bool a_bSlowMotion) { m_bSlowMotion = a_bSlowMotion; }


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

