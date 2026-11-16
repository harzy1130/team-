#include "Pch.h"
#include "EffectDmgUI.h"
void C_EffectDmgUI::Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera)
{
	std::string path = "Src/UI/Texture/DamageUI/";
	m_poisonTex.Load(path + "Poison.png");
	m_flameTex.Load(path + "Flame.png");
	m_numberTex = a_pNumberTex;
	m_numberFrameTex = a_pFrameTex;
	m_pCamera = a_pCamera;
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].Init(m_pCamera);
		m_damageUIs[i].SetNumberTexture(m_numberTex);
		m_damageUIs[i].SetFrameTexture(m_numberFrameTex);
		m_UiPos[i] = { 0,0 };
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

		m_UiPos[i] = m_damageUIs[i].GetDrawPos();


		m_dmgUiSize[i] = m_damageUIs[i].GetSize();

		if(m_type[i] == E_TYPE_POISON)
		{
			m_color[i] = { m_poisonColor.R(), m_poisonColor.G(), m_poisonColor.B(), m_damageUIs[i].GetAlpha() };
			m_transMat = Math::Matrix::CreateTranslation(
				m_UiPos[i].x - 20*m_dmgUiSize[i].x, m_UiPos[i].y, 0);
			m_scaleMat = Math::Matrix::CreateScale(m_dmgUiSize[i].x*0.9f, m_dmgUiSize[i].y*0.9f, 1.0f);
			m_UiMat[i] = m_scaleMat * m_transMat;
		}
		else if(m_type[i] == E_TYPE_FLAME)
		{
			m_color[i] = { m_0Color.R(), m_0Color.G(), m_0Color.B(), m_damageUIs[i].GetAlpha() };
			m_transMat = Math::Matrix::CreateTranslation(
				m_UiPos[i].x - 20*m_dmgUiSize[i].x, m_UiPos[i].y, 0);
			m_scaleMat = Math::Matrix::CreateScale(m_dmgUiSize[i].x, m_dmgUiSize[i].y, 1.0f);
			m_UiMat[i] = m_scaleMat * m_transMat;
		}
	}

}

void C_EffectDmgUI::Draw()
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())continue;
		m_damageUIs[i].Draw();
		if(m_type[i] == E_TYPE_POISON)
		{
			// ポイズンUIの場合の描画
			SHADER.m_spriteShader.SetMatrix(m_UiMat[i]);
			Math::Rectangle srcRect = { 0,0,34,34 };//テクスチャ座標
			SHADER.m_spriteShader.DrawTex(&m_poisonTex, 0, 0, &srcRect, &m_color[i]);
		}
		else if (m_type[i]==E_TYPE_FLAME)
		{
			// ポイズンUIの場合の描画
			SHADER.m_spriteShader.SetMatrix(m_UiMat[i]);
			Math::Rectangle srcRect = { 0,0,34,34 };//テクスチャ座標
			SHADER.m_spriteShader.DrawTex(&m_flameTex, 0, 0, &srcRect, &m_color[i]);
		}

	};
}

void C_EffectDmgUI::Release()
{
	m_poisonTex.Release();
	m_flameTex.Release();
}

void C_EffectDmgUI::CreatePoisonDamageUI(float a_Damage, Math::Vector2 a_Pos)
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())
		{
			m_damageUIs[i].Generate(a_Damage, a_Pos, m_moveSpeed, m_UIsize, 0.0f, m_poisonColor, m_frameColor, m_UiTime, false);
			m_type[i] = E_TYPE_POISON;
			break;
		}
	}
}

void C_EffectDmgUI::CreateFlameDamageUI(float a_Damage, Math::Vector2 a_Pos)
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())
		{
			m_damageUIs[i].Generate(a_Damage, a_Pos, m_moveSpeed, m_UIsize, 0.0f, m_flameColor, m_frameColor, m_UiTime, false);
			m_type[i] = E_TYPE_FLAME;
			break;
		}
	}
}
