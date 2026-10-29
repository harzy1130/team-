#include "PlayerBase.h"
#include "hit/hit.h"
#include"Cursor/Cursor.h"
#include <cmath>

void C_PlayerBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor* a_pCursor)
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
	m_MAXHP = 100;
	m_HP = m_MAXHP;
	m_DEF = 50;
	m_ATK = 80;
	m_AS = 1.3;
	//特殊ステータスd
	m_LifStl = 0;
	m_MSUP = 0;
	m_HPRegene = 0;
	m_CrtDmgP = 150;
	m_CrtRate = 0;
	m_SkillHaste=50;
	m_IncreasedASP = 0;
	//通常攻撃
	InitAtk();
	m_skill.Init();
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);
	SetRect(PTEX_RADIUS, PTEX_RADIUS);
	SetRadius(PTEX_RADIUS);
	//状態異常
	m_effect.InitPlayer(this);
}

void C_PlayerBase::Update()
{
    if (!m_bAlive) { m_move = { 0, 0 }; return; }
    if (m_damageInvincibleFrames > 0) --m_damageInvincibleFrames;
	m_move.x = 0.0f;
	m_move.y -= GRAVITY;

	m_skill.Update();
	PlayerInput();
	UpdateAtk();
	m_effect.UpdatePlayer(this);
	if (m_pCursor->GetMousePos().x > 0) {
		m_scaleX = 1;
	}
	else
	{
		m_scaleX = -1;
	}
	
	m_move += m_skillMove;
	//当たり判定
	m_pHit->MapHitCheck(this);
	m_bJump = !m_bHitRect.bottom;
	m_pHit->EnemyHitCheck(this);

	//座標確定
	m_pos += m_move;
	m_AtkPos = { m_pos.x + (m_scaleX * 55),m_pos.y};

	if (m_pos.y <= -300) {
		m_pos.y = -300;
        m_move.y = 0.0f;
		m_bJump = false;
	}
	CalcMat();
}

void C_PlayerBase::Draw()
{
	if (!m_bAlive)return;
    if (m_damageInvincibleFrames > 0 && (m_damageInvincibleFrames / 4) % 2 == 0) return;
	//カメラ座標修正

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(0, 0, 64, 64), 1.0f);
	DrawAtk();
}

//防御力計算
float C_PlayerBase::DefCalc(float a_DMG, float a_DEF, float a_penet)
{
	return a_DMG * (100 / (100 + (a_DEF * ((100 - a_penet) / 100))));
}

//HPリジェネ
void C_PlayerBase::HPRegeneCalc()
{
	//一秒に一回回復
	m_HPRegeneCD--;
	if (m_HPRegeneCD <= 0) {
		m_HPRegeneCD = 0;
	}
	if (m_HPRegeneCD > 0)return;

	m_HP += m_HPRegene;
	m_HPRegeneCD = 60;
	return;
}

//クリティカル
float C_PlayerBase::CrtDmgCalc(float a_DMG, float a_CrtDmg, float a_CrtRate)
{
	if (rand() % 100 + 1 <= a_CrtRate) {
		return a_DMG * (a_CrtDmg / 100);
	}
	else
	{
		return a_DMG;
	}
}

//スピード加速
float C_PlayerBase::MoveSpeedCalc(float a_speed, float a_MS)
{
	return a_speed*(a_MS/100);
}

//攻撃速度計算
float C_PlayerBase::AtkSpeedCalc()
{
	return 60/(m_AS + (m_AS * m_IncreasedASP/100));
}

//キー操作
void C_PlayerBase::PlayerInput()
{
	if (!m_bAlive)return;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)||(GetAsyncKeyState('D') & 0x8000))
	{
		m_move.x += 8.0f;
	}
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
	{
		m_move.x -= 8.0f;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (m_bJump == false) {
			m_bJump = true;
			m_move.y = m_JumpPower;
		}
	}
	if (GetAsyncKeyState(VK_SHIFT) & 0x8000) {
		m_skill.SkillActivation(this);
	}
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000) {
		PlayerAtk();
	}
	if(GetAsyncKeyState('L') & 0x8000) {
		ResetHitEnemies();
	}
}

//行列合成
void C_PlayerBase::CalcMat()
{
	m_transMat= Math::Matrix::CreateTranslation(
		m_pos.x - m_pCamera->GetPos().x, m_pos.y - m_pCamera->GetPos().y, 0);
	m_scaleMat= Math::Matrix::CreateScale(m_scaleX, 1, 1);
	m_mat = m_scaleMat * m_transMat;

	m_AtkTransMat = Math::Matrix::CreateTranslation(
		m_AtkPos.x - m_pCamera->GetPos().x, m_AtkPos.y - m_pCamera->GetPos().y, 0);
	m_AtkScaleMat = Math::Matrix::CreateScale(m_scaleX, 1, 1);
	m_AtkMat = m_AtkScaleMat * m_AtkTransMat;
}

//通常攻撃
void C_PlayerBase::PlayerAtk()
{
	if (!m_bAlive || !m_AttackDelay == 0)return;
	m_maxAtkCount = 10;
	m_AtkCount = m_maxAtkCount;
	m_bAtk = true;
	m_AttackDelay = AtkSpeedCalc();

}

void C_PlayerBase::InitAtk()
{
	m_AtkCount = 0;
	m_atkAnimCount = 0;
	m_AttackDelay = 0;
	m_bAtk = false;

}

void C_PlayerBase::UpdateAtk()
{
	m_AttackDelay--;
	if (m_AttackDelay <= 0)
	{
		m_AttackDelay = 0;
		m_atkAnimCount = 0;
		m_bAtk = false;
	}
	m_atkAnimCount = (3 - m_AttackDelay / AtkSpeedCalc() * 3)*3.5;
	if (m_atkAnimCount >= 3) 
	{
		m_atkAnimCount = 0;
	}
}

void C_PlayerBase::DrawAtk()
{
	if (!m_bAlive)return;
	SHADER.m_spriteShader.SetMatrix(m_AtkMat);
	SHADER.m_spriteShader.DrawTex(
		m_pAtkTex, Math::Rectangle(64*(int)m_atkAnimCount, 0, 64, 64), 1.0f);

}

bool C_PlayerBase::TakeDamage(float rawDamage)
{
    if (!m_bAlive || m_HP <= 0.0f || m_damageInvincibleFrames > 0 || !std::isfinite(rawDamage) || !(rawDamage > 0.0f) || !std::isfinite(m_DEF)) return false;
    const float damage = DefCalc(rawDamage, std::max(0.0f, m_DEF), 0.0f);
    m_HP = std::max(0.0f, m_HP - damage);
    m_damageInvincibleFrames = 30;
    if (m_HP == 0.0f)
    {
        m_bAlive = false;
        m_move = { 0.0f, 0.0f };
        m_skillMove = { 0.0f, 0.0f };
    }
    return true;
}

void C_PlayerBase::IfHitEnemy(EnemyBase* a_pEnemy)
{
	a_pEnemy->TakeDamage(20, m_pos.x);

}
