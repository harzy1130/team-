#pragma once
#include"Constants/Constants.h"
#include"HitBase.h"

class Map;
class C_Hit
{
public:
	C_Hit() = default;
	~C_Hit() = default;

	//void HitCheck(HitBase* a_pHitBase);
	
	S_HitBase HitCheck(S_HitBase a_sHitBase);

	void SetMapData(Map* a_pMap) { m_pMap = a_pMap; }

private:
	Map* m_pMap;
};