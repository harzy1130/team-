#pragma once
#include"Constants/Constants.h"
#include"Enemy/EnemyManager.h"
#include"Boss/BossBase.h"
#include"HitBase.h"

class C_Map;
class EnemyManager;
class C_BossBase;
class C_Hit
{
public:
	C_Hit() = default;
	~C_Hit() = default;
	// 初期化関数
	void Init(C_Map* a_pMap, EnemyManager* a_pEnemyManager,C_BossBase* a_pBoss);
	void SetMapClass(C_Map* a_pMap) { m_pMap = a_pMap; }
	void SetEnemyManager(EnemyManager* a_pEnemyManager) { m_pEnemyManager = a_pEnemyManager; }
	void SetBossClass(C_BossBase* a_pBoss) { m_pBoss = a_pBoss; }

	// 引数に入れたHitBaseを持つポインタにMapへの当たり判定を持たせる。
	void MapHitCheck(HitBase* a_pHitBase);
	// 引数に入れたHitBaseを持つポインタに敵への当たり判定を持たせる。
	bool EnemyHitCheck(HitBase* a_pHitBase);
	// 引数に入れたHitBaseを持つポインタにボスへの当たり判定を持たせる。
	bool BossHitCheck(HitBase* a_pHitBase);
	// 円と円の当たり判定を行う
	bool CircleHit(Math::Vector2 a_pos1, float a_radius1,Math::Vector2 a_pos2, float a_radius2);
	// 円と矩形の当たり判定を行う
	bool RectCirclrHit(Math::Vector2 a_rectPos, RECT a_rect, Math::Vector2 a_circlePos, float a_circleRadius);
	// 矩形と矩形の当たり判定を行う。
	bool RectHit(Math::Vector2 a_pos1, RECT a_rect1, Math::Vector2 a_pos2, RECT a_rect2);
private:
	RECT CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos);
	bool RectInPos(RECT a_rect, Math::Vector2 a_pos);
	C_Map* m_pMap = nullptr;
	EnemyManager* m_pEnemyManager = nullptr;
	C_BossBase* m_pBoss = nullptr;
};