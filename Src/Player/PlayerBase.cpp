#include "PlayerBase.h"


void C_PlayerBase::Init()
{
	m_Aflg = true;
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
	m_CrtDmg = 150;
	m_CrtRate = 0;
}

void C_PlayerBase::Update()
{
	m_move.x = 0.0f;
	m_move.y -= m_Gravity;

	PlayerInput();

	//座標確定
	m_pos.x += m_move.x;
	m_pos.y += m_move.y;
	if (m_pos.y <= -300) {
		m_pos.y = -300;
		m_jumpFlg = false;
	}
	MatCalc();
}

void C_PlayerBase::Draw()
{
	if (!m_Aflg)return;
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
	m_HPRegeneCT--;
	if (m_HPRegeneCT <= 0) {
		m_HPRegeneCT = 0;
	}
	if (m_HPRegeneCT > 0)return;

	m_HP += m_HPRegene;
	m_HPRegeneCT = 60;
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
	if (!m_Aflg)return;
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		m_move.x += 8.0f;
		m_scaleX = 1;
	}
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
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

//行列合成
void C_PlayerBase::MatCalc()
{
	m_transMat= Math::Matrix::CreateTranslation(
		m_pos.x , m_pos.y, 0);
	m_scaleMat= Math::Matrix::CreateScale(m_scaleX, 1, 1);
	m_mat = m_scaleMat * m_transMat;
}
