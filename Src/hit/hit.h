#pragma once
#include"Constants/Constants.h"
#include"HitBase.h"

class C_Map;
class C_Hit
{
public:
	C_Hit() = default;
	~C_Hit() = default;

	void MapHitCheck(HitBase* a_pHitBase);

	void SetMapClass(C_Map* a_pMap) { m_pMap = a_pMap; }

	RECT CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos);
private:
	C_Map* m_pMap;
};