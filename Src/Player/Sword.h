#pragma once
#include"Hit/HitBase.h"

class C_Player1;
class C_UIManager;
class C_Sword : public HitBase
{
public:
	C_Sword() {};
	~C_Sword() {};

	void Init(C_Player1* player,C_Hit* a_pHit, C_Camera* a_pCamera, C_UIManager* a_pUIManager);
	void Update();
	void MatrixUpdate();
	void Draw(KdTexture*a_pTex); 

	//éQè∆ÉNÉâÉXset
	void SetPlayer(C_Player1* player) { m_pPlayer = player; }
	void SetCamera(C_Camera* a_pCamera) { m_pCamera = a_pCamera; }
	void SetHitClass(C_Hit* a_pHit) { m_pHit = a_pHit; }
	void SetUIManagerClass(C_UIManager* a_pUIManager) { m_pUIManager = a_pUIManager; }	

	void IfHitBoss(C_BossBase* a_pBoss);
	void IfHitEnemy(EnemyBase* a_pEnemy);

private:
	Math::Vector2 m_gap;
	bool m_bAtkHit = false;

	Math::Matrix m_AtkMat;
	Math::Matrix m_AtkTransMat;
	Math::Matrix m_AtkScaleMat;

	C_Player1* m_pPlayer = nullptr;
	C_Camera* m_pCamera = nullptr;
	C_Hit* m_pHit = nullptr;
	C_UIManager* m_pUIManager = nullptr;

};

