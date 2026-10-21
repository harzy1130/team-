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

	int GetGameState() { return m_gameState; }

	void SetGameState(int a_gameState) { m_gameState = a_gameState; }
	void SetMapData(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetPlayerData(C_PlayerBase* a_pPlayer) { m_pPlayer = a_pPlayer; }

private:
	C_Map* m_pMap;
	C_PlayerBase* m_pPlayer;
	int m_gameState;

	void StageTransition();
	void SearchNearStage(int h,int w,StageNum stage,float& nearestStageDistance,StageNum& nearestStage, bool& foundStage);
	
};

const enum GameState
{
	Title  = 0,
	Select = 1,
	Game   = 2,
	Result = 3
};