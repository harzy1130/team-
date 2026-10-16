#include "PlayerBase.h"


void C_PlayerBase::Init()
{
	//移動
	m_bAlive = true;
	m_pos = { 0,0 };
	m_speed = 10;
	m_jumpFlg = false;
	//基本ステータス
	m_MAXHP = 100;
	m_HP = m_MAXHP;
	m_DEF = 50;
	m_ATK = 80;
	m_AS = 100;
	//特殊ステータス
	m_LifStl = 0;
	m_MSUP = 0;
	m_HPRegene = 0;
	m_CrtDmgP = 150;
	m_CrtRate = 0;
	//画像
	m_TexSize = 64;
	m_TexRadius = 32;
}

void C_PlayerBase::Update()
{
	m_move.x = 0.0f;
	m_move.y -= m_Gravity;

	PlayerInput();

	//当たり判定
	HitBaseSet();


	HitBaseResult();

	//座標確定
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	if (m_pos.y <= -300) {
		m_pos.y = -300;
		m_jumpFlg = false;
	}
	MatCalc();
}

void C_PlayerBase::Draw(Math::Vector2 a_pos)
{
	if (!m_bAlive)return;
	//カメラ座標修正
	m_mat = Math::Matrix::CreateTranslation(m_pos.x - a_pos.x, m_pos.y - a_pos.y, 0);

	SHADER.m_spriteShader.SetMatrix(m_mat);
	SHADER.m_spriteShader.DrawTex(
		m_pTex, Math::Rectangle(0, 0, 64, 64), 1.0f);
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
	return a_speed * (a_MS / 100);
}

//キー操作
void C_PlayerBase::PlayerInput()
{
	if (!m_bAlive)return;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)||(GetAsyncKeyState('D') & 0x8000))
	{
		m_move.x += 8.0f;
		m_scaleX = 1;
	}
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
	{
		m_move.x -= 8.0f;
		m_scaleX = -1;
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) {
		if (m_jumpFlg == false) {
			m_jumpFlg = true;
			m_move.y = m_JumpPower;
		}
	}
}

void C_PlayerBase::HitBaseResult()
{
	m_pos = m_hitBase.pos;
	m_move = m_hitBase.Move;
}

void C_PlayerBase::HitBaseSet()
{
	m_hitBase.pos = m_pos;
	m_hitBase.Move = m_move;
	m_hitBase.hitTop = false;
	m_hitBase.hitBottom = false;
	m_hitBase.hitLeft = false;
	m_hitBase.hitRight = false;
	m_hitBase.rect.top = m_pos.y + m_TexRadius;
	m_hitBase.rect.bottom = m_pos.y - m_TexRadius;
	m_hitBase.hitRight = m_pos.x + m_TexRadius;
	m_hitBase.hitLeft = m_pos.x - m_TexRadius;
	m_hitBase.radius = m_TexRadius;
}

//行列合成
void C_PlayerBase::MatCalc()
{
	m_transMat= Math::Matrix::CreateTranslation(
		m_pos.x , m_pos.y, 0);
	m_scaleMat= Math::Matrix::CreateScale(m_scaleX, 1, 1);
	m_mat = m_scaleMat * m_transMat;
}
