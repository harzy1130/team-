#include "DamageUI.h"

void C_DamageUI::Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera)
{
	std::string path = "Src/UI/Texture/DamageUI/";
	m_criticalTex.Load(path+"critical.png");
	m_numberTex = a_pNumberTex;
	m_numberFrameTex = a_pNumberTex;
	m_pCamera = a_pCamera;
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].Init(m_pCamera);
		m_damageUIs[i].SetNumberTexture(m_numberTex);
		m_damageUIs[i].SetFrameTexture(m_numberFrameTex);
		m_criticalFlg[i] = false;
		m_criticalUiPos[i] = { 0,0 };
	}
}

void C_DamageUI::Update()
{
	//if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
	//	//CreateDamageUI(428, { 0,0 }, true, false);
	//	CreateEnemyDamageUI(428, { 0,0 }, true, false);
	//}
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].MoveDamage();
		m_damageUIs[i].Update();

		m_criticalUiPos[i] = m_damageUIs[i].GetDrawPos();

		if (!m_criticalFlg[i])continue;
		m_color[i] = { m_criticalColor.R(), m_criticalColor.G(), m_criticalColor.B(), m_damageUIs[i].GetAlpha() };

		m_criticalUiMat[i] = Math::Matrix::CreateTranslation(
			m_criticalUiPos[i].x-35, m_criticalUiPos[i].y, 0);
	}
}

void C_DamageUI::Draw()
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].Draw();
		
		if(m_damageUIs[i].GetDrawFlg() && m_criticalFlg[i])
		{
			// クリティカルUIの場合の描画
			SHADER.m_spriteShader.SetMatrix(m_criticalUiMat[i]);
			Math::Rectangle srcRect = { 0,0,37,34 };	//テクスチャ座標
			SHADER.m_spriteShader.DrawTex(&m_criticalTex, 0, 0, &srcRect, &m_color[i]);
		}
	}
}

void C_DamageUI::Release()
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		m_damageUIs[i].Release();
	}
	m_criticalTex.Release();
}

void C_DamageUI::CreateDamageUI(float a_Damage, Math::Vector2 a_Pos,bool a_movingDirectionX , bool a_critical)
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())
		{
			if (a_critical)
			{
				// クリティカルの場合の処理
				m_criticalFlg[i] = true;
				m_damageUIs[i].Generate(a_Damage, a_Pos, { 5.0f, 4.0f }, { 1.0f, 1.0f }, 0.0f, m_criticalColor, m_frameColor, 40, a_movingDirectionX);
			}
			else
			{
				// 通常のダメージUIの生成
				m_criticalFlg[i] = false;
				m_damageUIs[i].Generate(a_Damage, a_Pos, { 5.0f, 4.0f }, { 0.7f, 0.7f }, 0.0f,m_dmgColor, m_frameColor, 30, a_movingDirectionX);
			}
			break;
		}
	}
}

// 敵のダメージUI生成
void C_DamageUI::CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical)
{
	for (int i = 0; i < m_maxDamageUI; i++)
	{
		if (!m_damageUIs[i].GetDrawFlg())
		{
			// 通常のダメージUIの生成(敵)
			m_criticalFlg[i] = false;
			m_damageUIs[i].Generate(a_Damage, a_Pos, { 5.0f, 4.0f }, { 0.7f, 0.7f }, 0.0f,m_enemyDmgColor,m_frameColor, 30, a_movingDirectionX);
			break;
		}
	}
}
