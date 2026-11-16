#include "ParticleManager.h"

C_ParticleManager::C_ParticleManager()
	: m_pMap(nullptr)
	, m_pPlayer(nullptr)
	, m_pCamera(nullptr)
	, m_pChipTex(nullptr)
	, m_pGrainTex(nullptr)
	, m_pAtkUpTex(nullptr)
	, m_pDefUpTex(nullptr)
	, m_pSmokeTex(nullptr)
	, m_spawnAtkIntervalTimer(0)
	, m_bAtkRepeat(false)
	, m_atkRpeatTime(0)
{
}

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

void C_ParticleManager::SetTexture(KdTexture* a_pChipTex, KdTexture* a_pGrainTex, KdTexture* a_pAtkUpTex, KdTexture* a_pDefUpTex, KdTexture* a_pSmokeTex)
{
	m_pChipTex = a_pChipTex;
	m_pGrainTex = a_pGrainTex;
	m_pAtkUpTex = a_pAtkUpTex;
	m_pDefUpTex = a_pDefUpTex;
	m_pSmokeTex = a_pSmokeTex;
}

void C_ParticleManager::UpdatePlayer()
{
	bool jump = m_pPlayer->GetJumpFlg();
	bool moveRight = m_pPlayer->GetMove().x > 0;
	bool moveLeft = m_pPlayer->GetMove().x < 0;
	bool moveStop = m_pPlayer->GetMove().x == 0;
	bool AtkUp = m_pPlayer->GetGearA()->GetWhileActive() == true;
	bool poisning = GetAsyncKeyState('P') & 0x8000;
	//bool DefUp = m_pPlayer->GetGearB()->GetWhileActive() == true;

	//UpdateParticle();

	//if (GetAsyncKeyState('J') & 0x8000)
	//{
	//	startParticle(ParticleType::dust, m_pPlayer->GetPos());
	//}


	if ((moveLeft || moveRight) && !jump)
	{
		S_ParticleObjects dust;
		if (m_spawnIntervalTimer <= 0)
		{
			dust.type = ParticleType::dust;
			dust.pos = { m_pPlayer->GetPos().x, m_pPlayer->GetPos().y - 60.0f };
			if (moveRight) dust.pos.x -= PTEX_RADIUS;
			if (moveLeft) dust.pos.x += PTEX_RADIUS;
			dust.move = { Rnd() * 2 - 1, Rnd() * 2 };
			dust.draw = { 0,0 };
			dust.scale = { 1,1 };
			dust.rect = { 0,0,0,0 };
			dust.color = { 1,1,1,1 };
			dust.size = Rnd() * 7 + 4;
			dust.lifeSpan = 45.0f;
			dust.alpha = 1.0f;
			dust.data = GetUnderChip(m_pPlayer->GetPos());
			m_grain.push_back(dust);
			m_spawnIntervalTimer = 3;
		}
		m_spawnIntervalTimer--;
	}

	if (AtkUp)
	{
		m_bAtkRepeat = true;
	}

	if (m_bAtkRepeat)
	{
		if (m_spawnAtkIntervalTimer <= 0)
		{
			S_ParticleObjects atk{};
			atk.type = ParticleType::atkUp;
			atk.pos = { m_pPlayer->GetPos().x + Rnd() * 48.0f - 24.0f, m_pPlayer->GetPos().y - 20.0f };
			atk.move = { m_pPlayer->GetMove().x, 3.0f };
			atk.draw = { 0, 0 };
			atk.scale = { 1, 1 };
			atk.rect = { 0, 0, 32, 32 };
			atk.color = { 1, 0.7f, 0.7f, 1 };
			atk.size = 32;
			atk.lifeSpan = 45.0f;
			atk.alpha = 1.0f;
			atk.data = 1;
			atk.IsActive = false;
			m_grain.push_back(atk);
			m_spawnAtkIntervalTimer = 5.0f;
		}

		m_spawnAtkIntervalTimer--;
		m_atkRpeatTime++;
		if (m_atkRpeatTime >= 60.0f)
		{
			m_bAtkRepeat = false;
			m_atkRpeatTime = 0.0f;
		}
	}

	if (GetAsyncKeyState('I')&0x8000)
	{
		m_bDefRepeat = true;
	}
	if (m_bDefRepeat)
	{
		S_ParticleObjects Def1;
		S_ParticleObjects Def2;
		if (m_spawnDefIntervalTimer <= 0 && m_bDefRepeat)
		{
			Def1.type = ParticleType::defUp1;
			Def1.pos = { m_pPlayer->GetPos().x + Rnd() * 48 - 24, m_pPlayer->GetPos().y - 20 };
			Def1.move = { m_pPlayer->GetMove().x, 3 };
			Def1.draw = { 0,0 };
			Def1.scale = { 1,1 };
			Def1.rect = { 0,0,0,0 };
			Def1.color = { 1,1,1,1 };
			Def1.size = 32;
			Def1.lifeSpan = 45;
			Def1.alpha = 1.0f;

			Def2.type = ParticleType::defUp2;
			Def2.pos = { m_pPlayer->GetPos().x + Rnd() * 48 - 24, m_pPlayer->GetPos().y - 20 };
			Def2.move = { m_pPlayer->GetMove().x, 3 };
			Def2.draw = { 0,0 };
			Def2.scale = { 1,1 };
			Def2.rect = { 0,0,0,0 };
			Def2.color = { 1,1,1,1 };
			Def2.size = 32;
			Def2.lifeSpan = 45;
			Def2.alpha = 1.0f;
			m_grain.push_back(Def1);
			m_grain.push_back(Def2);
			m_spawnDefIntervalTimer = 5;
		}
		else m_spawnDefIntervalTimer--;
	}
	if (m_bDefRepeat) m_defRpeatTime++;
	if (m_defRpeatTime >= 60)
	{
		m_bDefRepeat = false;
		m_defRpeatTime = 0;
	}

	if (GetAsyncKeyState('P') & 0x8000)
	{
		m_bPoisonRepeat = true;
	}
	if (m_bPoisonRepeat)
	{
		S_ParticleObjects Poison;
		if (m_spawnPoisonIntervalTimer <= 0 && m_bPoisonRepeat)
		{
			for (int i = 0; i < 4; i++)
			{
				Poison.type = ParticleType::poison;
				Poison.pos = { m_pPlayer->GetPos().x + Rnd() * 48 - 24, m_pPlayer->GetPos().y + Rnd() * 60 - 30 };
				Poison.move = { m_pPlayer->GetMove().x, 3 };
				Poison.draw = { 0,0 };
				Poison.scale = { 1,1 };
				Poison.rect = { 0,0,0,0 };
				Poison.color = { 1,1,1,0 };
				Poison.size = 64;
				Poison.lifeSpan = 60;
				Poison.alpha = 1.0f;
				Poison.angle = 10;
				m_grain.push_back(Poison);
			}
			m_spawnPoisonIntervalTimer = 60;
		}
		m_spawnPoisonIntervalTimer--;
	}
	if (m_bPoisonRepeat) m_poisonRpeatTime++;
	if (m_poisonRpeatTime >= 60)
	{
		m_bPoisonRepeat = false;
		m_poisonRpeatTime = 0;
	}

	for (auto& g : m_grain)
	{
		if (g.lifeSpan > 0 && g.type == ParticleType::dust)
		{
			g.pos += g.move;
			g.draw = g.pos - m_pCamera->GetPos();
			g.lifeSpan--;
			g.alpha = g.lifeSpan / 45.0f;
			g.rect = { CHIP_SIZE * g.data, 0, g.size, g.size };
			g.color = { 1,1,1,g.alpha };
			g.scale = { g.alpha, g.alpha };
			g.sMat = Math::Matrix::CreateScale(g.scale.x, g.scale.y, 1);
			g.tMat = Math::Matrix::CreateTranslation(g.draw.x, g.draw.y, 0);
			g.mat = g.sMat * g.tMat;
		}
	}


	for (auto& g : m_grain)
	{
		if (g.lifeSpan > 0 && g.type == ParticleType::defUp1)
		{
			g.pos += g.move;
			g.draw = g.pos - m_pCamera->GetPos();
			if (moveStop)g.move.x = 0;
			g.lifeSpan--;
			g.alpha = (g.lifeSpan / 45) / 1.2f;
			g.rect = { 0,0,g.size,g.size };
			g.color = { 1,1,0.7f,g.alpha };
			g.scale = { g.alpha, g.alpha };
			g.sMat = Math::Matrix::CreateScale(g.scale.x, g.scale.y * 2.0f, 1);
			g.tMat = Math::Matrix::CreateTranslation(g.draw.x, g.draw.y, 0);
			g.mat = g.sMat * g.tMat;
		}
	}

	for (auto& g : m_grain)
	{
		if (g.lifeSpan > 0 && g.type == ParticleType::defUp2)
		{
			g.pos += g.move;
			g.draw = g.pos - m_pCamera->GetPos();
			if (moveStop)g.move.x = 0;
			g.lifeSpan--;
			g.alpha = g.lifeSpan / 45;
			g.rect = { 32,0,g.size,g.size };
			g.color = { 1,1,1.0f,g.alpha };
			g.scale = { g.alpha, g.alpha };
			g.sMat = Math::Matrix::CreateScale(g.scale.x, g.scale.y, 1);
			g.tMat = Math::Matrix::CreateTranslation(g.draw.x, g.draw.y, 0);
			g.mat = g.sMat * g.tMat;
		}
	}



	for (auto& grain : m_grain)
	{
		if (grain.lifeSpan > 0.0f && grain.type == ParticleType::atkUp)
		{
			grain.pos += grain.move;
			grain.draw = grain.pos - m_pCamera->GetPos();
			if (moveStop)grain.move.x = 0;
			grain.lifeSpan--;
			grain.alpha = (grain.lifeSpan / 45.0f) / 1.2f;
			grain.rect = { 0, 0, grain.size, grain.size };
			grain.color = { 1, 0.7f, 0.7f, grain.alpha };
			grain.scale = { grain.alpha, grain.alpha };
			grain.sMat = Math::Matrix::CreateScale(grain.scale.x, grain.scale.y * 2.0f, 1);
			grain.tMat = Math::Matrix::CreateTranslation(grain.draw.x, grain.draw.y, 0);
			grain.mat = grain.sMat * grain.tMat;
		}
	}

	for (auto& g : m_grain)
	{
		if (g.lifeSpan > 0 && g.type == ParticleType::poison)
		{
			g.pos += g.move;
			g.draw = g.pos - m_pCamera->GetPos();
			if (moveStop)g.move.x = 0;
			g.lifeSpan--;
			g.alpha = (g.lifeSpan / 90) / 1.2f;
			g.angle += Rnd() * 10 - 5;
			g.rect = { 0,0,g.size,g.size };
			g.color = { 0.30f, 0.50f, 0.16f, g.alpha };
			g.scale = { g.lifeSpan / 60, g.lifeSpan / 60 };
			g.sMat = Math::Matrix::CreateScale(g.scale.x, g.scale.y, 1);
			g.rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(g.angle));
			g.tMat = Math::Matrix::CreateTranslation(g.draw.x, g.draw.y, 0);
			g.mat = g.sMat * g.rMat * g.tMat;
		}
	}

	EraseGrain(m_grain);
}

void C_ParticleManager::DrawPlayer()
{
	for (auto& grain : m_grain)
	{
		if (grain.lifeSpan <= 0.0f) continue;
		SHADER.m_spriteShader.SetMatrix(grain.mat);
		switch (grain.type)
		{
		case ParticleType::dust:
			D3D.SetBlendState(BlendMode::Add);
			SHADER.m_spriteShader.DrawTex(m_pChipTex, 0, 0, &grain.rect, &grain.color);
			D3D.SetBlendState(BlendMode::Alpha);
			break;
		case ParticleType::atkUp:
			D3D.SetBlendState(BlendMode::Add);
			SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &grain.rect, &grain.color);
			D3D.SetBlendState(BlendMode::Alpha);
			break;
		case ParticleType::defUp1:
			D3D.SetBlendState(BlendMode::Add);
			SHADER.m_spriteShader.DrawTex(m_pDefUpTex, 0, 0, &grain.rect, &grain.color);
			D3D.SetBlendState(BlendMode::Alpha);
			break;
		case ParticleType::defUp2:
			D3D.SetBlendState(BlendMode::Add);
			SHADER.m_spriteShader.DrawTex(m_pDefUpTex, 0, 0, &grain.rect, &grain.color);
			D3D.SetBlendState(BlendMode::Alpha);
			break;
		case ParticleType::poison:
			SHADER.m_spriteShader.DrawTex(m_pSmokeTex, 0, 0, &grain.rect, &grain.color);
			D3D.SetBlendState(BlendMode::Add);
			D3D.SetBlendState(BlendMode::Alpha);
			break;
		default:
			break;
		}
	}


	//for (auto& p : m_particles)
	//{
	//	SHADER.m_spriteShader.SetMatrix(p.mat);
	//	SHADER.m_spriteShader.DrawTex(m_pChipTex, 0, 0, &p.rect, &Math::Color{1,1,1,1});

	//}
}

//void C_ParticleManager::startParticle(ParticleType a_type, Math::Vector2 a_pos)
//{
//	for (auto& particle : m_particles)
//	{
//		if (particle.type == a_type)
//		{
//			const auto* config = GetConfig(a_type);
//
//			if (particle.generateIntervalTime <= config->generateInterval && particle.IsActive)
//			{
//				spawnParticle(a_type, a_pos);
//				particle.generateIntervalTime = 0;
//			}
//			else particle.generateIntervalTime++;
//
//			if (particle.IsActive)particle.elapsedTime++;
//			if (particle.elapsedTime >= config->generateTime)
//			{
//				particle.IsActive = false;
//				particle.elapsedTime = 0;
//			}
//		}
//	}
//}
//
//void C_ParticleManager::spawnParticle(ParticleType a_type, Math::Vector2 a_pos)
//{
//	for (auto& particle : m_particles)
//	{
//		if (particle.type == a_type)
//		{
//			const auto* config = GetConfig(a_type);
//			particle.type = a_type;
//
//
//			if (particle.lifeSpan > 0)
//			{
//				particle.lifeSpan--;
//				particle.pos = a_pos;
//				particle.move = config->move;
//
//			}
//		}
//	}
//}
//void C_ParticleManager::UpdateParticle()
//{
//	for (auto& particle : m_particles)
//	{
//		particle.pos = { m_pPlayer->GetPos().x,m_pPlayer->GetPos().y - 60.0f };
//		particle.pos -= m_pCamera->GetPos();
//		particle.pos += particle.move;
//		particle.rect = { CHIP_SIZE * 1, 0, 32,32 };
//		particle.smat = Math::Matrix::CreateScale(1, 1, 1);
//		particle.tmat = Math::Matrix::CreateTranslation(particle.pos.x, particle.pos.y, 0);
//		particle.mat = particle.smat * particle.tmat;
//	}
//}
//
//const S_ParticleConfig* C_ParticleManager::GetConfig(ParticleType a_type)
//{
//	for (auto& config : m_particleConfig)
//	{
//		if (config.type == a_type)
//		{
//			return &config;
//		}
//	}
//	return nullptr;
//}

int C_ParticleManager::GetUnderChip(Math::Vector2 a_pos)
{
	const auto underChip = m_pMap->GetIndex(a_pos.x + 32, a_pos.y - 92);
	if (underChip.second < MAP_HEIGHT && underChip.first < MAP_WIDTH && underChip.second >= 0 && underChip.first >= 0)
	{
		return m_pMap->GetData(underChip.second, underChip.first);
	}
	return 0;
}

float C_ParticleManager::Rnd()
{
	return rand() / static_cast<float>(RAND_MAX);
}

void C_ParticleManager::EraseGrain(std::vector<S_ParticleObjects>& a_grain)
{
	a_grain.erase(std::remove_if(a_grain.begin(), a_grain.end(),
		[](const S_ParticleObjects& grain)
		{
			return grain.lifeSpan <= 0.0f;
		}), a_grain.end());
}
