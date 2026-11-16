#include "RecoveryUI.h"

void C_RecoveryUI::Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera)
{
	std::string path = "Src/UI/Texture/RecoveryUI/";
	m_recoveryTex.Load(path + "Recovery2.png");
	m_numberTex = a_pNumberTex;
	m_numberFrameTex = a_pFrameTex;
	m_pCamera = a_pCamera;
	for (int i = 0; i < m_maxRecoveryUI; i++)
	{
		m_recoveryUIs[i].Init(m_pCamera);
		m_recoveryUIs[i].SetNumberTexture(m_numberTex);
		m_recoveryUIs[i].SetFrameTexture(m_numberFrameTex);
		m_recoveryUiPos[i] = { 0,0 };
	}
}

void C_RecoveryUI::Update()
{
	//if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
	//	CreateRecoveryUI(50, { 0,0 });
	//}

	for (int i = 0; i < m_maxRecoveryUI; i++)
	{
		m_recoveryUIs[i].MoveRecovery();
		m_recoveryUIs[i].Update();
		m_recoveryUiPos[i] = m_recoveryUIs[i].GetDrawPos();

		m_color[i] = { m_0Color.R(), m_0Color.G(), m_0Color.B(), m_recoveryUIs[i].GetAlpha() };
		m_recoveryUiSize[i] = m_recoveryUIs[i].GetSize();

		m_transMat = Math::Matrix::CreateTranslation(
			m_recoveryUiPos[i].x - 30* m_recoveryUiSize[i].x, m_recoveryUiPos[i].y, 0);
		m_scaleMat = Math::Matrix::CreateScale(m_recoveryUiSize[i].x*0.9f, m_recoveryUiSize[i].y*0.9f, 1.0f);
		m_recoveryUiMat[i] = m_scaleMat * m_transMat;
	}
}

void C_RecoveryUI::Draw()
{
	for (int i = 0; i < m_maxRecoveryUI; i++)
	{
		if (!m_recoveryUIs[i].GetDrawFlg())continue;
		m_recoveryUIs[i].Draw();

		// 回復UIの描画
		SHADER.m_spriteShader.SetMatrix(m_recoveryUiMat[i]);
		Math::Rectangle srcRect = { 0,0,36,36 };//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(&m_recoveryTex, 0, 0, &srcRect, &m_color[i]);

	}
}

void C_RecoveryUI::Release()
{
	m_recoveryTex.Release();
}

void C_RecoveryUI::CreateRecoveryUI(float a_Recovery, Math::Vector2 a_Pos)
{
	for (int i = 0; i < m_maxRecoveryUI; i++)
	{
		if (!m_recoveryUIs[i].GetDrawFlg())
		{
			m_recoveryUIs[i].Generate(a_Recovery, a_Pos, m_moveSpeed, m_UIsize, 0.0f, m_recoveryColor, m_frameColor, m_UiTime, false);
			return;
		}
	}	
}
