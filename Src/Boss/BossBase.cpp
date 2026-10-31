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

	float x, y;
	x = GetPlusMinus(m_scale.x);
	y = GetPlusMinus(m_scale.y);
	m_sMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 1);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x + m_gap.x * x, m_draw.y + m_gap.y * y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
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
		m_pTex, Math::Rectangle(m_texWidth * int(m_animationCnt / m_animationWait), 0, m_texWidth, m_texHeight), 1.0f);
	Math::Color color = { 1.0, 0, 0, 1.0 };
	DrawDebugRect(&color);
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
	m_sMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 0);
	m_rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tMat = Math::Matrix::CreateTranslation(m_draw.x, m_draw.y, 0);
	m_mat = m_sMat * m_rMat * m_tMat;
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);
	SetPlayerClass(a_pPlayer);
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
