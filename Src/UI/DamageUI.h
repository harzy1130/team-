#pragma once
#include"NumberUI.h"

class C_Camera;
class C_DamageUI
{
public:
	C_DamageUI() {};

	~C_DamageUI() {};

	void Init(KdTexture* a_pNumberTex, KdTexture* a_pFrameTex, C_Camera* a_pCamera);
	void Update();
	void Draw();
	void Release();

	//ダメージUI生成
	void CreateDamageUI(float a_Damage, Math::Vector2 a_Pos,bool a_movingDirectionX , bool a_critical);

	void CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);

private:

	//ダメージUIの最大数
	static const int m_maxDamageUI = 10;
	C_NumberUI m_damageUIs[m_maxDamageUI];

	//ダメージUI(クリティカル)
	bool m_criticalFlg[m_maxDamageUI];
	Math::Vector2 m_criticalUiPos[m_maxDamageUI];
	Math::Matrix m_criticalUiMat[m_maxDamageUI];
	Math::Color m_criticalColor={ 1,0.27f,0,1 };
	Math::Color m_dmgColor = { 1,0.647,0,1 };
	Math::Color m_enemyDmgColor = { 0.724,0,0,1 };
	Math::Color m_frameColor = { 1, 0.27f ,0 , 1 };

	Math::Color m_color[m_maxDamageUI];


	//画像
	KdTexture *m_numberTex;
	KdTexture* m_numberFrameTex;
	KdTexture m_criticalTex;

	//参照するクラスのポインタ
	C_Camera* m_pCamera;
};

