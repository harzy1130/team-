#include "PlayerUI.h"

void C_PlayerUI::Init(C_PlayerBase* a_pPlayer)
{
	std::string path = "Src/UI/Texture/PlayerUI/";
	m_pPlayer = a_pPlayer;
	m_playerHpBaseTex.Load(path + "PlayerHpBase.png");
	m_playerHpFrameTex.Load(path + "PlayerHpFrame.png");
	m_playerHpGaugeTex.Load(path + "PlayerHpGauge.png");
	m_playerHpTailTex.Load(path + "PlayerHpTail.png");
	m_playerHpHeadTex.Load(path + "PlayerHpHead.png");

	m_playerSkillFrameTex.Load(path + "PlayerSkillFrame.png");
	m_playerSkillBaseTex.Load(path + "PlayerSkillBase.png");
	m_playerSkillCDTex.Load(path + "PlayerSkillCD.png");
	m_playerSkillGaugeTex.Load(path + "PlayerSkillGauge.png");
	m_pPlayerSkillIconTex = a_pPlayer->GetSkillIconTex();

	m_renderTex.CreateRenderTarget(1280, 720);
}

void C_PlayerUI::Update()
{
}

void C_PlayerUI::Draw()
{
	DrawPlayerHpGauge();
	DrawPlayerSkill();
	DrawPlayerGear(m_pPlayer->GetGearA(), { -100, -275 });
	DrawPlayerGear(m_pPlayer->GetGearB(), { 100, -275 });
}

void C_PlayerUI::DrawPlayerHpGauge()
{
	Math::Rectangle gaugeRect = CreateRectFromTex(&m_playerHpGaugeTex);
	Math::Rectangle headRect = CreateRectFromTex(&m_playerHpHeadTex);
	Math::Rectangle tailRect = CreateRectFromTex(&m_playerHpTailTex);
	float hpRatio = float(m_pPlayer->GetHP()) / float(m_pPlayer->GetMAXHP());
	float gaugeWidth = gaugeRect.width * hpRatio;
	float gaugeX = -gaugeRect.width / 2.0f + gaugeWidth / 2.0f;
	Math::Vector2 pos;
	//‰º’n
	pos = m_hpGaugePos;
	DrawTexFromTex(&m_playerHpBaseTex, pos);
	//F’²®
	Math::Color gaugeColor = { Clamp((1 - hpRatio) * 2, 1, 0), Clamp(hpRatio * 2, 1, 0), 0, 1 };
	//ƒQ[ƒW‚Ì•`‰æ
	if (m_pPlayer->GetHP() <= 0)goto DrawGaugeEnd;
	pos = { m_hpGaugePos.x - gaugeRect.width / 2.0f - tailRect.width / 2.0f, m_hpGaugePos.y };
	DrawTexFromTex(&m_playerHpTailTex, pos, { 1.0f, 1.0f }, 0.0f, gaugeColor);
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_hpGaugePos.x + gaugeX, m_hpGaugePos.y, 0.0f));
	//1ƒhƒbƒg‚¸‚ê‚Ä‚µ‚Ü‚¤‚Ì‚Å+
	SHADER.m_spriteShader.DrawTex(&m_playerHpGaugeTex, 0, 0, &Math::Rectangle(0, 0, gaugeWidth + 1, gaugeRect.height), &gaugeColor);
	pos = { m_hpGaugePos.x - gaugeRect.width / 2.0f + gaugeWidth + headRect.width / 2.0f, m_hpGaugePos.y };
	DrawTexFromTex(&m_playerHpHeadTex, pos, { 1.0f, 1.0f }, 0.0f, gaugeColor);
DrawGaugeEnd:;
	//˜g‚Ì•`‰æ
	DrawTexFromTex(&m_playerHpFrameTex, { m_hpGaugePos.x, m_hpGaugePos.y }, { 1.0f, 1.0f }, 0.0f);
}

void C_PlayerUI::DrawPlayerSkill()
{
	float skillUIRatio = m_pPlayer->GetSkillCD() / m_pPlayer->GetSkillCalcCD();

	DrawTexFromTex(&m_playerSkillBaseTex, m_skillGaugePos);
	Math::Color CDColor = skillUIRatio > 0 ? Math::Color{ 0.4, 0.4, 0.4, 1.0f } : Math::Color{ 1.0f, 1.0f, 1.0f, 1.0f };
	DrawTexFromTex(m_pPlayerSkillIconTex, m_skillGaugePos, { 1.0f, 1.0f }, 0.0f, CDColor);
	Math::Rectangle cdRect = CreateRectFromTex(&m_playerSkillCDTex);
	Math::Matrix mat, sMat, rMat, tMat;
	sMat = Math::Matrix::CreateScale(1,1,1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(180.0f));
	tMat = Math::Matrix::CreateTranslation(m_skillGaugePos.x, m_skillGaugePos.y - (cdRect.height * (1 - skillUIRatio) / 2.0f), 0);
	mat = sMat * rMat * tMat;
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(&m_playerSkillCDTex, 0, 0, &Math::Rectangle(0, 0, cdRect.width, cdRect.height * skillUIRatio), &Math::Color(1.0f, 1.0f, 1.0f, 0.4f));
	DrawTexFromTex(&m_playerSkillFrameTex, m_skillGaugePos);
}

void C_PlayerUI::DrawPlayerGear(C_GearBase* a_pGear,Math::Vector2 a_pos)
{
	float gearCDGaugeRatio;
	Math::Color CDIconColor;
	float fanRatio;
	if (a_pGear->GetPassiveOnly()) 
	{
		gearCDGaugeRatio = a_pGear->GetPassiveCD() / a_pGear->GetCalcPassiveCDMax();
		if (!a_pGear->GetHavePassiveCD())gearCDGaugeRatio = 0;
		CDIconColor = a_pGear->GetPassiveCD() == 0 ? VALID_COLOR : INVALID_COLOR;
		if (!a_pGear->GetHavePassiveCD())CDIconColor = VALID_COLOR;
		fanRatio = a_pGear->GetPassiveTimer() / a_pGear->GetPassiveTimerMax();
		if (a_pGear->GetPassiveTimerMax() <= 0 && a_pGear->GetWhilePassive())fanRatio = 1;
	}
	else
	{
		gearCDGaugeRatio = a_pGear->GetActiveCD() / a_pGear->GetCalcActiveCDMax();
		CDIconColor = a_pGear->GetActiveCD() > 0 || a_pGear->GetWhileActive() ? INVALID_COLOR : VALID_COLOR;
		if (!a_pGear->GetWaitActiveTimer())CDIconColor = a_pGear->GetActiveCD() == 0 ? VALID_COLOR : INVALID_COLOR;
		fanRatio = a_pGear->GetActiveTimer() / a_pGear->GetActiveTimerMax();
	}

	Math::Color frameColor = fanRatio > 0 ? INVALID_COLOR : VALID_COLOR;

	DrawTexFromTex(&m_playerSkillBaseTex,a_pos);
	DrawTexFromTex(a_pGear->GetGearIconTex(), a_pos, { 1.0f, 1.0f }, 0.0f, CDIconColor);
	Math::Rectangle cdRect = CreateRectFromTex(&m_playerSkillCDTex);
	Math::Matrix mat;
	mat = CreateMat({ a_pos.x, a_pos.y - (cdRect.height * (1 - gearCDGaugeRatio) / 2.0f) }, {1,1}, 180.0f);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(&m_playerSkillCDTex, 0, 0, &Math::Rectangle(0, 0, cdRect.width, cdRect.height * gearCDGaugeRatio), &Math::Color(1.0f, 1.0f, 1.0f, 0.4f));
	

	DrawTexFromTex(&m_playerSkillFrameTex,a_pos, { 1.0f, 1.0f }, 0.0f, frameColor);
	CreateFanTex(&m_playerSkillGaugeTex, &m_renderTex, CreateRectFromTex(&m_playerSkillGaugeTex), 360 * fanRatio);
	D3D.SetBackBuffer();
	D3D.SetBlendState(BlendMode::Add);
	DrawTexFromTex(&m_renderTex, a_pos, { 1.0f, 1.0f }, 0.0f, Math::Color{ 1.0f, 0.5f, 0.0f, 1.0f });

	D3D.SetBlendState(BlendMode::Alpha);
}

void C_PlayerUI::Release()
{
	m_playerHpBaseTex.Release();
	m_playerHpFrameTex.Release();
	m_playerHpGaugeTex.Release();
	m_playerHpHeadTex.Release();
	m_playerHpTailTex.Release();

	m_playerSkillFrameTex.Release();
	m_playerSkillBaseTex.Release();
	m_playerSkillGaugeTex.Release();
	m_renderTex.Release();
}
