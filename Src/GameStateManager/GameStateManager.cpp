#include"GameStateManager.h"

void C_GameStateManager::Init()
{
	m_gameState = GameState::Title;
	m_pMap = nullptr;
	m_pPlayer = nullptr;

	m_back.x = 0;
	m_back.y = 0;

	m_tmpTex.CreateRenderTarget(1280, 720);

	wipe.x = 0;
	wipe.y = 0;
	wipe.scaleX = 0.5f;
	wipe.scaleY = 0.5f;

	m_scaleNum = 1;
	m_tmpTex2.CreateRenderTarget(1280, 720);
}

void C_GameStateManager::Draw()
{
	SHADER.m_spriteShader.SetMatrix(m_back.mat);
	SHADER.m_spriteShader.DrawTex(m_pBackTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	switch (m_gameState)
	{
	case GameState::Title:
		SHADER.m_spriteShader.DrawString(-200, 0, "タイトル(仮)\nEnterでセレクトステージ遷移\n1 でテストステージ遷移", Math::Vector4(0, 1, 1, 1));
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

void C_GameStateManager::SceneChangeDraw()
{
	DynamicDraw2D();
	D3D.SetBackBuffer();

	float scaleX = 1480.0f / (1280.0f * m_tmp2.scale);
	float scaleY = 920.0f / (720.0f * m_tmp2.scale);

	Math::Matrix scalemat = Math::Matrix::CreateScale(scaleX, scaleY, 0);

	SHADER.m_spriteShader.SetMatrix(scalemat);
	SHADER.m_spriteShader.DrawTex(&m_tmpTex2, Math::Rectangle(0, 0, 1280 * m_tmp2.scale, 720 * m_tmp2.scale), m_alpha);

	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(0, 0, 1280, 720), m_alpha);
}

void C_GameStateManager::Update()
{
	//============================================================
	// デバッグ用
	// T : タイトル
	// R : セレクトステージ
	// 1 : テストステージ
	DebugKey();


	switch (m_gameState)
	{
	case GameState::Title:
		m_pPlayer->SetPos(0, -200);
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

		m_pPlayer->SetPos(0, -200);

		m_gameState = GameState::Select;
		break;
	default:
		break;
	}

	SceneTrans();

	//縮小率
	m_tmp2.scale = 1.0f / m_scaleNum;
}

void C_GameStateManager::DynamicDraw2D()
{
	m_tmpTex.ClearRenerTarget(Math::Color(0, 0, 0, 1));
	m_tmpTex.SetRenderTarget();

	//背景
	SHADER.m_spriteShader.SetMatrix(m_back.mat);
	SHADER.m_spriteShader.DrawTex(m_pBackTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);

	m_pMap->Draw();
	m_pPlayer->Draw();

	//tmpTex2にtmpTexを縮小して描画する
	m_tmpTex2.ClearRenerTarget(Math::Color(0, 0, 0, 1));
	m_tmpTex2.SetRenderTarget();

	//座標は縮小画像が左上に配置されるように補正
	m_tmp2.transMat = Math::Matrix::CreateTranslation(-640.0f + (640.0f * m_tmp2.scale), 360.0f - (360.0f * m_tmp2.scale), 0);
	m_tmp2.scaleMat = Math::Matrix::CreateScale(m_tmp2.scale, m_tmp2.scale, 0);
	m_tmp2.mat = m_tmp2.scaleMat * m_tmp2.transMat;

	//tmpTex2にtmpTexを描画
	SHADER.m_spriteShader.SetMatrix(m_tmp2.mat);
	SHADER.m_spriteShader.DrawTex(&m_tmpTex, Math::Rectangle(0, 0, 1280, 720), 1.0f);
}

void C_GameStateManager::SceneTrans()
{
	switch (m_gameState)
	{
	case GameState::Select:
		if (m_bChangeScene && m_sceneTransCnt <= m_sceneTransTime)
		{
			m_sceneTransCnt++;
			m_scaleNum++;
		}
		m_alpha = m_sceneTransCnt / m_sceneTransTime;

		if (m_scaleNum >= 200)m_scaleNum = 200;
		break;
	case GameState::Game:
		if (!m_bChangeScene)
		{
			if (m_loadCnt < m_loadTime)m_loadCnt++;
			if (m_sceneTransCnt > 0 && m_loadCnt >= m_loadTime)
			{
				m_sceneTransCnt--;
				m_scaleNum--;
			}
		}
		m_alpha = m_sceneTransCnt / m_sceneTransTime;

		if (m_scaleNum < 1)m_scaleNum = 1;
		break;
	default:
		break;
	}

	if (m_loadCnt >= m_loadTime)m_loadCnt = m_loadTime;

	if (m_sceneTransCnt >= m_sceneTransTime)m_sceneTransCnt = m_sceneTransTime;
	if (m_sceneTransCnt < 0)m_sceneTransCnt = 0;
	if (m_alpha >= 1.0f)m_alpha = 1.0f;
	if (m_alpha < 0.0f)m_alpha = 0.0f;
}

void C_GameStateManager::StageTrans()
{
	float nearStageDistance = pow(60.0f, 2);
	StageNum nearStage{};
	bool foundStage = false;

	for (int h = 0;h < m_pMap->GetIdxMaxMin().second;h++)
	{
		for (int w = 0;w < m_pMap->GetIdxMaxMin().first;w++)
		{
			switch (m_pMap->GetData(h, w))
			{
			case MapChip::s1:
				SearchNearStage(h, w, StageNum::stage1, nearStageDistance, nearStage, foundStage);
				break;
			case MapChip::s2:
				SearchNearStage(h, w, StageNum::stage2, nearStageDistance, nearStage, foundStage);
				break;
			default:
				break;
			}
		}
	}

	if (foundStage && GetAsyncKeyState('F') & 0x8000)
	{
		m_nextStage = nearStage;
		m_bChangeScene = true;
		m_loadCnt = 0;
	}
	if (m_sceneTransCnt >= 60 && m_bChangeScene)
	{
		m_pMap->SetStageNum(m_nextStage);
		m_pMap->Load();
		m_pMap->Create();

		m_pPlayer->SetPos(0, -200);

		m_bChangeScene = false;

		m_gameState = GameState::Game;
	}
}

void C_GameStateManager::SearchNearStage(int h, int w, StageNum stage, float& nearestStageDistance, StageNum& nearestStage, bool& foundStage)
{
	float x, y, ToStageDistance = 0;

	x = m_pPlayer->GetPos().x - m_pMap->GetPosition(h, w).x;
	y = m_pPlayer->GetPos().y - m_pMap->GetPosition(h, w).y;
	ToStageDistance = pow(x, 2) + pow(y, 2);

	if (ToStageDistance < nearestStageDistance)
	{
		nearestStageDistance = ToStageDistance;
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
		m_bChangeScene = false;
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
			m_pMap->SetStageNum(StageNum::Test);
			m_pMap->Load();
			m_pMap->Create();
			//m_pPlayer->SetPos(150, -200);
			SetGameState(GameState::Game);
		}
	}
	//============================================================
}
