#pragma once
#include"Constants/Constants.h"
#include"Camera/Camera.h"
#include"Hit/Hit.h"

class C_Hit;
class C_Camera;
class C_BossBase {
public:

	virtual ~C_BossBase() = default;
	
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Draw() = 0;
	virtual void Init(C_Hit* a_pHit, C_Camera* a_pCamera);


	//セッター
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	
	//ゲッター
	Math::Vector2 GetPos(){return m_pos;}
	Math::Vector2 GetMove(){return m_move;}
	bool GetAlive(){return m_bAlive;}
	float GetAngle(){return m_angle;}

protected:
	//必須純粋仮想関数
	virtual void MatrixUpdate() final;
	
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

	//参照する別クラスのポインタ
	C_Hit* m_pHit;
	C_Camera* m_pCamera;
};
