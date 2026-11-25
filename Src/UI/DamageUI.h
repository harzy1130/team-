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
	//敵のダメージUI生成
	void CreateEnemyDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);
	//装備追加のダメージUI生成
	void CreateGearDamageUI(float a_Damage, Math::Vector2 a_Pos, bool a_movingDirectionX, bool a_critical);

private:

	//ダメージUIの最大数
	static const int m_maxDamageUI = 10;
	C_NumberUI m_damageUIs[m_maxDamageUI];

	//ダメージUI(全体)
	Math::Vector2 m_UIsize = { 0.6f, 0.6f };//UIの大きさノーマル（生成時）
	Math::Vector2 m_cUIsize = { 0.8f, 0.8f };//UIの大きさクリティカル（生成時）
	Math::Vector2 m_drawUiSize[m_maxDamageUI];//UIの大きさ（描画時）
	Math::Vector2 m_moveSpeed = { 4.0f, 3.0f };//ダメージUIの移動速度
	float m_UiTime = 40.0f;//表示時間ノーマル
	float m_cUiTime = 50.0f;//表示時間クリティカル
	Math::Matrix m_transMat;
	Math::Matrix m_scaleMat;

	//ダメージUI(クリティカル)
	bool m_criticalFlg[m_maxDamageUI];
	Math::Vector2 m_criticalUiPos[m_maxDamageUI];
	Math::Matrix m_criticalUiMat[m_maxDamageUI];

	//ダメージUIの色
	Math::Color m_criticalColor={ 1,0.27f,0,1 };
	Math::Color m_dmgColor = { 1.0f,0.847f,0.0f,1 };
	Math::Color m_enemyDmgColor = { 0.724f,0,0,1 };
	Math::Color m_gearDmgColor = { 0.333f,0.866f,0.960f,1 };
	Math::Color m_frameColor = { 0,0,0,1 };

	Math::Color m_color[m_maxDamageUI];


	//画像
	KdTexture *m_numberTex;
	KdTexture* m_numberFrameTex;
	KdTexture m_criticalTex;

	//参照するクラスのポインタ
	C_Camera* m_pCamera;
};

