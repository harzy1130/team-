#pragma once
#include"Constants/Constants.h"
#include"Map/Map.h"
#include"Player/PlayerBase.h"

class C_Map;
class C_PlayerBase;
class C_Boss;
class C_GameStateManager
{
public:
	C_GameStateManager();
	~C_GameStateManager() {};

	void Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer);
	void Update();
	void Draw();
	void SceneTransDraw();

	void SetTexture(KdTexture* a_pBlackTex, KdTexture* a_pBackTex);
	void SetGameState(int a_gameState) { m_gameState = a_gameState; }

	int GetGameState() { return m_gameState; }
	bool GetTransScene();
private:
	C_Map* m_pMap;
	C_PlayerBase* m_pPlayer;
	int m_gameState;

	KdTexture m_renderTex;   //レンダーターゲット用
	KdTexture m_renderTex2;  //レンダーターゲット切り替えようテクスチャ
	S_RenderTarget m_ren;//座標や行列計算に使う
	int m_scaleNum;	     //拡大率・縮小率用

	KdTexture* m_pBackTex;
	KdTexture* m_pBlackTex;
	StageNum m_nextStage{};
	bool m_bTransScene = false;
	float m_alpha = 0.0f;
	float m_sceneTransCnt = 0;
	float m_loadCnt = 0;

	void UpdateSceneTrans();
	void UpdateState();
	void DynamicDraw2D();

	void StageTrans();
	void SearchNearStage(int h, int w, StageNum stage, float& nearestStageDist, StageNum& nearestStage, bool& foundStage);

	void DebugKey();//********************:::提出時消す
};