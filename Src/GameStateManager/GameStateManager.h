#pragma once
#include"Map/Map.h"
#include"Player/PlayerBase.h"

struct S_RenderTarget
{
	float x;
	float y;
	float scaleX;
	float scaleY;
	float scale;
	Math::Matrix mat;
	Math::Matrix transMat;
	Math::Matrix scaleMat;
};

struct S_Object
{
	float x;
	float y;
	Math::Matrix mat;
	KdTexture tex;
};

class C_Map;
class C_PlayerBase;
class C_Boss;
class C_GameStateManager
{
public:
	C_GameStateManager() {};
	~C_GameStateManager() {};

	void Init();
	void Update();
	void Draw();

	void SceneChangeDraw();
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	void SetBackTexture(KdTexture* a_pBackTex) { m_pBackTex = a_pBackTex; }

	int GetGameState() { return m_gameState; }
	bool GetChangeTime() { return m_sceneTransCnt; }

	void SetGameState(int a_gameState) { m_gameState = a_gameState; }
	void SetMapData(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetPlayerData(C_PlayerBase* a_pPlayer) { m_pPlayer = a_pPlayer; }

private:
	C_Map* m_pMap;
	C_PlayerBase* m_pPlayer;
	int m_gameState;

	S_Object m_back;
	KdTexture* m_pBackTex;

	KdTexture m_tmpTex;   //レンダーターゲット用
	S_RenderTarget wipe;

	KdTexture m_tmpTex2;  //レンダーターゲット切り替えようテクスチャ
	S_RenderTarget m_tmp2;//座標や行列計算に使う
	int m_scaleNum;	    //拡大率・縮小率用

	KdTexture* m_pTex;
	StageNum m_nextStage{};
	bool m_bChangeScene = false;
	float m_alpha = 0.0f;
	const float m_sceneTransTime = 60;
	float m_sceneTransCnt = 0;

	const float m_loadTime = 90;
	float m_loadCnt = 0;

	void SceneTrans();
	void DynamicDraw2D();

	void StageTrans();
	void SearchNearStage(int h, int w, StageNum stage, float& nearestStageDistance, StageNum& nearestStage, bool& foundStage);
	void DebugKey();//********************:::提出時消す
};

const enum GameState
{
	Title = 0,
	Select = 1,
	Game = 2,
	Result = 3
};