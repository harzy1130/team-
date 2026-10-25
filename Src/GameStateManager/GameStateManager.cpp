#include"GameStateManager.h"

C_GameStateManager::C_GameStateManager() : m_gameState(GameState::Title),
m_pMap(nullptr),
m_pPlayer(nullptr) {
}

void C_GameStateManager::Draw()
{
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
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(0, 0, 0));
	SHADER.m_spriteShader.DrawTex(m_pTex, Math::Rectangle(0, 0, 1280, 720), m_alpha);
}

void C_GameStateManager::Update()
{
	//============================================================
	// デバッグ用
	// タイトルに戻る
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
		m_changeScene = false;
		m_alpha = 0.0f;
		m_changeTime = 0;
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
			m_pPlayer->SetPos(150, -200);
			SetGameState(GameState::Game);
		}
	}
	//============================================================


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
}

void C_GameStateManager::SceneTrans()
{
	switch (m_gameState)
	{
	case GameState::Select:
		if (m_changeScene)
		{
			if (m_changeTime <= 60)m_changeTime++;
		}
		m_alpha = m_changeTime / 60;
		break;
	case GameState::Game:
		if (!m_changeScene)
		{
			if (m_changeTime > 0)m_changeTime--;
		}
		m_alpha = m_changeTime / 60;
		break;
	default:
		break;
	}

	if (m_changeTime >= 60)m_changeTime = 60;
	if (m_changeTime < 0)m_changeTime = 0;
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
		nextStage = nearStage;
		m_changeScene = true;
	}
	if (m_changeTime >= 60 && m_changeScene)
	{
		m_pMap->SetStageNum(nextStage);
		m_pMap->Load();
		m_pMap->Create();

		m_pPlayer->SetPos(0, -200);

		m_changeScene = false;

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