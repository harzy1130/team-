#include"Boss.h"

void C_Boss::Draw()
{
	if (!m_bAlive)return;
	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(896 * int(m_animationCnt / m_animationWait), 0, 896, 512), 1.0f);
	
	Math::Color color = { 1.0, 0, 0, 1.0 };
	DrawDebugRect(&color);
}

void C_Boss::Update()
{
	if (!m_bAlive)return;
	m_animationCnt += 1;
	if (m_animationCnt > (6 * m_animationWait))m_animationCnt = 0;
	Move();
	Gravity();
	
	MapCollision();
}

void C_Boss::Move()
{
	ChargeStance();
	Walk();
	JumpCharge();
	DashCharge();
	m_move.x += m_skillMove.x * -m_lr;

}
void C_Boss::ChangeChargeStance()
{
	m_state = State(stance);
	m_chargeWait = 30;
}

void C_Boss::ChargeStance()
{
	if (m_state != State(stance))return;
	m_animationCnt = 0;
	m_chargeWait--;
	if (m_chargeWait < 0) {
		if (m_pPlayer->GetPos().y > m_pos.y)StartJumpCharge();
		else {
			StartDashCharge();
		}
	}
}

void C_Boss::StartWalk()
{
	m_state = State(walk);
	m_chargeWait = 180;
}

void C_Boss::Walk()
{
	if (m_state != State(walk))return;
	m_lr = GetPlusMinus(-(m_pPlayer->GetPos().x - m_pos.x));
	m_scale = { m_size * m_lr, m_size };
	m_chargeWait--;
	if (m_chargeWait < 0)m_chargeWait = 0;

	bool CheckDist = m_chargeRange > GetDist(m_pos, m_pPlayer->GetPos());
	if (m_chargeWait <= 0 && CheckDist)ChangeChargeStance();
	m_rawMove.x += (m_moveSpeed / 15) * -m_lr;
	m_rawMove.x = Clamp(m_rawMove.x, m_moveSpeed, -m_moveSpeed);
	m_move += m_rawMove;
}

void C_Boss::StartDashCharge()
{
	m_state = State(dashCharge);
	m_chargeWait = 30;
	m_skillMove.x = 30;
}

void C_Boss::DashCharge()
{
	if (m_state != State(dashCharge))return;
	m_skillMove.x *= 0.99;
	m_chargeWait--;
	if (m_chargeWait < 0) {
		m_skillMove = { 0,0 };
		StartWalk();
	}
}

void C_Boss::StartJumpCharge()
{
	m_state = State(jumpCharge);
	m_move.y = 12;
	m_skillMove.x = 30;
	ResetHitRect();
}

void C_Boss::JumpCharge()
{
	if (m_state !=State(jumpCharge))return;
	if (m_skillMove.x > 10)m_skillMove.x -= 1;
	if (m_bHitRect.bottom) {
		m_skillMove = {0,0};
		StartWalk();
	}
}

void C_Boss::Init(C_Hit* a_pHit, C_Camera* a_pCamera,C_PlayerBase* a_pPlayer)
{
	C_BossBase::Init(a_pHit, a_pCamera, a_pPlayer);
	m_size = 0.5;
	m_animationWait = 8;
	m_scale = { -m_size * m_lr, m_size };
	m_gap = { CHIP_RADIUS,CHIP_RADIUS };
	SetRect(PTEX_SIZE * 2.5,PTEX_SIZE * 1.5);
	m_hp = 500;
	m_atk = 20;
	m_moveSpeed = 5.0f;
	m_state = State(walk);
	m_angle = 0;
	m_chargeWait = 0;
}

