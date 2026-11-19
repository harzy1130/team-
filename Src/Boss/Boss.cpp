#include"Boss.h"


void C_Boss::Update()
{
	if (!m_bAlive)return;
	m_animationCnt += 1;
	if (m_invincibleFrames > 0) --m_invincibleFrames;
	if (m_animationCnt > (6 * m_animationWait))m_animationCnt = 0;
	Move();
	Gravity();
	//デバッグ用
	//m_pos = { 400, 0 };
	//m_move = { 0, 0 };
	//if (GetAsyncKeyState('J')) 
	//{
	//	m_move.x = -5;
	//}
	//if (GetAsyncKeyState('I'))
	//{
	//	m_move.y = 5;
	//}
	//if(GetAsyncKeyState('L'))
	//{
	//	m_move.x = 5;
	//}
	//if(GetAsyncKeyState('K'))
	//{
	//	m_move.y = -5;
	//}
	//if(GetAsyncKeyState('U'))
	//{
	//	m_angle += 1;
	//}
	//if(GetAsyncKeyState('O'))
	//{
	//	m_angle -= 1;
	//}
	if (m_angle > 360)m_angle -= 360;
	MapCollision();
	if (!(m_moveState == State(walk) || m_moveState == State(stance)))
	{
		m_pHit->PlayerHitCheck(this);
	}
	ResetAlreadyHit();
}

void C_Boss::Move()
{
	ChargeStance();
	Walk();
	JumpCharge();
	DashCharge();
	m_move.x += m_skillMove.x * m_lr;

}
void C_Boss::ChangeChargeStance()
{
	m_moveState = State(stance);
	m_chargeWait = 30;
}

void C_Boss::ChargeStance()
{
	if (m_moveState != State(stance))return;
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
	m_moveState = State(walk);
	m_chargeWait = 180;
}


void C_Boss::Walk()
{
	if (m_moveState != State(walk))return;
	m_lr = GetSign(m_pPlayer->GetPos().x - m_pos.x);
	m_scale = { m_size * m_lr, m_size };
	m_chargeWait--;
	if (m_chargeWait < 0)m_chargeWait = 0;

	bool CheckDist = m_chargeRange > GetDist(m_pos, m_pPlayer->GetPos());
	if (m_chargeWait <= 0 && CheckDist)ChangeChargeStance();
	m_rawMove.x += (m_moveSpeed / 15) * m_lr;
	m_rawMove.x = Clamp(m_rawMove.x, m_moveSpeed, -m_moveSpeed);
	m_move += m_rawMove;
}

void C_Boss::StartDashCharge()
{
	m_moveState = State(dashCharge);
	m_chargeWait = 30;
	m_skillMove.x = 30;
}

void C_Boss::DashCharge()
{
	if (m_moveState != State(dashCharge))return;
	m_skillMove.x *= 0.99;
	m_chargeWait--;
	if (m_chargeWait < 0) {
		m_skillMove = { 0,0 };
		StartWalk();
	}
}

void C_Boss::StartJumpCharge()
{
	m_moveState = State(jumpCharge);
	m_move.y = 12;
	m_skillMove.x = 30;
	ResetHitRect();
}

void C_Boss::JumpCharge()
{
	if (m_moveState !=State(jumpCharge))return;
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
	m_scale = {m_size, m_size };
	if (LoadHitBox("Capricorn"));
	//NewObbObjects({ 160, 50 }, { 32, 64 }, 45);
	//NewObbObjects({ 160, 50 }, { 32, 64 }, -45);
	//NewObbObjects({ 160, -10 }, { 32, 64 }, 45);
	m_hp = 500;
	m_atk = 10;
	m_moveSpeed = 5.0f;
	m_moveState = State(walk);
	m_angle = 0;
	m_chargeWait = 0;
}
void C_Boss::IfHitPlayer(C_PlayerBase* a_pPlayer)
{
	a_pPlayer->TakeDamage(m_atk, this);
}
