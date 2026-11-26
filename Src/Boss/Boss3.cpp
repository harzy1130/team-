#include "Boss3.h"

void C_Boss3::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_PlayerBase* a_pPlayer)
{
	C_BossBase::Init(a_pHit, a_pCamera, a_pPlayer);
	m_texture.Load(BOSS_TEX_PATH + "Scorpius1.png");
	m_animationCnt = 0;
	m_animationFrame = 0;
	m_animationMaxFrame = 6;
	m_animationWait = 8;
	m_size = 2;
	m_scale = { m_size, m_size };
	m_hp = 500;
	m_atk = 30;
	m_moveSpeed = 5.0f;
	m_angle = 0;
	LoadHitBox("Scorpius");
	TailInit();
	ScissorInit();
}

void C_Boss3::Update()
{
	MainBodyUpdate();
	TailUpdate();
	ScissorUpdate();
}

void C_Boss3::Draw()
{
	m_tail2.Draw();
	m_needle.Draw();
	m_tail1.Draw();
	m_scissor2.Draw();
	C_BossBase::Draw();
	m_scissor1.Draw();
}

void C_Boss3::Release()
{
	C_BossBase::Release();
}

void C_Boss3::Move()
{
}

void C_Boss3::MainBodyUpdate()
{
	if (!m_bAlive)return;
	if (m_invincibleFrames > 0) --m_invincibleFrames;
	Move();
	Gravity();
	//デバッグ用
	//m_pos = { 400, 0 };
	m_move = { 0, 0 };
	if (GetAsyncKeyState('J')) 
	{
		m_move.x = -5;
	}
	if (GetAsyncKeyState('I'))
	{
		m_move.y = 5;
	}
	if(GetAsyncKeyState('L'))
	{
		m_move.x = 5;
	}
	if(GetAsyncKeyState('K'))
	{
		m_move.y = -5;
	}
	if(GetAsyncKeyState('U'))
	{
		m_angle += 1;
	}
	if(GetAsyncKeyState('O'))
	{
		m_angle -= 1;
	}
	if (GetAsyncKeyState('P'))
	{
		if (!m_dbgKey)
		{
			m_lr *= -1;
			m_dbgKey = true;
		}
	}
	else m_dbgKey = false;
	if (GetAsyncKeyState('B'))
	{
		Init(m_pHit, m_pCamera, m_pPlayer);
	}
	m_scale = { m_size * m_lr,m_size };
	if (m_angle > 360)m_angle -= 360;
	MapCollision();
}

void C_Boss3::TailInit()
{
	m_tail1.SetParent(this);
	m_tail1.Init(m_pHit, m_pCamera, m_pPlayer);
	m_tail1.LoadHitBox("ScorpiusTail");
	m_tail1.SetOffsetPos({ 0, 50 * m_size });
	m_tail1.SetOffsetAngle(60);
	m_tail1.SetTexturePath(BOSS_TEX_PATH + "ScorpiusTail.png");

	m_tail2.SetParent(&m_tail1);
	m_tail2.Init(m_pHit, m_pCamera, m_pPlayer);
	m_tail2.LoadHitBox("ScorpiusTail");
	m_tail2.SetOffsetPos(Math::Vector2{ m_tail1.GetGap().x * 2,0 });
	m_tail2.SetOffsetAngle(-30);
	m_tail2.SetTexturePath(BOSS_TEX_PATH + "ScorpiusTail.png");

	m_needle.SetParent(&m_tail2);
	m_needle.Init(m_pHit, m_pCamera, m_pPlayer);
	m_needle.LoadHitBox("ScorpiusNeedle");
	m_needle.SetOffsetPos(Math::Vector2{ m_tail2.GetGap().x * 2,0 });
	m_needle.SetOffsetAngle(-40);
	m_needle.SetTexturePath(BOSS_TEX_PATH + "ScorpiusNeedle.png");
	m_parents.clear();
	m_parents.push_back(&m_tail1);
	m_parents.push_back(&m_tail2);
	m_parents.push_back(&m_needle);
}

void C_Boss3::TailUpdate()
{
	//m_tail1.SetOffsetAngle(m_tail1.GetOffsetAngle() + 1);
	m_tail1.Update();
	//m_tail2.SetOffsetAngle(m_tail1.GetOffsetAngle() + 1);
	m_tail2.Update();
	//m_needle.SetOffsetAngle(m_tail2.GetOffsetAngle() + 1);
	m_needle.Update();
}

void C_Boss3::ScissorInit()
{
	m_scissor1.SetParent(this);
	m_scissor1.Init(m_pHit, m_pCamera, m_pPlayer);
	m_scissor1.LoadHitBox("ScorpiusScissors");
	m_scissor1.SetTexturePath(BOSS_TEX_PATH + "ScorpiusScissors1.png");
	m_scissor1.SetOffsetPos({ 18 * m_size, -52 * m_size });
	
	m_scissor2.SetParent(this);
	m_scissor2.Init(m_pHit, m_pCamera, m_pPlayer);
	m_scissor2.LoadHitBox("ScorpiusScissors");
	m_scissor2.SetTexturePath(BOSS_TEX_PATH + "ScorpiusScissors2.png");
	m_scissor2.SetOffsetPos({ 40 * m_size, -37 * m_size });
	m_parents.push_back(&m_scissor1);
	m_parents.push_back(&m_scissor2);
}

void C_Boss3::ScissorUpdate()
{
	m_scissor1.Update();
	m_scissor2.Update();
}

