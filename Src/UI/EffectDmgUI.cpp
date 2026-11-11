#include "Pch.h"
#include "EffectDmgUI.h"

void C_EffectDmgUI::Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera)
{
	std::string path = "Src/UI/Texture/DamageUI/";
	m_poisonTex.Load(path + "Poison.png");
	m_numberTex = a_pNumberTex;
	m_numberFrameTex = a_pFrameTex;
	m_pCamera = a_pCamera;
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].Init(m_pCamera);
		m_damageUIs[i].SetNumberTexture(m_numberTex);
		m_damageUIs[i].SetFrameTexture(m_numberFrameTex);
		m_poisonUiPos[i] = { 0,0 };
	}
}

void C_EffectDmgUI::Update()
{
	//if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
	//	CreateEffectDamageUI(1, { 0,0 });
	//}
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].MoveEffectDamage();
		m_damageUIs[i].Update();

		m_poisonUiPos[i] = m_damageUIs[i].GetDrawPos();
		m_color[i] = { m_poisonColor.R(), m_poisonColor.G(), m_poisonColor.B(), m_damageUIs[i].GetAlpha() };

		m_transMat = Math::Matrix::CreateTranslation(
			m_poisonUiPos[i].x - 30, m_poisonUiPos[i].y, 0);
		m_scaleMat = Math::Matrix::CreateScale(0.8f, 0.8f, 1.0f);
		m_poisonUiMat[i] = m_scaleMat * m_transMat;
	}
}

void C_EffectDmgUI::Draw()
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())continue;
		m_damageUIs[i].Draw();
		// ポイズンUIの場合の描画
		SHADER.m_spriteShader.SetMatrix(m_poisonUiMat[i]);
		Math::Rectangle srcRect = { 0,0,34,34 };//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_poisonTex, 0, 0, &srcRect, &m_color[i]);

	};
}

void C_EffectDmgUI::Release()
{
	m_poisonTex.Release();
}

void C_EffectDmgUI::CreateEffectDamageUI(float a_Damage, Math::Vector2 a_Pos)
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())
		{
			m_damageUIs[i].Generate(a_Damage, a_Pos, { 5.0f, 4.0f }, { 0.8f, 0.8f }, 0.0f, m_poisonColor, m_frameColor, 30, false);
			break;
		}
	}
}
