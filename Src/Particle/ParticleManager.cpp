#include "ParticleManager.h"

C_ParticleManager::C_ParticleManager() :
	m_pMap(nullptr),
	m_pPlayer(nullptr),
	m_pCamera(nullptr),
	m_pChipTex(nullptr),
	m_spawnIntervalTimer(3){}

void C_ParticleManager::Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer, C_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;
	m_pMap = a_pMap;
	m_pPlayer = a_pPlayer;
}

void C_ParticleManager::Update()
{
	UpdatePlayer();
}

void C_ParticleManager::Draw()
{
	DrawPlayer();
}

void C_ParticleManager::UpdatePlayer()
{
	bool moveStop = m_pPlayer->GetMove().x == 0;
	bool moveLeft = m_pPlayer->GetMove().x < 0;
	bool moveRight = m_pPlayer->GetMove().x > 0;
	bool jump = m_pPlayer->GetJumpFlg();

	if ((moveLeft || moveRight) && !jump)
	{
		S_ParticleObjects tmp;

		if (m_spawnIntervalTimer <= 0)
		{
			tmp.pos        = { m_pPlayer->GetPos().x ,m_pPlayer->GetPos().y - 60.0f };
			if (moveRight)tmp.pos.x -= PTEX_RADIUS;
			if (moveLeft)tmp.pos.x  += PTEX_RADIUS;
			tmp.move	   = { Rnd() * 2 - 1,Rnd() * 2 };
			tmp.draw       = { 0,0 };
			tmp.scale      = { 1,1 };
			tmp.color      = { 0,0,0,1 };
			tmp.size       = 7;
			tmp.m_lifeSpan = 45.0f;
			tmp.m_alpha    = 1.0f;
			tmp.data       = GetUnderChip(m_pPlayer->GetPos());

			m_grain.push_back(tmp);
			m_spawnIntervalTimer = 3;
		}
		m_spawnIntervalTimer--;
	}

	for (int i = 0;i < m_grain.size();i++)
	{
		if (m_grain[i].m_lifeSpan > 0)
		{
			m_grain[i].pos  += m_grain[i].move;
			m_grain[i].draw = m_grain[i].pos - m_pCamera->GetPos();
			m_grain[i].m_lifeSpan--;
			m_grain[i].m_alpha = m_grain[i].m_lifeSpan / 45.0f;
			m_grain[i].scale   = { m_grain[i].m_alpha, m_grain[i].m_alpha };

			m_grain[i].sMat = Math::Matrix::CreateScale(m_grain[i].scale.x, m_grain[i].scale.y, 1);
			m_grain[i].tMat = Math::Matrix::CreateTranslation(m_grain[i].draw.x, m_grain[i].draw.y, 0);
			m_grain[i].mat  = m_grain[i].sMat * m_grain[i].tMat;
		}
	}
	EraseGrain(m_grain);
}

void C_ParticleManager::DrawPlayer()
{
	for (auto& g : m_grain)
	{
		SHADER.m_spriteShader.SetMatrix(g.mat);
		switch (g.data)
		{
		case 0:
			break;
		case MapChip::dirt:
			SHADER.m_spriteShader.DrawTex(m_pChipTex, Math::Rectangle(CHIP_SIZE, 0, g.size, g.size), g.m_alpha);
			break;
		case MapChip::glass:
			SHADER.m_spriteShader.DrawTex(m_pChipTex, Math::Rectangle(CHIP_SIZE * 2, 0, g.size, g.size), g.m_alpha);
			break;
		case MapChip::stone:
			SHADER.m_spriteShader.DrawTex(m_pChipTex, Math::Rectangle(CHIP_SIZE * 3, 0, g.size, g.size), g.m_alpha);
			break;
		default:
			break;
		}
	}
}

int C_ParticleManager::GetUnderChip(Math::Vector2 a_pos)
{
	std::pair<int, int> underChip;
	
	underChip = m_pMap->GetIndex(a_pos.x + 32, a_pos.y - 92);
	if (underChip.second < MAP_HEIGHT && underChip.first < MAP_WIDTH &&
		underChip.second >= 0 && underChip.first >= 0)
	{
		return m_pMap->GetData(underChip.second, underChip.first);
	}
	else return 0;
}

float C_ParticleManager::Rnd()
{
	return (rand() / (float)RAND_MAX);
}

void C_ParticleManager::EraseGrain(std::vector<S_ParticleObjects>& a_grain)
{
	a_grain.erase(std::remove_if(a_grain.begin(), a_grain.end(),
		[](const S_ParticleObjects& grain)
		{
			return grain.m_lifeSpan <= 0;
		}),
		a_grain.end());
}
