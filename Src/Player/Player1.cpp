#include "Player1.h"
#include"Cursor/Cursor.h"
#include <cmath>

void C_Player1::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor* a_pCursor)
{
	//移動
	m_bAlive = true;
	m_pos = { 0,0 };
	m_move = { 0,0 };
	m_damageInvincibleFrames = 0;
	m_skillMove = { 0,0 };
	m_speed = 10;
	m_bJump = false;
	//基本ステータス
	m_Level = 1;
	m_MAXHP = 100;
	m_HP = m_MAXHP;
	m_DEF = 50;
	m_ATK = 80;
	m_AS = 1.2;
	//特殊ステータスd
	m_LifStl = 0;
	m_MSUP = 0;
	m_HPRegene = 0;
	m_CrtDmgP = 150;
	m_CrtRate = 0;
	m_SkillHaste = 50;
	m_IncreasedASP = 0;
	//通常攻撃
	InitAtk();
	m_skill.Init();
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);
	SetRect(PTEX_RADIUS, 60);
	SetRadius(PTEX_RADIUS);
	NewRectObjects({ 0, 0 }, { -16, 48,16,-60});
	m_hitRange = PTEX_SIZE;
	//状態異常
	m_effect.InitPlayer(this);
	m_sword.Init(this, a_pHit, a_pCamera);
}

void C_Player1::Update()
{
	if (!m_bAlive) { m_move = { 0, 0 }; return; }
	if (m_damageInvincibleFrames > 0) --m_damageInvincibleFrames;
	m_move.x = 0.0f;
	m_move.y -= GRAVITY;

	m_skill.Update();
	PlayerInput();
	UpdateAtk();
	AnimationUpdate();
	m_effect.UpdatePlayer(this);
	//if (m_pCursor->GetMousePos().x > 0) {
	//	m_scaleX = 1;
	//}
	//else
	//{
	//	m_scaleX = -1;
	//}

	m_move += m_skillMove;
	//当たり判定
	m_pHit->MapHitCheck(this);
	m_bJump = !m_bHitRect.bottom;
	m_pHit->EnemyHitCheck(this);
	m_pHit->BossHitCheck(this);

	//座標確定
	m_pos += m_move;
	m_AtkPos = { m_pos.x + (m_scale.x * 55),m_pos.y };

	if (m_pos.y <= -300) {
		m_pos.y = -300;
		m_move.y = 0.0f;
		m_bJump = false;
	}
	MatrixUpdate();
	m_sword.Update();
}

void C_Player1::Draw()
{
	if (!m_bAlive)return;
	if (m_damageInvincibleFrames > 0 && (m_damageInvincibleFrames / 4) % 2 == 0) return;
	//カメラ座標修正

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(600*(int)m_moveAnimCount, 600*m_moveMode, 600, 600), 1.0f);
	DrawHitBox();
	//DrawAtk();
	m_sword.Draw();
}
