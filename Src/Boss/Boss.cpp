#include"Boss.h"

void C_Boss::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)
{
	C_BossBase::Init(a_pHit, a_pCamera, a_pPlayer);
	m_animationCnt = 0;
	m_animationFrame = 0;
	m_animationMaxFrame = 6;
	m_animationWait = 8;
	m_size = 0.5;
	m_scale = { m_size, m_size };
	m_texture.Load("Texture/Capricorn.png");
	if (LoadHitBox("Capricorn"));
	m_hp = 500;
	m_atk = 30;
	m_moveSpeed = 5.0f;
	m_moveState = State::walk;
	m_angle = 0;
	m_moveCnt = 0;
	m_backStep = 1;
}

void C_Boss::Update()
{
	if (!m_bAlive)return;
	if (m_invincibleFrames > 0) --m_invincibleFrames;
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
	if (!(m_moveState == State::walk || m_moveState == State::stance))
	{
		m_pHit->PlayerHitCheck(this);
	}
}

void C_Boss::Move()
{
	ChargeStance();
	Walk();
	JumpCharge();
	DashCharge();
	Tired();
	m_move.x += m_skillMove.x * m_lr;

}
void C_Boss::ChangeChargeStance()
{
	m_moveState = State::stance;
	m_moveCnt = 30;
}

void C_Boss::ChargeStance()
{
	if (m_moveState != State::stance)return;
	m_animationFrame = 3;
	m_moveCnt--;
	if (m_moveCnt < 0) {
		if (m_pPlayer->GetPos().y > m_pos.y)StartJumpCharge();
		else {
			StartDashCharge();
		}
	}
}

void C_Boss::StartWalk()
{
	m_moveState = State::walk;
	m_moveCnt = 180;
}


void C_Boss::Walk()
{
	if (m_moveState != State::walk)return;
	m_lr = GetSign(m_pPlayer->GetPos().x - m_pos.x);
	m_scale = { m_size * m_lr, m_size };
	m_moveCnt--;
	if (m_moveCnt < 0)m_moveCnt = 0;
	bool CheckDist = m_chargeRange > GetDist(m_pos, m_pPlayer->GetPos());
	if (m_chargeRange * 0.6 > GetDist(m_pos, m_pPlayer->GetPos()) && m_backStep >= 1)
	{
		m_backStep = -0.6;
	}
	if (m_chargeRange < GetDist(m_pos, m_pPlayer->GetPos()) && m_backStep < 1)
	{
		m_backStep = 1;
	}
	if (m_moveCnt <= 0 && CheckDist)ChangeChargeStance();
	Animation(m_backStep);
	m_rawMove.x += (m_moveSpeed / 15) * (m_lr * m_backStep);
	m_rawMove.x = Clamp(m_rawMove.x, m_moveSpeed * fabsf(m_backStep), -m_moveSpeed * fabsf(m_backStep));
	m_move += m_rawMove;
}


void C_Boss::StartDashCharge()
{
	m_moveState = State::dashCharge;
	ResetAlreadyHit();
	m_moveCnt = 30;
	m_skillMove.x = 30;
}

void C_Boss::DashCharge()
{
	if (m_moveState != State::dashCharge)return;
	m_skillMove.x *= 0.99;
	m_moveCnt--;
	if (m_moveCnt < 0) {
		m_skillMove = { 0,0 };
		StartTired();
	}
}

void C_Boss::StartJumpCharge()
{
	m_moveState = State::jumpCharge;
	ResetAlreadyHit();
	m_move.y = 12;
	m_skillMove.x = 30;
	ResetHitRect();
}

void C_Boss::JumpCharge()
{
	if (m_moveState !=State::jumpCharge)return;
	if (m_skillMove.x > 10)m_skillMove.x -= 1;
	if (m_bHitRect.bottom) {
		m_skillMove = {0,0};
		StartTired();
	}
}

void C_Boss::StartTired()
{
	m_moveState = State::tired;
	m_skillMove = { 0,0 };
	m_moveCnt = 60;
}

void C_Boss::Tired()
{
	if (m_moveState != State::tired)return;
	m_moveCnt--;
	if (m_moveCnt < 0) {
		StartWalk();
	}
}


void C_Boss::IfHitPlayer(C_PlayerBase* a_pPlayer)
{
	a_pPlayer->TakeDamage(m_atk, this);
}
