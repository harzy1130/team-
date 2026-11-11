#include "PlayerBase.h"
#include"Cursor/Cursor.h"
#include <cmath>

void C_PlayerBase::Init(C_Hit* a_pHit, C_Camera* a_pCamera, C_Cursor* a_pCursor, C_UIManager* a_pUIManager)
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
	//m_MaxHp = 100;
	//m_Hp = m_MaxHp;
	m_playerStatus.def = 50;
	m_playerStatus.atk = 80;
	m_atkSpeed = 1.3;
	//特殊ステータス
	m_playerStatus.lifStl = 0;
	m_playerStatus.msUP = 0;
	m_playerStatus.hpRegene = 0;
	m_playerStatus.crtDmgP = 150;
	m_playerStatus.crtRate = 0;
	m_playerStatus.skillHaste=50;
	m_playerStatus.increasedASP = 0;
	m_status = { 0,0,0,0,0,0,0,0 };
	m_gearStatus = { 0,0,0,0,0,0,0,0 };
	//通常攻撃
	InitAtk();
	m_skill.SetIconTexture(m_pSkillIconTex);
	m_skill.Init();
	SetHitClass(a_pHit);
	SetCameraClass(a_pCamera);
	SetRect(PTEX_RADIUS, PTEX_RADIUS);
	SetRadius(PTEX_RADIUS);
	//状態異常
	m_effect.InitPlayer(this, a_pUIManager);
}

void C_PlayerBase::Update()
{
    if (!m_bAlive) { m_move = { 0, 0 }; return; }
    if (m_damageInvincibleFrames > 0) --m_damageInvincibleFrames;
	m_move.x = 0.0f;
	m_move.y -= GRAVITY;

	m_skill.Update();
	GearUpdate();
	PlayerInput();
	UpdateAtk();
	m_effect.UpdatePlayer();
	if (m_pCursor->GetMousePos().x > 0) {
		m_scale = { 1,1 };
	}
	else
	{
		m_scale = { -1,1 };
	}
	
	m_move += m_skillMove;
	//当たり判定
	m_pHit->MapHitCheck(this);
	m_bJump = !m_bHitRect.bottom;
	m_pHit->EnemyHitCheck(this);
	m_pHit->BossHitCheck(this);

	//座標確定
	m_pos += m_move;
	m_AtkPos = { m_pos.x + (m_scale.x * 55),m_pos.y};

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

void C_PlayerBase::GearUpdate()
{
	m_gearStatus = { 0,0,0,0,0,0,0,0 };

	if (m_pGearA == nullptr) goto EndA;
	if(m_pGearA->PassiveTiming()) m_pGearA->OncePassiveGear();
	if(GetAsyncKeyState('Q') & 0x8000)m_pGearA->OnceActiveGear();
	m_pGearA->WhileActiveGear();
	m_pGearA->WhilePassiveGear();
	m_pGearA->UpdateCD();
EndA:;

	if(m_pGearB == nullptr) goto EndB;
	if(m_pGearB->PassiveTiming()) m_pGearB->OncePassiveGear();
	if (GetAsyncKeyState('E') & 0x8000)m_pGearB->OnceActiveGear();
	m_pGearB->WhileActiveGear();
	m_pGearB->WhilePassiveGear();
	m_pGearB->UpdateCD();
EndB:;
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

	m_hpRegeneBuffer += m_status.hpRegene;
	if(m_hpRegeneBuffer >= 1.0f)
	{
		Heal((int)m_hpRegeneBuffer);
		m_hpRegeneBuffer -= (int)m_hpRegeneBuffer;
	}
	m_HPRegeneCD = 60;
	return;
}

//クリティカルかどうか判定
bool C_PlayerBase::isCritical()
{
	if (rand() % 100 + 1 <= m_status.crtRate) {
		return true;
	}
	else
	{
		return false;
	}
}

//クリティカル
float C_PlayerBase::CrtDmgCalc()
{
	if (rand() % 100 + 1 <= m_status.crtRate) {
		return m_status.atk * (m_status.crtDmgP / 100);
	}
	else
	{
		return m_status.atk;
	}
}

//攻撃ダメージ計算
float C_PlayerBase::DmageCalc(bool a_isCritical)
{
	if (a_isCritical)
	{
		return m_status.atk* (m_status.crtDmgP / 100);
	}
	return m_status.atk;;
}

//スピード加速
float C_PlayerBase::MoveSpeedCalc(float a_speed, float a_MS)
{
	return a_speed*(a_MS/100);
}

//攻撃速度計算
float C_PlayerBase::AtkSpeedCalc()
{
	return 60/(m_atkSpeed + (m_atkSpeed*m_LevelUpAtkSpeedP * (m_Level - 1))) + (m_atkSpeed * m_status.increasedASP/100);
}

//キー操作
void C_PlayerBase::PlayerInput()
{
	if (!m_bAlive)return;
	if ((GetAsyncKeyState(VK_RIGHT) & 0x8000)||(GetAsyncKeyState('D') & 0x8000))
	{
		if (m_moveMode != E_MoveMode::attacking&&m_moveMode != E_MoveMode::defense)
		{
			m_move.x += m_moveSpeed;
			m_moveMode = E_MoveMode::run;
			m_scale = { 1,1 };
		}

	}
	if ((GetAsyncKeyState(VK_LEFT) & 0x8000) || (GetAsyncKeyState('A') & 0x8000))
	{
		if (m_moveMode != E_MoveMode::attacking&&m_moveMode != E_MoveMode::defense)
		{
			m_move.x -= m_moveSpeed;
			m_moveMode = E_MoveMode::run;
			m_scale = { -1,1 };
		}

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
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000&& m_AttackDelay==0)
	{
		// 左クリック時の処理
		if (m_moveMode == E_MoveMode::defense)return;
		PlayerAtk();
		m_moveMode = E_MoveMode::attacking;
	}
	if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
	{
		// 右クリック時の処理
		if (m_moveMode == E_MoveMode::attacking)return;
		PlayerDef();
		m_moveMode = E_MoveMode::defense;
	}
}

//行列合成
void C_PlayerBase::CalcMat()
{
	m_draw = m_pos - m_pCamera->GetPos();
	m_transMat= Math::Matrix::CreateTranslation(
		m_draw.x, m_draw.y, 0);
	m_scaleMat= Math::Matrix::CreateScale(m_scale.x, m_scale.y, 1);
	m_mat = m_scaleMat * m_transMat;

	m_AtkTransMat = Math::Matrix::CreateTranslation(
		m_AtkPos.x - m_pCamera->GetPos().x, m_AtkPos.y - m_pCamera->GetPos().y, 0);
	m_AtkScaleMat = Math::Matrix::CreateScale(m_scale.x, m_scale.y, 1);
	m_AtkMat = m_AtkScaleMat * m_AtkTransMat;
}

void C_PlayerBase::MatrixUpdate()
{
	m_draw = m_pos - m_pCamera->GetPos();
	m_transMat = Math::Matrix::CreateTranslation(
		m_draw.x, m_draw.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_scale.x * (1 / 5.0f), m_scale.y * (1 / 5.0f), 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_mat = m_scaleMat * m_rotMat * m_transMat;
}

//アニメーション
void C_PlayerBase::AnimationUpdate()
{
	m_moveAnimCount += 0.2f;
	//攻撃は別のカウントで管理する
	if (m_moveMode == E_MoveMode::attacking)
	{
		m_moveAnimCount = m_atkAnimCount;
	}
	if (m_move.x == 0 && m_moveMode != E_MoveMode::attacking && m_moveMode != E_MoveMode::defense)
	{
		m_moveMode = E_MoveMode::stand;
	}
	if (m_move.x == 0 && m_moveMode == E_MoveMode::attacking && m_bAtk == false)
	{
		//m_moveMode = E_MoveMode::stand;
	}
	if (m_moveAnimCount >= 8 && m_moveMode == E_MoveMode::run) m_moveAnimCount = 0;
	if (m_moveAnimCount >= 3 && m_moveMode == E_MoveMode::stand) m_moveAnimCount = 0;
	//if (m_moveAnimCount >= 4 && m_moveMode == E_MoveMode::attacking) m_moveAnimCount = 0;
	if (m_moveAnimCount >= 4 && m_moveMode == E_MoveMode::defense && !(GetAsyncKeyState(VK_RBUTTON) & 0x8000))
	{
		m_moveMode = E_MoveMode::stand;
		m_moveAnimCount = 0;
	}
	else if (m_moveAnimCount >= 4 &&m_moveMode == E_MoveMode::defense)
	{
		m_moveAnimCount = 3.8;
	}
}

//通常攻撃
void C_PlayerBase::PlayerAtk()
{
	if (!m_bAlive || m_AttackDelay != 0)return;
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
		
	}
	if (!m_bAtk)return;
	m_atkAnimCount = (4 - m_AttackDelay / AtkSpeedCalc() * 4) * 2.0f;
	if (m_atkAnimCount >= 4)
	{
		m_atkAnimCount = 0;
		m_bAtk = false;
		m_moveMode = E_MoveMode::run;
	}

}

void C_PlayerBase::DrawAtk()
{
	if (!m_bAtk)return;
	SHADER.m_spriteShader.SetMatrix(m_AtkMat);
	SHADER.m_spriteShader.DrawTex(
		m_pAtkTex, Math::Rectangle(0, 0, 64, 64), 1.0f);

}

//防御処理
void C_PlayerBase::PlayerDef()
{
	m_bDef = true;
}

void C_PlayerBase::UpdateDef()
{
	m_bDef = false;
}

void C_PlayerBase::ExpUpdate()
{
	if (m_Exp >= m_NextLvExp+m_ExpUp*(m_Level-1))
	{
		m_Level++;
		m_Exp -= m_NextLvExp;
		//ステータス上昇
		m_Hp = m_LevelUpHp;
	}
}

void C_PlayerBase::StatusUpdate()
{
	CalcGearStatus();
	m_status = m_playerStatus + m_gearStatus;
	//HPリジェネ
	HPRegeneCalc();

	m_playerStatus.maxHp = m_baseHp + (m_Level - 1) * m_LevelUpHp;
	m_playerStatus.atk = m_baseAtk + (m_Level - 1) * m_LevelUpAtk;
	m_playerStatus.def = m_baseDef + (m_Level - 1) * m_LevelUpDef;
}

void C_PlayerBase::CalcGearStatus()
{
	m_gearStatus = { 0,0,0,0,0,0,0,0 };
	if (m_pGearA == nullptr) goto EndA;
	m_gearStatus += m_pGearA->GetGearStatus();
EndA:;
	if (m_pGearB == nullptr) goto EndB;
	m_gearStatus += m_pGearB->GetGearStatus();
EndB:;
}

bool C_PlayerBase::TakeDamage(float rawDamage)
{
    if (!m_bAlive || m_Hp <= 0 || m_damageInvincibleFrames > 0 || !std::isfinite(rawDamage) || !(rawDamage > 0) || !std::isfinite(m_status.def)) return false;
    const float damage = DefCalc(rawDamage, std::max(0.0f, m_status.def), 0.0f);
    m_Hp = std::max(0, m_Hp - (int)damage);
    m_damageInvincibleFrames = 30;
    if (m_Hp == 0)
    {
        m_bAlive = false;
        m_move = { 0.0f, 0.0f };
        m_skillMove = { 0.0f, 0.0f };
    }
    return true;
}

bool C_PlayerBase::Heal(float a_heal)
{
	if (!m_bAlive || m_Hp <= 0 || !std::isfinite(a_heal) || !(a_heal > 0)) return false;
	m_Hp = std::min(m_status.maxHp, m_Hp + (int)a_heal);
	if(m_Hp == m_status.maxHp)
	{
		return true;
	}
	return false;
}
