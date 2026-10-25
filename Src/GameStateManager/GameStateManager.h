#pragma once
#include"Map/Map.h"
#include"Player/PlayerBase.h"

class C_Map;
class C_PlayerBase;
class C_GameStateManager
{
public:
	C_GameStateManager();
	~C_GameStateManager() {};

	void Update();
	void Draw();

	void SceneChangeDraw();
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }

	int GetGameState() { return m_gameState; }
	bool GetChangeScene() { return m_changeScene; }

	void SetGameState(int a_gameState) { m_gameState = a_gameState; }
	void SetMapData(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetPlayerData(C_PlayerBase* a_pPlayer) { m_pPlayer = a_pPlayer; }

private:
	C_Map* m_pMap;
	C_PlayerBase* m_pPlayer;
	int m_gameState;


	KdTexture* m_pTex;
	StageNum nextStage{};
	bool m_changeScene = false;
	float m_alpha = 0.0f;
	float m_changeTime = 0;
	void SceneTrans();

	void StageTrans();
	void SearchNearStage(int h, int w, StageNum stage, float& nearestStageDistance, StageNum& nearestStage, bool& foundStage);

};

const enum GameState
{
	Title = 0,
	Select = 1,
	Game = 2,
	Result = 3
};