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
	m_pPlayerSkillIconTex = a_pPlayer->GetSkillIconTex();
}

void C_PlayerUI::Update()
{
}

void C_PlayerUI::Draw()
{
	DrawPlayerHpGauge();
	DrawPlayerSkill();
	DrawPlayerGear();
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

void C_PlayerUI::DrawPlayerGear()
{
	auto gearA = m_pPlayer->GetGearA();
	DrawTexFromTex(&m_playerSkillBaseTex,{-100, -275});
	DrawTexFromTex(gearA->GetGearIconTex(), { -100, -275 });
	DrawTexFromTex(&m_playerSkillFrameTex,{-100, -275});


	auto gearB = m_pPlayer->GetGearB();
	DrawTexFromTex(&m_playerSkillBaseTex, { 100, -275 });
	DrawTexFromTex(gearB->GetGearIconTex(), { 100, -275 });
	DrawTexFromTex(&m_playerSkillFrameTex, { 100, -275 });
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
}
