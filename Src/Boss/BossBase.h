#pragma once
#include"Constants/Constants.h"
#include"Camera/Camera.h"
#include"Hit/Hit.h"
#include"Hit/HitBase.h"
#include"Player/PlayerBase.h"

class C_PlayerBase;
class C_BossBase : public HitBase{
public:

	virtual ~C_BossBase() = default;
	
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Draw();
	virtual void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer);

	bool TakeDamage(int a_damage);
	//セッター
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	void SetHp(int a_hp);
	//ゲッター
	Math::Vector2 GetPos(){return m_pos;}
	Math::Vector2 GetMove(){return m_move;}
	bool GetAlive(){return m_bAlive;}
	float GetAngle(){return m_angle;}

	
private:
	void SetPlayerClass(C_PlayerBase* a_pPlayer);

protected:
	//必須純粋仮想関数
	virtual void MatrixUpdate() final;
	virtual void Gravity() final;
	virtual void MapCollision() final;

	//状態
	bool m_bAlive;
	float m_angle;
	int m_animationCnt;
	float m_animationWait;
	KdTexture* m_pTex;
	int m_texWidth;
	int m_texHeight;

	int m_hp = 500;
	int m_atk = 20;
	int m_invincibleFrames = 0;
	int m_state;
	int m_lr;
	float m_size;
	float m_moveSpeed = 5;
	
	Math::Vector2 m_gap{0,0};
	Math::Vector2 m_skillMove{0,0};
	Math::Vector2 m_rawMove{ 0,0 };
	C_PlayerBase* m_pPlayer;

	//行列
	Math::Matrix m_mat;
	Math::Matrix m_tMat;
	Math::Matrix m_sMat;
	Math::Matrix m_rMat;
};
