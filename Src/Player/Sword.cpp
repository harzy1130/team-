#include "Sword.h"
#include "Player1.h"
#include"Boss/BossBase.h"
#include"Enemy/EnemyBase.h"
#include"UI/UIManager.h"

void C_Sword::Init(C_Player1* a_player, C_Hit* a_pHit, C_Camera* a_pCamera, C_UIManager* a_pUIManager)
{
	m_gap = { 50,0 };
	SetPlayer(a_player);
	SetHitClass(a_pHit);
	SetCamera(a_pCamera);
	SetUIManagerClass(a_pUIManager);
	m_hitRange = 60;
}

void C_Sword::Update()
{
	m_gap.x =70 * m_pPlayer->GetScaleX();
	m_pos = m_pPlayer->GetPos();
	m_pos += m_gap;
	m_draw = m_pos - m_pCamera->GetPos();
	if(m_pPlayer->GetAtkFlg()&&!m_bAtkHit)
	{
		// 攻撃時の処理
		ResetAlreadyHit();
		m_hitRange = 60;
		NewCircleObjects({0,0}, 50.0f);
		m_bAtkHit = true;
	}
	else if(m_pPlayer->GetAtkFlg() == false)
	{
		DeleteCircleObjects(GetCircleObjectsCount()-1);
		m_bAtkHit = false;
		m_hitRange = 0;
		
	}
	if(m_bAtkHit)
	{
		m_pHit->BossHitCheck(this);
		m_pHit->EnemyHitCheck(this);
		
	}
	MatrixUpdate();
}

void C_Sword::MatrixUpdate()
{
	m_AtkTransMat = Math::Matrix::CreateTranslation(
		m_draw.x, m_draw.y, 0);
	m_AtkScaleMat = Math::Matrix::CreateScale(m_pPlayer->GetScaleX()*0.8, m_scale.y*2.5, 1);
	m_AtkMat = m_AtkScaleMat * m_AtkTransMat;
}

void C_Sword::Draw(KdTexture* a_pTex)
{
	if (!m_bAtkHit)return;
	DrawHitBox();
	//SHADER.m_spriteShader.SetMatrix(m_AtkMat);
	//SHADER.m_spriteShader.DrawTex(
	//	a_pTex, Math::Rectangle(0, 0, 64, 64), 1.0f);
}

void C_Sword::IfHitBoss(C_BossBase* a_pBoss)
{
	//クリティカルかどうか判定
	bool isCritical = m_pPlayer->isCritical();

	//UIが飛ぶ方向を判定
	bool direction=a_pBoss->GetPos().x > m_pPlayer->GetPos().x;

	a_pBoss->TakeDamage((int)m_pPlayer->DmageCalc(isCritical));
	m_pUIManager->CreateDamageUI((int)m_pPlayer->DmageCalc(isCritical),(a_pBoss->GetPos()+m_pPlayer->GetPos())*0.5f, direction, isCritical);
}

void C_Sword::IfHitEnemy(EnemyBase* a_pEnemy)
{
	bool isCritical = m_pPlayer->isCritical();
	//UIが飛ぶ方向を判定
	bool direction=a_pEnemy->GetPos().x > m_pPlayer->GetPos().x;

	a_pEnemy->TakeDamage((int)m_pPlayer->DmageCalc(isCritical), m_pPlayer->GetPos().x);
	m_pUIManager->CreateDamageUI((int)m_pPlayer->DmageCalc(isCritical), (a_pEnemy->GetPos() + m_pPlayer->GetPos()) * 0.5f, direction, isCritical);
}
