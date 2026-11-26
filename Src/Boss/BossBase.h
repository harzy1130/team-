#pragma once
#include"Constants/Constants.h"
#include"Camera/Camera.h"
#include"Hit/Hit.h"
#include"Hit/HitBase.h"
#include"Player/PlayerBase.h"

class C_PlayerBase;
class C_BossBase : public HitBase{
public:
	C_BossBase() {};
	virtual ~C_BossBase() = default;
	
	virtual void Update() = 0;
	virtual void Move() = 0;
	virtual void Draw();
	virtual void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer);
	virtual void Release();
	virtual void Animation(float a_sign = 1.0f);

	bool TakeDamage(int a_damage);
	bool AddParent(C_BossBase* a_pParent);
	//セッター
	void SetTexture(KdTexture* a_pTex) { m_pTex = a_pTex; }
	void SetHp(int a_hp);
	//ゲッター
	Math::Vector2 GetPos(){return m_pos;}
	Math::Vector2 GetMove(){return m_move;}
	bool GetAlive(){return m_bAlive;}
	float GetAngle(){return m_angle;}
	int GetHp() { return m_hp; };
	int GetAtk() { return m_atk; };
	int GetInvincibleFrames() { return m_invincibleFrames; };
	int GetLR() { return m_lr; };
	int GetParentCount() { return m_parents.size(); }
	C_BossBase* GetParent(int a_index) { if (a_index < 0 || a_index >= m_parents.size()) return nullptr; return m_parents[a_index]; }
	
private:
	void SetPlayerClass(C_PlayerBase* a_pPlayer);

protected:
	//必須仮想関数
	virtual void MatrixUpdate() final;
	virtual void Gravity() final;
	virtual void MapCollision() final;

	//状態
	bool m_bAlive;
	int m_animationCnt;
	int m_animationWait;
	int m_animationFrame;
	int m_animationMaxFrame;
	KdTexture* m_pTex;
	KdTexture m_texture;

	int m_hp = 500;
	int m_atk = 20;
	int m_invincibleFrames = 0;
	int m_moveState;
	int m_lr;
	float m_size;
	float m_moveSpeed = 5;
	
	Math::Vector2 m_skillMove{0,0};
	Math::Vector2 m_rawMove{ 0,0 };

	//ペアレント紐付け用配列
	vector<C_BossBase*> m_parents;

	//必要な外部クラス
	C_PlayerBase* m_pPlayer;

	//行列
	Math::Matrix m_mat;

};
