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

	m_growTex.Load(path + "Grow.png");

	m_renderTex.CreateRenderTarget(1280, 720);
}

void C_PlayerUI::Update()
{
	UpdateGrowEffect();
	UpdatePlayerHpGauge();
}

void C_PlayerUI::Draw()
{
	DrawGrowEffect();
	DrawPlayerHpGauge();
	DrawPlayerSkill();
	DrawPlayerGear(m_pPlayer->GetGearA(), m_gearAPos);
	DrawPlayerGear(m_pPlayer->GetGearB(), m_gearBPos);
}

void C_PlayerUI::DrawPlayerHpGauge()
{
	Math::Rectangle gaugeRect = CreateRectFromTex(&m_playerHpGaugeTex);
	Math::Rectangle headRect = CreateRectFromTex(&m_playerHpHeadTex);
	Math::Rectangle tailRect = CreateRectFromTex(&m_playerHpTailTex);
	//ゲージの長さを計算
	float healRatio = bufHeal / m_pPlayer->GetMAXHP();
	float dmgRatio = bufDmg / m_pPlayer->GetMAXHP();
	float gaugeWidth;
	float gaugeX;
	Math::Vector2 pos;

	//下地
	DrawTexFromTex(&m_playerHpBaseTex, m_hpGaugePos);

	Math::Color gaugeColor;
	//ダメージバッファ描画
	//このゲージは回復には敏感に反応し、ダメージには鈍感に反応する
	gaugeWidth = gaugeRect.width * dmgRatio + tailRect.width;
	gaugeX = -gaugeRect.width / 2.0f + gaugeWidth / 2.0f - tailRect.width;
	pos = { m_hpGaugePos.x - gaugeRect.width / 2.0f + gaugeWidth + headRect.width / 2.0f - tailRect.width, m_hpGaugePos.y };
	gaugeColor = bufDmg > m_pPlayer->GetHP() ? Math::Color{ 0.7,0,0,1 } : Math::Color{ 0.5, 1.7,0.5,1 };
	DrawTexFromTex(&m_playerHpHeadTex, pos, { 1.0f, 1.0f }, 0.0f, gaugeColor);
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_hpGaugePos.x + gaugeX, m_hpGaugePos.y, 0.0f));
	//1ドットずれてしまうので+1
	SHADER.m_spriteShader.DrawTex(&m_playerHpGaugeTex, 0, 0, &Math::Rectangle(0, 0, gaugeWidth + 1, gaugeRect.height), &gaugeColor);

	//回復バッファ描画
	//このゲージはダメージには敏感に反応し、回復には鈍感に反応する
	if (m_pPlayer->GetHP() <= 0)goto DrawGaugeEnd;
	gaugeWidth = gaugeRect.width * healRatio;
	gaugeX = -gaugeRect.width / 2.0f + gaugeWidth / 2.0f;
	//色調整
	gaugeColor = { Clamp((1 - healRatio) * 2, 1, 0), Clamp(healRatio * 2, 1, 0), 0, 1 };

	//ゲージの描画
	pos = { m_hpGaugePos.x - gaugeRect.width / 2.0f - tailRect.width / 2.0f, m_hpGaugePos.y };
	DrawTexFromTex(&m_playerHpTailTex, pos, { 1.0f, 1.0f }, 0.0f, gaugeColor);
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::CreateTranslation(m_hpGaugePos.x + gaugeX, m_hpGaugePos.y, 0.0f));
	//1ドットずれてしまうので+1
	SHADER.m_spriteShader.DrawTex(&m_playerHpGaugeTex, 0, 0, &Math::Rectangle(0, 0, gaugeWidth + 1, gaugeRect.height), &gaugeColor);
	pos = { m_hpGaugePos.x - gaugeRect.width / 2.0f + gaugeWidth + headRect.width / 2.0f, m_hpGaugePos.y };
	DrawTexFromTex(&m_playerHpHeadTex, pos, { 1.0f, 1.0f }, 0.0f, gaugeColor);
DrawGaugeEnd:;

	//枠の描画
	DrawTexFromTex(&m_playerHpFrameTex, m_hpGaugePos, { 1.0f, 1.0f }, 0.0f);
}

void C_PlayerUI::UpdatePlayerHpGauge()
{
	//HPゲージの更新
	if (bufHeal == bufDmg && bufDmg == m_pPlayer->GetHP())return;
		bufDmg = max((bufDmg - m_pPlayer->GetHP()) * 0.9f, 0.0f) + float(m_pPlayer->GetHP());
		bufHeal = min((bufHeal - m_pPlayer->GetHP()) * 0.9f, 0.0f) + float(m_pPlayer->GetHP());
}

void C_PlayerUI::DrawPlayerSkill()
{
	//クールダウンゲージのサイズを計算
	float skillCDGauge = m_pPlayer->GetSkillCD() / m_pPlayer->GetSkillCalcCD();
	//下地を描画
	DrawTexFromTex(&m_playerSkillBaseTex, m_skillGaugePos);
	//スキルアイコンの色をアクティブかどうかで変える
	Math::Color CDColor = skillCDGauge > 0 ? INVALID_COLOR : VALID_COLOR;
	DrawTexFromTex(m_pPlayerSkillIconTex, m_skillGaugePos, { 1.0f, 1.0f }, 0.0f, CDColor);

	Math::Rectangle cdRect = CreateRectFromTex(&m_playerSkillCDTex);
	Math::Matrix mat = CreateMat({ m_skillGaugePos.x, m_skillGaugePos.y - (cdRect.height * (1 - skillCDGauge) / 2.0f) }, { 1,1 }, 180.0f);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(&m_playerSkillCDTex, 0, 0, &Math::Rectangle(0, 0, cdRect.width, cdRect.height * skillCDGauge), &Math::Color(1.0f, 1.0f, 1.0f, 0.4f));
	DrawTexFromTex(&m_playerSkillFrameTex, m_skillGaugePos);
}

void C_PlayerUI::DrawPlayerGear(C_GearBase* a_pGear, Math::Vector2 a_pos)
{
	//クールダウンとアクティブゲージのサイズを計算
	float gearCDGaugeRatio;
	Math::Color CDIconColor;
	float fanRatio;
	//ギアがパッシブ専用かどうかでゲージの計算を変える
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
	
	//枠のの色を計算（加算合成で色が飛ばないように）
	Math::Color frameColor = fanRatio > 0 ? INVALID_COLOR : VALID_COLOR;
	//ギアの発光エフェクトの制御
	int address = reinterpret_cast<std::uintptr_t>(a_pGear);

	auto itr = m_grow.find(address);
	if (fanRatio > 0)
	{
		//ギアのアドレスを取得して、発光エフェクトのコンテナに登録されているか確認する。
		//されていなければ登録する。されていれば、発光エフェクトの目標サイズを最大にする。	
		if (itr == m_grow.end()) AddGrowEffect(address, a_pos);
		else itr->second.destSize = 1.0f;
	}
	else
	{
		//ギアのアドレスが既に登録されていて、アクティブタイマーの描画がないなら目標サイズを0にして、発光エフェクトを消す
		if (itr != m_grow.end())itr->second.destSize = 0.0f;
	}
	//下地の描画
	DrawTexFromTex(&m_playerSkillBaseTex, a_pos);
	//プレイヤーからもらったギアのアイコンを描画
	DrawTexFromTex(a_pGear->GetGearIconTex(), a_pos, { 1.0f, 1.0f }, 0.0f, CDIconColor);
	//クールダウンゲージの描画
	Math::Rectangle cdRect = CreateRectFromTex(&m_playerSkillCDTex);
	Math::Matrix mat;
	mat = CreateMat({ a_pos.x, a_pos.y - (cdRect.height * (1 - gearCDGaugeRatio) / 2.0f) }, { 1,1 }, 180.0f);
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(&m_playerSkillCDTex, 0, 0, &Math::Rectangle(0, 0, cdRect.width, cdRect.height * gearCDGaugeRatio), &Math::Color(1.0f, 1.0f, 1.0f, 0.4f));
	//枠線の描画
	DrawTexFromTex(&m_playerSkillFrameTex, a_pos, { 1.0f, 1.0f }, 0.0f, frameColor);
	//画像の扇形切り取りでアクティブタイマーを表現し、レンダーテクスチャに描画する
	CreateFanTex(&m_playerSkillGaugeTex, &m_renderTex, CreateRectFromTex(&m_playerSkillGaugeTex), 360 * fanRatio);
	//レンダーテクスチャを加算合成で描画する
	D3D.SetBackBuffer();
	D3D.SetBlendState(BlendMode::Add);
	DrawTexFromTex(&m_renderTex, a_pos, { 1.0f, 1.0f }, 0.0f, Math::Color{ 1.0f, 0.5f, 0.0f, 1.0f });
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_PlayerUI::DrawGrowEffect()
{
	//発光エフェクトの描画
	D3D.SetBlendState(BlendMode::Add);
	for (auto& g : m_grow)
	{
		for (int i = 0;i < lightNum;i++)
		{
			//各光源の描画
			DrawTexFromTex(&m_growTex, g.second.pos, g.second.light[i].scale * g.second.size, g.second.light[i].deg, g.second.light[i].color);
		}
	}
	D3D.SetBlendState(BlendMode::Alpha);
}

void C_PlayerUI::UpdateGrowEffect()
{
	//発光エフェクトの更新
	for (auto itr = m_grow.begin(); itr != m_grow.end();)
	{
		//発光エフェクトのサイズを目標サイズに近づける
		itr->second.size = itr->second.destSize + (itr->second.size - itr->second.destSize) * 0.98f;
		if (itr->second.size < 0.5)
		{
			//発光エフェクトのサイズが小さくなったら削除
			itr = m_grow.erase(itr);
			continue;
		}
		for (int i = 0;i < lightNum;i++)
		{
			//各光源の角度を更新
			if (i < lightNum / 2)itr->second.light[i].deg += 1.0f;
			else itr->second.light[i].deg -= i == lightNum - 1 ? 1.0f : 2.0f;
			if (itr->second.light[i].deg > 360.0f)itr->second.light[i].deg -= 360.0f;
			itr->second.light[i].scale.x = 1.0f + sinf(ToRadians(itr->second.light[i].deg)) * 0.2f;
			itr->second.light[i].scale.y = 0.8f;
		}
		//ループ内でコンテナを削除するため、イテレータを進めるのは最後に行う
		++itr;
	}
}

void C_PlayerUI::AddGrowEffect(int a_addres,Math::Vector2 a_pos)
{
	//発光エフェクトを追加する
	//初期化をしたgrowを作成し、コンテナに追加する
	S_Grow grow;
	grow.pos = a_pos;
	grow.size = 0.7f;
	grow.destSize = 1.0f;
	for (int i = 0;i < lightNum;i++)
	{
		grow.light[i].deg = (360.0f / lightNum) * i;
		grow.light[i].scale = { 1.0f, 0.2f };
		grow.light[i].color = Math::Color{1.0f, 0.5f, 0.0f, 1.0f};
	}
	m_grow.emplace(a_addres, grow);
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
	m_growTex.Release();
	m_renderTex.Release();
}