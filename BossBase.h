#pragma once
#include"BossConstants.h"

class BossBase {
public:

	virtual ~BossBase() = default;

	//セッター
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	
	//ゲッター
	

protected:
	//必須純粋仮想関数
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void MatrixUpdate() = 0;
	virtual void Draw() = 0;
	virtual void Init() = 0;

	//状態
	bool m_bAlive;
	float m_xScale;
	float m_yScale;
	float m_angle;
	KdTexture* m_pTex;

	//座標
	Math::Vector2 m_pos;
	Math::Vector2 m_move;
	Math::Vector2 m_draw;

	//行列
	Math::Matrix m_mat;
	Math::Matrix m_tMat;
	Math::Matrix m_sMat;
	Math::Matrix m_rMat;

	S_Boss m_boss;
};