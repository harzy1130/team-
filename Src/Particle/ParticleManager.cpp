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
	DUST.move = { 0,0 };
	DUST.randomOffsetX = { 10,-10 };
	DUST.randomOffsetY = { 0,0 };
	DUST.randomOffsetMoveX = { 1.0,-0.5 };
	DUST.randomOffsetMoveY = { 1,0 };
	DUST.size = 9;
	DUST.angle = 3;
	DUST.alpha = 0;
	DUST.lifeSpan = 30;
	DUST.IsRepeat = true;
	DUST.generateNum = 1;
	DUST.generateTime = 0;
	DUST.generateInterval = 3;
	DUST.IsFollowPlayer = true;
	m_config.push_back(DUST);

	S_Config ATKUP;
	ATKUP.type = ParticleType::atkUp;
	ATKUP.move = { m_pPlayer->GetMove().x,3 };
	ATKUP.randomOffsetX = { 32,-32 };
	ATKUP.randomOffsetY = { 10,-30 };
	ATKUP.randomOffsetMoveX = { 0,0 };
	ATKUP.randomOffsetMoveY = { 0,0 };
	ATKUP.size = 32;
	ATKUP.angle = 0;
	ATKUP.alpha = 0;
	ATKUP.lifeSpan = 45;
	ATKUP.IsRepeat = true;
	ATKUP.generateNum = 1;
	ATKUP.generateTime = 50;
	ATKUP.generateInterval = 5;
	ATKUP.IsFollowPlayer = true;
	m_config.push_back(ATKUP);

	S_Config ATKUP2;
	ATKUP2.type = ParticleType::atkUp2;
	ATKUP2.move = { 0,0.3 };
	ATKUP2.randomOffsetX = { 0,0 };
	ATKUP2.randomOffsetY = { 0,0 };
	ATKUP2.randomOffsetMoveX = { 0,0 };
	ATKUP2.randomOffsetMoveY = { 0,0 };
	ATKUP2.size = 32;
	ATKUP2.angle = 0;
	ATKUP2.alpha = 0;
	ATKUP2.lifeSpan = 60;
	ATKUP2.IsRepeat = false;
	ATKUP2.generateNum = 1;
	ATKUP2.generateTime = 1;
	ATKUP2.generateInterval = 0;
	ATKUP2.IsFollowPlayer = true;
	m_config.push_back(ATKUP2);

	S_Config DEFUP;
	DEFUP.type = ParticleType::defUp1;
	DEFUP.move = { m_pPlayer->GetMove().x,3 };
	DEFUP.randomOffsetX = { 32,-32 };
	DEFUP.randomOffsetY = { 10,-30 };
	DEFUP.randomOffsetMoveX = { 0,0 };
	DEFUP.randomOffsetMoveY = { 0,0 };
	DEFUP.size = 32;
	DEFUP.angle = 0;
	DEFUP.alpha = 0;
	DEFUP.lifeSpan = 45;
	DEFUP.IsRepeat = true;
	DEFUP.generateNum = 1;
	DEFUP.generateTime = 60;
	DEFUP.generateInterval = 5;
	DEFUP.IsFollowPlayer = true;
	m_config.push_back(DEFUP);

	S_Config DEFUP2;
	DEFUP2.type = ParticleType::defUp2;
	DEFUP2.move = { m_pPlayer->GetMove().x,3 };
	DEFUP2.randomOffsetX = { 32,-32 };
	DEFUP2.randomOffsetY = { 10,-30 };
	DEFUP2.randomOffsetMoveX = { 0,0 };
	DEFUP2.randomOffsetMoveY = { 0,0 };
	DEFUP2.size = 32;
	DEFUP2.angle = 5;
	DEFUP2.alpha = 0;
	DEFUP2.lifeSpan = 45;
	DEFUP2.IsRepeat = true;
	DEFUP2.generateNum = 1;
	DEFUP2.generateTime = 60;
	DEFUP2.generateInterval = 5;
	DEFUP2.IsFollowPlayer = true;
	m_config.push_back(DEFUP2);

	S_Config POISON;
	POISON.type = ParticleType::poison;
	POISON.move = { m_pPlayer->GetMove().x,0 };
	POISON.randomOffsetX = { 24,-24 };
	POISON.randomOffsetY = { -20,-30 };
	POISON.randomOffsetMoveX = { 0,0 };
	POISON.randomOffsetMoveY = { 2,1 };
	POISON.size = 16;
	POISON.angle = 10;
	POISON.alpha = 0;
	POISON.lifeSpan = 60;
	POISON.IsRepeat = true;
	POISON.generateNum = 2;
	POISON.generateTime = 60;
	POISON.generateInterval = 20;
	POISON.IsFollowPlayer = true;
	m_config.push_back(POISON);

	S_Config POISON2;
	POISON2.type = ParticleType::poison2;
	POISON2.move = { 0,0 };
	POISON2.randomOffsetX = { 24,-24 };
	POISON2.randomOffsetY = { 5,-20 };
	POISON2.randomOffsetMoveX = { 2.0f,-1.0f };
	POISON2.randomOffsetMoveY = { 1.5,0 };
	POISON2.size = 64;
	POISON2.angle = 3;
	POISON2.alpha = 0;
	POISON2.lifeSpan = 45;
	POISON2.IsRepeat = false;
	POISON2.generateNum = 4;
	POISON2.generateTime = 0;
	POISON2.generateInterval = 0;
	POISON2.IsFollowPlayer = true;
	m_config.push_back(POISON2);

	S_Config HEAL;
	HEAL.type = ParticleType::heal;
	HEAL.move = { 0,2 };
	HEAL.randomOffsetX = { 10,-10 };
	HEAL.randomOffsetY = { -20,-40 };
	HEAL.randomOffsetMoveX = { 0,0 };
	HEAL.randomOffsetMoveY = { 0,0 };
	HEAL.size = 32;
	HEAL.angle = 10;
	HEAL.alpha = 0;
	HEAL.lifeSpan = 60;
	HEAL.IsRepeat = true;
	HEAL.generateNum = 1;
	HEAL.generateTime = 45;
	HEAL.generateInterval = 5;
	HEAL.IsFollowPlayer = true;
	m_config.push_back(HEAL);
	S_Config HEAL2;
	HEAL2.type = ParticleType::heal2;
	HEAL2.move = { 0,0 };
	HEAL2.randomOffsetX = { 0,0 };
	HEAL2.randomOffsetY = { 0,0 };
	HEAL2.randomOffsetMoveX = { 0,0 };
	HEAL2.randomOffsetMoveY = { 0,0 };
	HEAL2.size = 32;
	HEAL2.angle = 0;
	HEAL2.alpha = 1.0;
	HEAL2.lifeSpan = 90;
	HEAL2.IsRepeat = false;
	HEAL2.generateNum = 1;
	HEAL2.generateTime = 0;
	HEAL2.generateInterval = 0;
	HEAL2.IsFollowPlayer = true;
	m_config.push_back(HEAL2);

	S_Config BURN;
	BURN.type = ParticleType::burn;
	BURN.move = { m_pPlayer->GetMove().x,2 };
	BURN.randomOffsetX = { 20,-20 };
	BURN.randomOffsetY = { 0,-40 };
	BURN.randomOffsetMoveX = { 0,0 };
	BURN.randomOffsetMoveY = { 0,0 };
	BURN.size = 16;
	BURN.angle = 5;
	BURN.alpha = 0;
	BURN.lifeSpan = 60;
	BURN.IsRepeat = true;
	BURN.generateNum = 1;
	BURN.generateTime = 60;
	BURN.generateInterval = 0;
	BURN.IsFollowPlayer = true;
	m_config.push_back(BURN);

	S_Config ATKDOWN;
	ATKDOWN.type = ParticleType::atkDown;
	ATKDOWN.move = { m_pPlayer->GetMove().x,-3 };
	ATKDOWN.randomOffsetX = { 32,-32 };
	ATKDOWN.randomOffsetY = { 30,-10 };
	ATKDOWN.randomOffsetMoveX = { 0,0 };
	ATKDOWN.randomOffsetMoveY = { 0,0 };
	ATKDOWN.size = 32;
	ATKDOWN.angle = 0;
	ATKDOWN.alpha = 0;
	ATKDOWN.lifeSpan = 45;
	ATKDOWN.IsRepeat = true;
	ATKDOWN.generateNum = 1;
	ATKDOWN.generateTime = 50;
	ATKDOWN.generateInterval = 5;
	ATKDOWN.IsFollowPlayer = true;
	m_config.push_back(ATKDOWN);

	S_Config ATKDOWN2;
	ATKDOWN2.type = ParticleType::atkDown2;
	ATKDOWN2.move = { 0,-0.3 };
	ATKDOWN2.randomOffsetX = { 0,0 };
	ATKDOWN2.randomOffsetY = { 0,0 };
	ATKDOWN2.randomOffsetMoveX = { 0,0 };
	ATKDOWN2.randomOffsetMoveY = { 0,0 };
	ATKDOWN2.size = 32;
	ATKDOWN2.angle = 0;
	ATKDOWN2.alpha = 0;
	ATKDOWN2.lifeSpan = 60;
	ATKDOWN2.IsRepeat = false;
	ATKDOWN2.generateNum = 1;
	ATKDOWN2.generateTime = 1;
	ATKDOWN2.generateInterval = 0;
	ATKDOWN2.IsFollowPlayer = true;
	m_config.push_back(ATKDOWN2);
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

void C_ParticleManager::SetTexture(KdTexture* a_pChipTex, KdTexture* a_pGrainTex, KdTexture* a_pAtkUpTex, KdTexture* a_pDefUpTex, KdTexture* a_pSmokeTex, KdTexture* a_pHealTex, KdTexture* a_pBurnTex)
{
	m_pChipTex = a_pChipTex;
	m_pGrainTex = a_pGrainTex;
	m_pAtkUpTex = a_pAtkUpTex;
	m_pDefUpTex = a_pDefUpTex;
	m_pSmokeTex = a_pSmokeTex;
	m_pHealTex = a_pHealTex;
	m_pBurnTex = a_pBurnTex;
}

void C_ParticleManager::UpdatePlayer()
{
	bool jump = m_pPlayer->GetJumpFlg();
	bool moveRight = m_pPlayer->GetMove().x > 0;
	bool moveLeft = m_pPlayer->GetMove().x < 0;
	bool moveStop = m_pPlayer->GetMove().x == 0;
	bool dush = GetAsyncKeyState(VK_SHIFT) & 0x8000;
	bool landing;
	bool AtkUp = m_pPlayer->GetGearA()->GetWhileActive() == true;
	//毒状態かどうかではなく、毒にかかった瞬間が欲しい
	bool poisning = m_pEffect->GetPoison();
	bool poisning2 = GetAsyncKeyState('P') & 0x8000;
	bool DefUp = GetAsyncKeyState('I') & 0x8000;
	bool heal = GetAsyncKeyState('H') & 0x8000;
	bool burn = m_pEffect->GetCombustion();
	bool burn2 = GetAsyncKeyState('C') & 0x8000;
	bool AtkDown = GetAsyncKeyState('O') & 0x8000;


	//着地のときのパーティクル
	UpdateEmitterRequest(ParticleType::dust, (moveLeft || moveRight) && !jump, { m_pPlayer->GetPos().x,m_pPlayer->GetPos().y - 60.0f });
	UpdateEmitterRequest(ParticleType::atkUp, AtkUp, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::atkUp2, AtkUp, m_pPlayer->GetPos());

	UpdateEmitterRequest(ParticleType::defUp1, DefUp, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::defUp2, DefUp, m_pPlayer->GetPos());

	UpdateEmitterRequest(ParticleType::poison, poisning, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::poison2, poisning2, m_pPlayer->GetPos());

	UpdateEmitterRequest(ParticleType::heal, debug, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::heal2, debug, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::burn, burn, m_pPlayer->GetPos());

	UpdateEmitterRequest(ParticleType::atkDown, AtkDown, m_pPlayer->GetPos());
	UpdateEmitterRequest(ParticleType::atkDown2, AtkDown, m_pPlayer->GetPos());

	if (heal)
	{
		debug = true;
	}
	if (debug == true)
	{
		count--;
	}
	if (count <= 0)
	{
		debug = false;
		count = Dtime;
	}

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
		case ParticleType::atkUp2:
			DrawAtk2(p);
			break;
		case ParticleType::defUp1:
			DrawDef(p);
			break;
		case ParticleType::defUp2:
			DrawDef2(p);
			break;
		case ParticleType::poison:
			DrawPoison(p);
			break;
		case ParticleType::poison2:
			DrawPoison2(p);
			break;
		case ParticleType::heal:
			DrawHeal(p);
			break;
		case ParticleType::heal2:
			DrawHeal2(p);
			break;
		case ParticleType::burn:
			DrawBurn(p);
			break;
		case ParticleType::atkDown:
			DrawAtkDown(p);
			break;
		case ParticleType::atkDown2:
			DrawAtkDown2(p);
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
	particle.size = config->size;
	particle.angle = config->angle;
	particle.alpha = config->alpha;
	particle.lifeSpan = config->lifeSpan;
	particle.IsActive = true;
	particle.data = 0;
	particle.alphaCalc = 0;
	particle.InitialPos = particle.pos;

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
	if (emitter.IsRepeat);
	else emitter.IsActive = false;
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
		if (config == nullptr)return;

		if (particle.lifeSpan >= 0)
		{
			if (particle.type == ParticleType::dust)particle.data = GetUnderChip(particle.pos);
			particle.pos += particle.move;
			particle.angle += config->angle;
			particle.size = config->size;
			particle.alpha = particle.lifeSpan / config->lifeSpan;
			particle.scale = particle.lifeSpan / config->lifeSpan;
			particle.lifeSpan--;

			if (particle.type == ParticleType::poison)
			{
				if (particle.angle >= 360)particle.angle = 0;
				particle.pos.x = particle.InitialPos.x + sin(DirectX::XMConvertToRadians(particle.angle)) * 4;
			}
			if (particle.type == ParticleType::burn)
			{
				if (particle.angle >= 360)particle.angle = 0;
				particle.pos.x = particle.InitialPos.x + sin(DirectX::XMConvertToRadians(particle.angle)) * 10;
			}
			if (particle.type == ParticleType::heal)
			{
				if (particle.angle >= 360)particle.angle = 0;
				particle.pos.x = particle.InitialPos.x + sin(DirectX::XMConvertToRadians(particle.angle)) * 40;
			}
			if (particle.type == ParticleType::heal2)
			{
				particle.alpha = 0;
				particle.alphaCalc = 0;
				if (particle.alpha <= 1.0f)particle.alpha = particle.lifeSpan / config->lifeSpan;
			}
		}
	}
	EraseParticle(m_particles);
}

void C_ParticleManager::DrawDust(S_Particle particle)
{
	if (particle.data == 0)return;
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;
	float alpha = 0;
	alpha = particle.alpha / 1.2f;
	rect = { CHIP_SIZE * particle.data, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };
	sMat = Math::Matrix::CreateScale(alpha, alpha, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pChipTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}
void C_ParticleManager::DrawAtk(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
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

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);

}

void C_ParticleManager::DrawAtk2(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 3.0f;

	rect = { 32, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };

	sMat = Math::Matrix::CreateScale(2.5f, 4.0f, 1);
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawDef(S_Particle particle)
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
	color = { 1, 1, 0.7f, alpha };

	sMat = Math::Matrix::CreateScale(alpha, alpha * 2.0f, 1);
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pDefUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawDef2(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Vector2 move = { Rnd() * 2 - 1,3 };
	draw += move;
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 1.2f;

	rect = { 32, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };

	sMat = Math::Matrix::CreateScale(alpha, alpha, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pDefUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawHeal(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	//Math::Vector2 move = { Rnd() * 2 - 1,3 };
	//draw += move;
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;
	float scale = 0;

	alpha = particle.alpha / 1.2f;
	scale = particle.scale;

	rect = { 0, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };

	sMat = Math::Matrix::CreateScale(scale, scale, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	//mat = sMat * rMat * tMat;
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pHealTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawHeal2(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 1.5f;
	rect = { 32, 0, particle.size, particle.size };
	color = { 1, 1, 1, alpha };

	sMat = Math::Matrix::CreateScale(3, 6, 1);
	//rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	//mat = sMat * rMat * tMat;
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pHealTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
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
	float scale = 0;


	alpha = particle.alpha / 1.2f;
	scale = particle.scale;
	rect = { 0, 0, particle.size, particle.size };
	color = { 0.27f, 0.94f, 0.00f, alpha };

	sMat = Math::Matrix::CreateScale(scale, scale, 1);
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
	float scale = 0;


	alpha = particle.alpha / 2.5f;
	scale = particle.scale;
	rect = { 0, 0, particle.size, particle.size };
	color = { 0.27f, 0.50f, 0.00f, alpha };

	sMat = Math::Matrix::CreateScale(scale * 1.5f, scale * 1.5f, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pSmokeTex, 0, 0, &rect, &color);
}

void C_ParticleManager::DrawBurn(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;
	float alpha = 0;
	float scale = 0;


	alpha = particle.alpha / 5.5f;
	scale = particle.scale;
	rect = { 0, 0, particle.size, particle.size };
	color = { 1,1,1, alpha };

	sMat = Math::Matrix::CreateScale(scale * 2.0f, scale * 2.5f, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(particle.angle));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	//mat = sMat * rMat * tMat;
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pBurnTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawAtkDown(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 1.2f;

	rect = { 0, 0, particle.size, particle.size };
	color = { 0.5f,0.6f,0.6f, alpha };

	sMat = Math::Matrix::CreateScale(alpha, alpha * 2.0f, 1);
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_ParticleManager::DrawAtkDown2(S_Particle particle)
{
	Math::Vector2 draw = particle.pos - m_pCamera->GetPos();
	Math::Color color;
	Math::Rectangle rect;
	Math::Matrix sMat;
	Math::Matrix rMat;
	Math::Matrix tMat;
	Math::Matrix mat;

	float alpha = 0;

	alpha = particle.alpha / 3.0f;

	rect = { 32, 0, particle.size, particle.size };
	color = { 0.5f,0.6f,0.6f, alpha };

	sMat = Math::Matrix::CreateScale(2.5f, 4.0f, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180));
	tMat = Math::Matrix::CreateTranslation(draw.x, draw.y, 0);
	mat = sMat * rMat * tMat;

	D3D.SetBlendState(BlendMode::Add);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(m_pAtkUpTex, 0, 0, &rect, &color);
	D3D.SetBlendState(BlendMode::Alpha);
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

//bool C_ParticleManager::GetLandingFlg(C_PlayerBase* player)
//{
//	//bool landing;
//	//if(player->GetJumpFlg() == true)
//
//	//return ;
//}

bool C_ParticleManager::IsPlaying(ParticleType a_type)
{
	// 再生中のエミッターがあるかどうかを確認する
	return std::any_of(m_emitter.begin(), m_emitter.end(),
		[a_type](S_Emitter m_emitter)
		{return m_emitter.type == a_type && m_emitter.IsActive == true;});
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
