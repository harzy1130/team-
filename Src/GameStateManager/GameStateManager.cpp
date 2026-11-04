#include"GameStateManager.h"

C_GameStateManager::C_GameStateManager() :
	m_gameState(GameState::Title),
	m_pMap(nullptr),
	m_pPlayer(nullptr),
	m_pBackTex(nullptr),
	m_pBlackTex(nullptr),
	m_bTransScene(false),
	m_scaleNum(1) {
}

void C_GameStateManager::Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer)
{
	m_renderTex.CreateRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT);
	m_renderTex2.CreateRenderTarget(SCREEN_WIDTH, SCREEN_HEIGHT);

	m_pMap = a_pMap;
	m_pPlayer = a_pPlayer;
}

void C_GameStateManager::Draw()
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_pBackTex, Math::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 1.0f);

	switch (m_gameState)
	{
	case GameState::Title:
		SHADER.m_spriteShader.DrawString(-200, 0, "Enter:選択ステージ\n1:テストステージ", Math::Vector4(0, 1, 1, 1));
		break;
		//case GameState::Select:
		//	break;
		//case GameState::Game:
		//	break;
		//case GameState::Result:
		//	break;
	default:
		break;
	}
}

void C_GameStateManager::SceneTransDraw()
{
	DynamicDraw2D();
	D3D.SetBackBuffer();

	float scaleX = 1480.0f / (SCREEN_WIDTH * m_ren.scale);
	float scaleY = 920.0f / (SCREEN_HEIGHT * m_ren.scale);

	Math::Matrix scalemat = Math::Matrix::CreateScale(scaleX, scaleY, 0);

	SHADER.m_spriteShader.SetMatrix(scalemat);
	SHADER.m_spriteShader.DrawTex(&m_renderTex2, Math::Rectangle(0, 0, SCREEN_WIDTH * m_ren.scale, SCREEN_HEIGHT * m_ren.scale), m_alpha);

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_pBlackTex, Math::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), m_alpha);
}

void C_GameStateManager::SetTexture(KdTexture* a_pBlackTex, KdTexture* a_pBackTex)
{
	m_pBlackTex = a_pBlackTex;
	m_pBackTex = a_pBackTex;
}

bool C_GameStateManager::GetTransScene()
{
	bool transScene = false;

	if (m_sceneTransCnt == 0)transScene = true;
	return transScene;
}

void C_GameStateManager::Update()
{
	//============================================================
	// デバッグ用
	// T : タイトル
	// R : セレクトステージ
	// 1 : テストステージ
	DebugKey();

	UpdateState();
	UpdateSceneTrans();

	//縮小率
	m_ren.scale = 1.0f / m_scaleNum;
}

void C_GameStateManager::UpdateSceneTrans()
{
	switch (m_gameState)
	{
	case GameState::Select:
		if (m_bTransScene && m_sceneTransCnt <= SCENETRANS_TIME)
		{
			m_sceneTransCnt++;
			m_scaleNum++;
		}
		m_alpha = m_sceneTransCnt / SCENETRANS_TIME;

		if (m_scaleNum >= 200)m_scaleNum = 200;
		break;
	case GameState::Game:
		if (!m_bTransScene)
		{
			if (m_loadCnt < LOAD_TIME)m_loadCnt++;

			if (m_sceneTransCnt > 0 && m_loadCnt >= LOAD_TIME)
			{
				m_sceneTransCnt--;
				m_scaleNum--;
			}
		}

		m_alpha = m_sceneTransCnt / SCENETRANS_TIME;
		if (m_scaleNum < 1)m_scaleNum = 1;
		break;
	default:
		break;
	}

	if (m_loadCnt >= LOAD_TIME)m_loadCnt = LOAD_TIME;
	if (m_sceneTransCnt >= SCENETRANS_TIME)m_sceneTransCnt = SCENETRANS_TIME;
	if (m_sceneTransCnt < 0)m_sceneTransCnt = 0;
	if (m_alpha >= 1.0f)m_alpha = 1.0f;
	if (m_alpha < 0.0f)m_alpha = 0.0f;
}

void C_GameStateManager::UpdateState()
{
	switch (m_gameState)
	{
	case GameState::Title:
		break;
	case GameState::Select:
		StageTrans();
		break;
	case GameState::Game:
		break;
	case GameState::Result:
		m_pMap->SetStageNum(StageNum::stageSelect);
		m_pMap->Load();
		m_pMap->Create();

		m_gameState = GameState::Select;
		break;
	default:
		break;
	}
}

void C_GameStateManager::DynamicDraw2D()
{
	m_renderTex.ClearRenerTarget(Math::Color(0, 0, 0, 1));
	m_renderTex.SetRenderTarget();

	//背景
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_pBackTex, Math::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 1.0f);

	m_pMap->Draw();
	m_pPlayer->Draw();

	//tmpTex2にtmpTexを縮小して描画する
	m_renderTex2.ClearRenerTarget(Math::Color(0, 0, 0, 1));
	m_renderTex2.SetRenderTarget();

	//座標は縮小画像が左上に配置されるように補正
	m_ren.transMat = Math::Matrix::CreateTranslation(LEFT_EDGE + (640.0f * m_ren.scale), TOP_EDGE - (360.0f * m_ren.scale), 0);
	m_ren.scaleMat = Math::Matrix::CreateScale(m_ren.scale, m_ren.scale, 0);
	m_ren.mat = m_ren.scaleMat * m_ren.transMat;

	//tmpTex2にtmpTexを描画
	SHADER.m_spriteShader.SetMatrix(m_ren.mat);
	SHADER.m_spriteShader.DrawTex(&m_renderTex, Math::Rectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT), 1.0f);
}

void C_GameStateManager::StageTrans()
{
	float nearStageDist = pow(60.0f, 2);
	StageNum nearStage{};
	bool foundStage = false;

	for (int h = 0;h < m_pMap->GetIdxMaxMin().second;h++)
	{
		for (int w = 0;w < m_pMap->GetIdxMaxMin().first;w++)
		{
			switch (m_pMap->GetData(h, w))
			{
			case MapChip::gate1:
				SearchNearStage(h, w, StageNum::stage1, nearStageDist, nearStage, foundStage);
				break;
			case MapChip::gate2:
				SearchNearStage(h, w, StageNum::stage2, nearStageDist, nearStage, foundStage);
				break;
			default:
				break;
			}
		}
	}

	if (foundStage && GetAsyncKeyState('F') & 0x8000)
	{
		m_nextStage = nearStage;
		m_bTransScene = true;
		m_loadCnt = 0;
	}
	if (m_sceneTransCnt >= SCENETRANS_TIME && m_bTransScene)
	{
		m_pMap->SetStageNum(m_nextStage);
		m_pMap->Load();
		m_pMap->Create();

		m_pPlayer->SetPos(0, 0);

		m_bTransScene = false;

		m_gameState = GameState::Game;
	}
}

void C_GameStateManager::SearchNearStage(int h, int w, StageNum stage, float& nearestStageDist, StageNum& nearestStage, bool& foundStage)
{
	float x, y, ToStageDist = 0;

	x = m_pPlayer->GetPos().x - m_pMap->GetPosition(h, w).x;
	y = m_pPlayer->GetPos().y - m_pMap->GetPosition(h, w).y;
	ToStageDist = pow(x, 2) + pow(y, 2);

	if (ToStageDist < nearestStageDist)
	{
		nearestStageDist = ToStageDist;
		nearestStage = stage;
		foundStage = true;
	}
}

void C_GameStateManager::DebugKey()
{
	if (GetAsyncKeyState('T') & 0x8000)
	{
		m_gameState = GameState::Title;
		m_pMap->SetStageNum(StageNum::stageSelect);
		m_pMap->Load();
		m_pMap->Create();
	}
	//ステージ選択画面
	if (GetAsyncKeyState(VK_RETURN) & 0x8000)
	{
		if (m_gameState == GameState::Title)
		{
			m_gameState = GameState::Select;
		}
	}
	// リザルト画面
	// そのままセレクトステージへ
	if (GetAsyncKeyState('R') & 0x8000)
	{
		m_pPlayer->SetPos(0, 0);

		m_bTransScene = false;
		m_alpha = 0.0f;
		m_sceneTransCnt = 0;
		m_scaleNum = 1;
		if (m_gameState == GameState::Game)
		{
			m_gameState = GameState::Result;
		}
	}
	//	テストステージ
	if (GetAsyncKeyState('1') & 0x8000)
	{
		if (m_gameState != GameState::Game || m_pMap->GetStageNum() != StageNum::Test)
		{
			m_pPlayer->SetPos(0, 0);

			m_pMap->SetStageNum(StageNum::Test);
			m_pMap->Load();
			m_pMap->Create();

			SetGameState(GameState::Game);
		}
	}
}
