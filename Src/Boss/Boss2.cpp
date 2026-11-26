#include "Boss2.h"

void C_Boss2::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)
{
	C_BossBase::Init(a_pHit, a_pCamera, a_pPlayer);
	m_animationCnt = 0;
	m_animationWait = 5;
	m_animationFrame = 0;
	m_animationMaxFrame = 5;
	m_size = 2;
	m_scale = { m_size, m_size };
	m_texture.Load(BOSS_TEX_PATH + "Aphrodite.png");
	m_projectileTex.Load("Texture/Chara.png");
	LoadHitBox("AphroditeTest");
	m_hp = 450;
	m_atk = 30;
	m_rawMove = { 0,0 };
	m_jumpVelocity = { 0,0 };
	StartWalk();
}

void C_Boss2::Update()
{
	if (!m_bAlive||m_moveStack.empty())return;
	if (m_invincibleFrames > 0) --m_invincibleFrames;
	if (m_angle > 360)m_angle -= 360;
	if(m_angle < -360)m_angle += 360;
	Animation();
	m_scale = { m_size * m_lr, m_size };
	m_moveCnt--;
	if (m_moveCnt <= 0)m_moveCnt = 0;
	Move();
	MatrixUpdate();
	m_pHit->PlayerHitCheck(this);
	ResetAlreadyHit();

	ProjectileUpdate();
}

void C_Boss2::Draw()
{
	C_BossBase::Draw();
	ProjectileDraw();
}

void C_Boss2::Release()
{
	C_BossBase::Release();
	m_projectiles.clear();
}

void C_Boss2::Move()
{
	m_move = {0,0};
	Walk();
	Dive();
	SetJump();
	JumpAttack();
	m_move += m_rawMove;
}

void C_Boss2::ProjectileUpdate()
{
	for(auto itr = m_projectiles.begin(); itr != m_projectiles.end();)
	{
		(*itr)->Update(m_pHit->GetMapClass(),m_pPlayer);
		if(!(*itr)->GetAlive())itr = m_projectiles.erase(itr);
		else++itr;		
	}
}

void C_Boss2::ProjectileDraw()
{
	for (auto& projectile : m_projectiles)
	{
		projectile->Draw(m_pCamera,&m_projectileTex);
	}
}

void C_Boss2::AddProjectile(Math::Vector2 a_pos, Math::Vector2 a_vec, float a_speed, int a_damage)
{
	auto projectile = std::make_unique<C_EnemyProjectile>();
	projectile->Init(a_pos, a_vec, a_speed, a_damage, 180);
	m_projectiles.push_back(std::move(projectile));
}

void C_Boss2::StartWalk()
{
	if (m_draw.y < BOTTOM_EDGE * 1.3)
	{
		m_pos.x = m_pPlayer->GetPos().x - m_pos.x < 0 ? m_pCamera->GetPos().x + RIGHT_EDGE * 1.3 : m_pCamera->GetPos().x + LEFT_EDGE * 1.3;
		m_pos.y = m_pCamera->GetPos().y + BOTTOM_EDGE;
	}
	m_moveStack.push(State::walk);
	m_moveCnt = 180;
}

void C_Boss2::Walk()
{
	if(m_moveStack.empty()||m_moveStack.top() != State::walk)return;
	if (m_draw.x < LEFT_EDGE)m_lr = 1;
	if (m_draw.x > RIGHT_EDGE)m_lr = -1;
	if (m_draw.y > BOTTOM_EDGE)
	{
		float tmpAngle = fabsf(m_angle);
		tmpAngle *= -m_lr;
		tmpAngle -= m_lr;
		tmpAngle = Clamp(tmpAngle, 30, -30);
		m_angle = tmpAngle;
	}
	else
	{
		m_angle++;
		if (m_angle > 0)m_angle = 0;
	}
	m_move.y = sinf(ToRadians(m_angle)) * m_moveSpeed * m_lr;
	m_rawMove.x += (m_moveSpeed / 30) * m_lr;
	m_rawMove.x = Clamp(m_rawMove.x, m_moveSpeed, -m_moveSpeed);

	if (m_moveCnt <= 0)StartJump();
}

void C_Boss2::Dive()
{
	if(m_moveStack.empty()||m_moveStack.top() != State::dive)return;
	float tmpAngle = fabsf(m_angle);
	tmpAngle *= -m_lr;
	tmpAngle -= m_lr;
	tmpAngle = Clamp(tmpAngle, 30, -30);
	m_angle = tmpAngle;

	m_move.y = sinf(ToRadians(m_angle)) * m_moveSpeed * 1.5 * m_lr;
	m_rawMove.x += (m_moveSpeed / 30) * m_lr;
	m_rawMove.x = Clamp(m_rawMove.x, m_moveSpeed*1.5, -m_moveSpeed*1.5);
	if (m_draw.y < BOTTOM_EDGE * 2)m_moveStack.pop();
}

void C_Boss2::StartJump()
{
	m_moveStack.push(State::setJump);
	m_moveStack.push(State::dive);
}

void C_Boss2::SetJump()
{
	if(m_moveStack.empty()||m_moveStack.top() != State::setJump)return;
	m_lr = GetSign(m_pPlayer->GetPos().x - m_pos.x);
	if(rand() % 2 == 0)	m_pos.x = m_pCamera->GetPos().x + (RIGHT_EDGE * 1.3 * -m_lr);
	m_rawMove = { 0,0 };

	Math::Vector2 toPlayer = m_pPlayer->GetPos() - m_pos
		//+ Math::Vector2{0,100}
	;
	m_jumpVelocity.x = toPlayer.x / m_jumpTime;
	m_jumpVelocity.x = Clamp(m_jumpVelocity.x, m_moveSpeed * 1.6f, -m_moveSpeed * 1.6f);
	if (fabsf(m_jumpVelocity.x) < 2.0f)
	{
		m_jumpVelocity.x = 2.0f * m_lr;
	}
	m_jumpVelocity.y = (toPlayer.y + (0.5f * m_jumpGravity * m_jumpTime * m_jumpTime)) / m_jumpTime;
	m_angle = ToDegrees(atan2f(m_jumpVelocity.y, fabsf(m_jumpVelocity.x))) * m_lr;
	m_moveStack.pop();
	m_moveStack.push(State::jumpAttack);
}

void C_Boss2::JumpAttack()
{
	if(m_moveStack.empty()||m_moveStack.top() != State::jumpAttack)return;
	m_move = m_jumpVelocity;
	m_jumpVelocity.y -= m_jumpGravity;

	if (fabsf(m_move.x) > 0.01f)
	{
		m_lr = GetSign(m_move.x);
	}
	m_angle = ToDegrees(atan2f(m_move.y, std::max(0.001f, fabsf(m_move.x)))) * m_lr;
	//m_shotCnt++;
	//if (m_shotCnt >= m_shotWait)
	//{
	//	Math::Vector2 vec = { cosf(ToRadians(CalcAngle(m_pos, m_pPlayer->GetPos()))),sinf(ToRadians(CalcAngle(m_pos, m_pPlayer->GetPos()))) };
	//	AddProjectile(m_pos, vec, 4, 10);
	//	m_shotCnt = 0;
	//}
	if(m_draw.y < BOTTOM_EDGE * 3 && m_jumpVelocity.y < 0)
	{
		m_moveStack.pop();
		StartWalk();
	}
}

void C_Boss2::IfHitPlayer(C_PlayerBase* a_pPlayer)
{
	a_pPlayer->TakeDamage(m_atk, this);
}
