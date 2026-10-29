#pragma once
#include"Constants/Constants.h"
#include"HitBase.h"
#include"Enemy/EnemyManager.h"

class C_Map;
class EnemyManager;
class C_Hit
{
public:
	C_Hit() = default;
	~C_Hit() = default;

	void Init(C_Map* a_pMap, EnemyManager* a_pEnemyManager);
	void MapHitCheck(HitBase* a_pHitBase);

	bool EnemyHitCheck(HitBase* a_pHitBase);

	void SetMapClass(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetEnemyManager(EnemyManager* a_pEnemyManager) { m_pEnemyManager = a_pEnemyManager; }

	RECT CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos);
private:
	C_Map* m_pMap;
	EnemyManager* m_pEnemyManager;
};