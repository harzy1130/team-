#pragma once
#include"NumberUI.h"
enum E_EffectDmgUIType
{
	E_TYPE_POISON,
	E_TYPE_FLAME,
};

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
	void CreatePoisonDamageUI(float a_Damage, Math::Vector2 a_Pos);
	void CreateFlameDamageUI(float a_Damage, Math::Vector2 a_Pos);

private:
	//ダメージUIの最大数
	static const int m_maxDamageUI = 30;
	C_NumberUI m_damageUIs[m_maxDamageUI];
	E_EffectDmgUIType m_type[m_maxDamageUI];

	//ダメージUI(全体)
	Math::Vector2 m_UIsize = { 0.5f, 0.5f };//UIの大きさ（生成時）
	float m_UiTime = 50.0f;//表示時間
	Math::Vector2 m_moveSpeed = { 5.0f, 3.0f };//ダメージUIの移動速度
	Math::Vector2 m_dmgUiSize[m_maxDamageUI];//UIの大きさ（描画時）
	Math::Vector2 m_UiPos[m_maxDamageUI];
	Math::Matrix m_UiMat[m_maxDamageUI];
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;
	Math::Color m_color[m_maxDamageUI];
	Math::Color m_frameColor = { 0,0,0,1 };
	Math::Color m_0Color = { 1,1,1,1 };
	//ダメージUI(毒)
	Math::Color m_poisonColor={0.333f,0.519f,0.184f,1};
	//Math::Color m_poisonColor={1,0,0,1};
	//ダメージUI(炎)
	Math::Color m_flameColor={1.0f,0.5f,0.0f,1};
	
	

	//画像
	KdTexture* m_numberTex;
	KdTexture* m_numberFrameTex;
	KdTexture m_poisonTex;
	KdTexture m_flameTex;

	//参照するクラスのポインタ
	C_Camera* m_pCamera;
};