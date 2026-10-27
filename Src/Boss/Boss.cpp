#include"Boss.h"

void C_Boss::Draw()
{
	if (!m_bAlive)return;
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(896 * int(m_animationCnt / 5.0f), 0, 896, 512), 1.0f);
	
	Math::Color color = { 1.0, 0, 0, 1.0 };
	DrawDebugRect(&color);
}

void C_Boss::Update()
{
	if (!m_bAlive)return;
	Move();
	Gravity();

	m_animationCnt += 1;
	if (m_animationCnt > (6 * 5))m_animationCnt = 0;
	
	MapCollision();
	
}

void C_Boss::Move()
{

	Walk();

}

void C_Boss::Walk()
{
	m_move.x += BOSS_SPEED * -(m_scale.x);
}

void C_Boss::Charge()
{
	m_skillMove.x = 30;

}

void C_Boss::Init(C_Hit* a_pHit, C_Camera* a_pCamera)
{
	C_BossBase::Init(a_pHit, a_pCamera);
	m_scale = { -0.5,0.5 };
	m_gap = { CHIP_RADIUS,CHIP_RADIUS };
	SetRect(PTEX_SIZE * 2.5,PTEX_SIZE * 1.5);
}

