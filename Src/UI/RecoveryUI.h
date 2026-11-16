#pragma once
#include"NumberUI.h"
class C_Camera;
class C_RecoveryUI
{
public:
	C_RecoveryUI() {};
	~C_RecoveryUI() {};

	void Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera);
	void Update();
	void Draw();
	void Release();

	//回復UI生成
	void CreateRecoveryUI(float a_Recovery, Math::Vector2 a_Pos);

private:
	//回復UIの最大数
	static const int m_maxRecoveryUI = 20;
	C_NumberUI m_recoveryUIs[m_maxRecoveryUI];

	//回復UI
	Math::Vector2 m_UIsize = { 0.5f, 0.5f };//回復UIの大きさ（生成時）
	float m_UiTime = 50.0f;//表示時間
	Math::Vector2 m_moveSpeed = { 5.0f, 4.5f };//回復UIの移動速度
	Math::Vector2 m_recoveryUiPos[m_maxRecoveryUI];
	Math::Matrix m_recoveryUiMat[m_maxRecoveryUI];
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Color m_recoveryColor = { 0.392f,0.992f,0.301f,1 };
	Math::Color m_color[m_maxRecoveryUI];
	Math::Vector2 m_recoveryUiSize[m_maxRecoveryUI];//回復UIの大きさ（描画時）
	Math::Color m_frameColor = { 0,0,0.129f,1 };
	Math::Color m_0Color = { 1,1,1,1 };

	//画像
	KdTexture* m_numberTex;
	KdTexture* m_numberFrameTex;
	KdTexture m_recoveryTex;
	KdTexture m_flameTex;

	//参照するクラスのポインタ
	C_Camera* m_pCamera;

};