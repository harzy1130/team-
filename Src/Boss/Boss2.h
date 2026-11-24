#pragma once
#include"BossBase.h"
#include"projectile/enemyprojectile.h"

class C_Boss2 : public C_BossBase
{
public:
	C_Boss2() {};
	~C_Boss2() {};
	void Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)override;
	void Update()override;
	void Draw()override;
	void Release()override;
	void Move()override;

	void ProjectileUpdate();
	void ProjectileDraw();
	void AddProjectile(Math::Vector2 a_pos, Math::Vector2 a_vec, float a_speed, int a_damage);

	void StartWalk();
	void Walk();

	void Dive();

	void StartJump();
	void SetJump();
	void JumpAttack();
	
	void IfHitPlayer(C_PlayerBase* a_pPlayer)override;
	
private:
	static const enum State {
		walk,
		dive,
		setJump,
		jumpAttack,
	};
	KdTexture m_projectileTex;
	std::stack<State> m_moveStack;
	const float m_moveSpeed = 10;
	const float m_jumpGravity = GRAVITY * 0.4f;
	const float m_jumpTime = 70.0f;
	int m_moveCnt = 180;
	Math::Vector2 m_jumpVelocity{ 0,0 };
	int m_shotCnt = 0;
	int m_shotWait = 10;

	std::vector<std::unique_ptr<C_EnemyProjectile>> m_projectiles;
};