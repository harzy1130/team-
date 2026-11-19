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
{
}

void C_ParticleManager::Init(C_Map* a_pMap, C_PlayerBase* a_pPlayer, C_Camera* a_pCamera)
{
	srand(time(0));
	m_pCamera = a_pCamera;
	m_pMap = a_pMap;
	m_pPlayer = a_pPlayer;
	m_pEffect = m_pPlayer->GetEffect();

	S_Config DUST;
	DUST.type = ParticleType::dust;
	DUST.pos = { m_pPlayer->GetPos().x, m_pPlayer->GetPos().y - 60.0f };
	DUST.move = { m_pPlayer->GetMove().x,3 };
	DUST.randomOffsetX = { 0,0 };
	DUST.randomOffsetY = { 0,0 };
	DUST.randomOffsetMoveX = { 1.0,-0.5 };
	DUST.randomOffsetMoveY = { 0.5,0 };
	DUST.size = 7;
	DUST.angle = 0;
	DUST.alpha = 0;
	DUST.lifeSpan = 45;
	DUST.IsRepeat = true;
	DUST.generateNum = 1;
	DUST.generateTime = 30;
	DUST.generateInterval = 60;
	DUST.IsFollowPlayer = true;
	m_config.push_back(DUST);

	S_Config ATKUP;
	ATKUP.type = ParticleType::atkUp;
	ATKUP.pos = { 0,0 };
	ATKUP.move = { m_pPlayer->GetMove().x,3 };
	ATKUP.randomOffsetX = { 32,-32 };
	ATKUP.randomOffsetY = { 30,-30 };
	ATKUP.randomOffsetMoveX = { 0,0 };
	ATKUP.randomOffsetMoveY = { 0,0 };
	ATKUP.size = 32;
	ATKUP.angle = 0;
	ATKUP.alpha = 0;
	ATKUP.lifeSpan = 45;
	ATKUP.IsRepeat = true;
	ATKUP.generateNum = 1;
	ATKUP.generateTime = 60;
	ATKUP.generateInterval = 3;
	ATKUP.IsFollowPlayer = true;
	m_config.push_back(ATKUP);

	S_Config POISON;
	POISON.type = ParticleType::poison;
	POISON.pos = { 0,0 };
	POISON.move = { m_pPlayer->GetMove().x,3 };
	POISON.randomOffsetX = { 24,-24 };
	POISON.randomOffsetY = { -20,-30 };
	POISON.randomOffsetMoveX = { 0,0 };
	POISON.randomOffsetMoveY = { 0,0 };
	POISON.size = 16;
	POISON.angle = 10;
	POISON.alpha = 0;
	POISON.lifeSpan = 50;
	POISON.IsRepeat = true;
	POISON.generateNum = 1;
	POISON.generateTime = 60;
	POISON.generateInterval = 25;
	POISON.IsFollowPlayer = true;
	m_config.push_back(POISON);

	S_Config POISON2;
	POISON2.type = ParticleType::bleed;
	POISON2.pos = { 0,0 };
	POISON2.move = { 0,0 };
	POISON2.randomOffsetX = { 24,-24 };
	POISON2.randomOffsetY = { 5,-20 };
	POISON2.randomOffsetMoveX = { 2.0f,-1.0f };
	POISON2.randomOffsetMoveY = { 1.5,0 };
	POISON2.size = 64;
	POISON2.angle = 10;
	POISON2.alpha = 0;
	POISON2.lifeSpan = 45;
	POISON2.IsRepeat = false;
	POISON2.generateNum = 4;
	POISON2.generateTime = 1;
	POISON2.generateInterval = 25;
	POISON2.IsFollowPlayer = true;
	m_config.push_back(POISON2);


}

void C_ParticleManager::Update()
{
	UpdatePlayer();
	UpdateEmitter();
	UpdateParticle();
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
	bool poisning = m_pEffect->GetPoison();

	//UpdateEmitterRequest(ParticleType::dust, (moveLeft || moveRight) && !jump, { m_pPlayer->GetPos().x,m_pPlayer->GetPos().y - 60.0f });
	UpdateEmitterRequest(ParticleType::atkUp, AtkUp, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::poison, poisning, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::bleed, poisning, m_pPlayer->GetPos());
}
void C_ParticleManager::DrawPlayer()
{
	for (auto& p : m_particles)
	{
		switch (p.type)
		{
		case ParticleType::dust:
			DrawDust(p);
			break;
		case ParticleType::atkUp:
			DrawAtk(p);
			break;
		case ParticleType::poison:
			DrawPoison(p);
			break;
		case ParticleType::bleed:
			DrawPoison2(p);
			break;
		default:
			break;
		}

	}
}

void C_ParticleManager::startEmitter(ParticleType a_type, Math::Vector2 a_pos)
{
	const auto* config = GetConfig(a_type);
	if (config == nullptr)return;

	S_Emitter emitter;
	emitter.type = config->type;
	emitter.pos = a_pos;
	emitter.elapsedTime = 0;
	emitter.generateIntervalTime = 0;
	emitter.IsActive = true;
	emitter.IsRepeat = config->IsRepeat;
	emitter.IsFollowPlayer = config->IsFollowPlayer;
	m_emitter.push_back(emitter);
}

void C_ParticleManager::spawnParticle(ParticleType a_type, Math::Vector2 a_pos)
{
	const auto* config = GetConfig(a_type);

	float randomOffsetX = (config->randomOffsetX.second + Rnd() * (config->randomOffsetX.first - config->randomOffsetX.second));
	float randomOffsetY = (config->randomOffsetY.second + Rnd() * (config->randomOffsetY.first - config->randomOffsetY.second));
	float randomOffsetMoveX = (config->randomOffsetMoveX.second + Rnd() * (config->randomOffsetMoveX.first - config->randomOffsetMoveX.second));
	float randomOffsetMoveY = (config->randomOffsetMoveY.second + Rnd() * (config->randomOffsetMoveY.first - config->randomOffsetMoveY.second));

	S_Particle particle;
	particle.type = a_type;
	particle.pos = { a_pos.x + randomOffsetX,   a_pos.y + randomOffsetY };
	particle.move = { config->move.x + randomOffsetMoveX,config->move.y + randomOffsetMoveY };
	particle.size = a_type == ParticleType::dust ? static_cast<long>(Rnd() * 7 + 4) : config->size;
	particle.angle = config->angle;
	particle.alpha = config->alpha;
	particle.lifeSpan = config->lifeSpan;
	particle.IsActive = true;
	particle.data = 0;

	m_particles.push_back(particle);
}

void C_ParticleManager::UpdateEmitter()
{
	for (auto& emitter : m_emitter)
	{
		const auto config = GetConfig(emitter.type);
		if (!emitter.IsActive)continue;


		emitter.generateIntervalTime--;
		if (emitter.generateIntervalTime <= 0)
		{
			if (emitter.IsFollowPlayer)emitter.pos = m_pPlayer->GetPos();
			if (emitter.type == ParticleType::dust)
				if (emitter.type == ParticleType::dust)
				{
					emitter.pos.y -= 60.0f;
					if (m_pPlayer->GetMove().x > 0)emitter.pos.x -= PTEX_RADIUS;
					else if (m_pPlayer->GetMove().x < 0)emitter.pos.x += PTEX_RADIUS;
				}
			for (int i = 0;i < config->generateNum;i++)
			{
				spawnParticle(emitter.type, emitter.pos);
			}
			emitter.generateIntervalTime = config->generateInterval;
		}

		emitter.elapsedTime++;
		if (emitter.elapsedTime >= config->generateTime)
		{
			FinishEmitter(emitter);
		}
	}
}

void C_ParticleManager::FinishEmitter(S_Emitter& emitter)
{
	if (emitter.IsRepeat)
	{
		emitter.generateIntervalTime = 0;
		emitter.elapsedTime = 0;
	}
	else
	{
		emitter.IsActive = false;
		emitter.elapsedTime = 0;
	}
}

void C_ParticleManager::UpdateEmitterRequest(ParticleType a_type, bool IsRequested, Math::Vector2 a_pos)
{
	const auto config = GetConfig(a_type);
	if (config == nullptr)return;

	// すでに再生中のエミッターがある場合は何もしない
	// ただし、IsRepeatがtrueの場合は再生中でも再度開始する
	// 逆に、IsRepeatがfalseの場合は再生中のエミッターがある場合は何もしない
	if (IsRequested)
	{
		if (config->IsRepeat)
		{
			if (!IsPlaying(a_type))startEmitter(a_type, a_pos);
		}
		else
		{
			if (std::find(m_playedTypes.begin(), m_playedTypes.end(), a_type) == m_playedTypes.end())
			{
				startEmitter(a_type, a_pos);
				m_playedTypes.push_back(a_type);
			}
		}
	}
	else
	{
		for (auto& emitter : m_emitter)
		{
			if (emitter.type == a_type && emitter.IsRepeat)
			{
				emitter.IsActive = false;
			}
		}

		m_playedTypes.erase(std::remove_if(m_playedTypes.begin(), m_playedTypes.end(),
			[a_type](const ParticleType& a_playedTypes)
			{
				return a_playedTypes == a_type;
			}), m_playedTypes.end());
	}
}

void C_ParticleManager::UpdateParticle()
{
	for (auto& particle : m_particles)
	{
		const auto config = GetConfig(particle.type);

		if (particle.lifeSpan >= 0)
		{
			if (particle.type == ParticleType::dust)particle.data = GetUnderChip(particle.pos);
			particle.pos += particle.move;
			particle.alpha = particle.lifeSpan / config->lifeSpan;
			particle.lifeSpan--;
		}
	}
	EraseParticle(m_particles);
}

void C_ParticleManager::DrawDust(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix tMat;
	Math::Matrix mat;
	float alpha = 0;
	alpha = particle.alpha / 1.2f;
	rect = { CHIP_SIZE * particle.data, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };
	sMat = Math::Matrix::CreateScale(alpha, alpha, 1);
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * tMat;

	//D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pChipTex, 0, 0, &rect, &color);
	//D3D.SetBlendState(BlendMode::Alpha);
}
void C_ParticleManager::DrawAtk(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Vector2 move = { Rnd() * 2 - 1,3 };
	draw += move;
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 1.2f;

	rect = { 0, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };

	sMat = Math::Matrix::CreateScale(alpha, alpha * 2.0f, 1);
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * tMat;

	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &rect, &color);
}

void C_ParticleManager::DrawPoison(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;
	float alpha = 0;


	alpha = particle.alpha / 1.2f;
	rect = { 0, 0, particle.size, particle.size };
	color = { 0.27f, 0.94f, 0.00f, alpha };

	sMat = Math::Matrix::CreateScale(alpha, alpha, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pGrainTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);

	color = { 0.25f, 0.38f, 0.13f, alpha };
	sMat = Math::Matrix::CreateScale(alpha / 2.0f, alpha / 2.0f, 1);
	mat = sMat * rMat * tMat;
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pGrainTex, 0, 0, &rect, &color);
}

void C_ParticleManager::DrawPoison2(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;
	float alpha = 0;


	alpha = particle.alpha / 1.5;
	rect = { 0, 0, particle.size, particle.size };
	color = { 0.27f, 0.50f, 0.00f, alpha };

	sMat = Math::Matrix::CreateScale(alpha * 2, alpha * 2, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pSmokeTex, 0, 0, &rect, &color);
}

const S_Config* C_ParticleManager::GetConfig(ParticleType a_type)
{
	for (auto& config : m_config)
	{
		if (config.type == a_type)
		{
			return &config;
		}
	}
	return nullptr;
}

int C_ParticleManager::GetUnderChip(Math::Vector2 a_pos)
{
	const auto underChip = m_pMap->GetIndex(a_pos.x + 32, a_pos.y - 92);
	if (underChip.second < MAP_HEIGHT && underChip.first < MAP_WIDTH && underChip.second >= 0 && underChip.first >= 0)
	{
		return m_pMap->GetData(underChip.second, underChip.first);
	}
	return 0;
}

bool C_ParticleManager::IsPlaying(ParticleType a_type)
{
	// 再生中のエミッターがあるかどうかを確認する
	return std::any_of(m_emitter.begin(), m_emitter.end(),
		[a_type](S_Emitter m_emitter)
		{return m_emitter.type == a_type && m_emitter.IsActive == true && m_emitter.type;});
}

float C_ParticleManager::Rnd()
{
	return rand() / static_cast<float>(RAND_MAX);
}

void C_ParticleManager::EraseParticle(std::vector<S_Particle>& a_grain)
{
	a_grain.erase(std::remove_if(a_grain.begin(), a_grain.end(),
		[](const S_Particle& a_grain)
		{
			return a_grain.lifeSpan <= 0.0f;
		}), a_grain.end());
}
