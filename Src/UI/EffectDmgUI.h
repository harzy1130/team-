#pragma once
#include"NumberUI.h"
class C_Camera;
class C_EffectDmgUI
{
public:
	C_EffectDmgUI() {};
	~C_EffectDmgUI() {};

	void Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera);
	void Update();
	void Draw();
	void Release();

	//ダメージUI生成
	void CreateEffectDamageUI(float a_Damage, Math::Vector2 a_Pos);

private:
	//ダメージUIの最大数
	static const int m_maxDamageUI = 20;
	C_NumberUI m_damageUIs[m_maxDamageUI];

	//ダメージUI(毒)
	Math::Vector2 m_poisonUiPos[m_maxDamageUI];
	Math::Matrix m_poisonUiMat[m_maxDamageUI];
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Color m_poisonColor={0.25f,0.992f,0.078f,1};
	Math::Color m_color[m_maxDamageUI];
	Math::Color m_frameColor={0,0,0.129f,1};

	//画像
	KdTexture* m_numberTex;
	KdTexture* m_numberFrameTex;
	KdTexture m_poisonTex;
	

	//参照するクラスのポインタ
	C_Camera* m_pCamera;
};