#pragma once
#include"Constants/Constants.h"
#include"Camera/Camera.h"
#include"Hit/Hit.h"
#include"Hit/HitBase.h"

class C_Hit;
class C_Camera;
class C_BossBase : public HitBase{
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
	virtual void Gravity() final;
	virtual void MapCollision() final;

	//状態
	bool m_bAlive;
	float m_angle;
	int m_animationCnt;
	KdTexture* m_pTex;

	
	Math::Vector2 m_gap{0,0};
	Math::Vector2 m_skillMove{0,0};

	//行列
	Math::Matrix m_mat;
	Math::Matrix m_tMat;
	Math::Matrix m_sMat;
	Math::Matrix m_rMat;
};
