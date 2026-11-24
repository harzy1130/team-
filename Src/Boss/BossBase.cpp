#include "BossBase.h"

void C_BossBase::SetPlayerClass(C_PlayerBase* a_pPlayer)
{
	m_pPlayer = a_pPlayer;
}

void C_BossBase::MatrixUpdate()
{
	m_pos += m_move;
	m_move.x = 0;
	m_draw = m_pos - m_pCamera->GetPos();

	m_mat = CalcDrawMatrix();
}

void C_BossBase::Gravity()
{
	m_move.y -= GRAVITY;
}

void C_BossBase::MapCollision()
{
	if (m_move.y > CHIP_SIZE)m_move.y = CHIP_SIZE;
	if (m_pHit != nullptr) m_pHit->MapHitCheck(this);
	MatrixUpdate();
}

void C_BossBase::Draw()
{
	if (!m_bAlive)return;
	if (m_invincibleFrames > 0 && (m_invincibleFrames / 4) % 2 == 0) return;
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		&m_texture, Math::Rectangle(m_texWidth * m_animationFrame, 0, m_texWidth, m_texHeight), 1.0f);
	
	DrawHitBox();
}

void C_BossBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_draw = { 0,0 };
	m_bAlive = true;
	m_angle = 0;
	m_animationCnt = 0;
	m_size = 1;
	m_lr = 1;
	m_mat = CalcDrawMatrix();
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);
	SetPlayerClass(a_pPlayer);
}

void C_BossBase::Release()
{
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_draw = { 0,0 };
	m_bAlive = true;
	m_angle = 0;
	m_animationCnt = 0;
	m_size = 0;
	m_lr = 0;
	m_mat = Math::Matrix::Identity;
	m_hp = 0;
	m_atk = 0;
	m_invincibleFrames = 0;
	m_moveState = 0;
	m_moveSpeed = 5;
	m_pCamera = nullptr;
	m_pHit = nullptr;
	m_pPlayer = nullptr;
	m_texture.Release();
}

void C_BossBase::Animation(float a_sign)
{
	int sign = GetSign(a_sign);
	m_animationCnt++;
	if(m_animationCnt >= m_animationWait)
	{
		m_animationCnt = 0;
		m_animationFrame += sign;
		if(m_animationFrame >= m_animationMaxFrame)m_animationFrame = 0;
		if(m_animationFrame < 0)m_animationFrame = m_animationMaxFrame - 1;
	}
}

bool C_BossBase::TakeDamage(int a_damage)
{
	if (!m_bAlive || a_damage <= 0 || m_invincibleFrames > 0)return false;
	SetHp(m_hp - std::min(a_damage, m_hp));
	if (!m_bAlive)return true;
	m_invincibleFrames = 30;
	return true;
}

void C_BossBase::SetHp(int a_hp)
{
	m_hp = std::max(0, a_hp);
	if (m_hp == 0)
	{
		m_bAlive = false;
		SetMove(0.0f, 0.0f);
	}
}
