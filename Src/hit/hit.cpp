#include"Hit.h"
#include"Map/Map.h"
#include"Pch.h"

void C_Hit::Init(C_Map* a_pMap, EnemyManager* a_pEnemyManager, C_BossBase* a_pBoss)
{
	SetMapClass(a_pMap);
	SetEnemyManager(a_pEnemyManager);
	SetBossClass(a_pBoss);
}

void C_Hit::MapHitCheck(HitBase* a_pHitBase)
{
	std::pair<int,int> startChip;
	std::pair<int,int> endChip;
	int start_w, end_w, start_h, end_h;
	Math::Vector2 CorrectionPos = { float(a_pHitBase->GetRect().left + CHIP_RADIUS), float(a_pHitBase->GetRect().top - CHIP_RADIUS) };
	startChip = m_pMap->GetIndex(a_pHitBase->GetPos() + CorrectionPos);
	
	CorrectionPos = { float(a_pHitBase->GetRect().right - CHIP_RADIUS),float(a_pHitBase->GetRect().bottom + CHIP_RADIUS) };
	endChip = m_pMap->GetIndex(a_pHitBase->GetPos() + CorrectionPos);

	start_w = Clamp(startChip.first - 1, m_pMap->GetIdxMaxMin().first, 0);
	end_w = Clamp(endChip.first + 2, m_pMap->GetIdxMaxMin().first, 0);
	start_h = Clamp(startChip.second - 1, m_pMap->GetIdxMaxMin().second, 0);
	end_h = Clamp(endChip.second + 2, m_pMap->GetIdxMaxMin().second, 0);

	a_pHitBase->ResetHitRect();
	//引数のキャラベースから現在と未来の矩形を取得
	RECT now_Rect = a_pHitBase->GetNowRect();
	RECT fut_Rect = a_pHitBase->GetFutureRect();
	for(int h = start_h;h <= end_h;h++)
	{
		for(int w = start_w;w <= end_w;w++)
		{
			//当たり判定がない場合はスキップ
			if (!m_pMap->GetCollisionFlg(h, w))continue;
			//マップチップの矩形を生成
			RECT chip_Rect = CreateRectPos(CHIP_RADIUS,CHIP_RADIUS,m_pMap->GetPosition(h,w));

			if (now_Rect.top > chip_Rect.bottom &&
				now_Rect.bottom < chip_Rect.top) {
				//右の判定
				if (fut_Rect.right > chip_Rect.right &&
					fut_Rect.left < chip_Rect.right) {
					a_pHitBase->SetMapHitLeft(chip_Rect.right);
				}
				//左の判定
				else if (fut_Rect.right > chip_Rect.left &&
					fut_Rect.left < chip_Rect.left) {
					a_pHitBase->SetMapHitRight(chip_Rect.left);
				}
			}
			else if (now_Rect.right > chip_Rect.left &&
				now_Rect.left < chip_Rect.right) {
				//上の判定
				if (fut_Rect.top > chip_Rect.top &&
					fut_Rect.bottom < chip_Rect.top) {
					a_pHitBase->SetMapHitBottom(chip_Rect.top);
				}
				//下の判定
				if (fut_Rect.top > chip_Rect.bottom &&
					fut_Rect.bottom < chip_Rect.bottom) {
					a_pHitBase->SetMapHitTop(chip_Rect.bottom);
				}
			}
		}
	}
	return;
}

bool C_Hit::EnemyHitCheck(HitBase* a_pHitBase)
{
	bool bHit = false;
	for (int en = 0;en < m_pEnemyManager->GetCount();en++) 
	{
		EnemyBase* enemy = m_pEnemyManager->GetEnemy(en);
		bool alreadyHit = a_pHitBase->GetAlreadyHit(enemy);
		if (enemy == nullptr || !enemy->GetAlive() || alreadyHit)continue;
		if(CircleHit(enemy->GetPos(),enemy->GetRadius(),a_pHitBase->GetPos(),a_pHitBase->GetRadius()))
		{
			a_pHitBase->SetAlreadyHit(enemy);
			a_pHitBase->IfHitEnemy(enemy);
		}
	}
	return bHit;
}

bool C_Hit::BossHitCheck(HitBase* a_pHitBase)
{
	bool alreadyHit = a_pHitBase->GetAlreadyHit(m_pBoss);
	if (!m_pBoss->GetAlive() || m_pBoss == nullptr || alreadyHit)return false;
	if (RectCirclrHit(m_pBoss->GetPos(), m_pBoss->GetRect(), a_pHitBase->GetPos(), a_pHitBase->GetRadius())) 
	{
		a_pHitBase->SetAlreadyHit(m_pBoss);
		a_pHitBase->IfHitBoss(m_pBoss);
	}
	return false;
}

bool C_Hit::CircleHit(Math::Vector2 a_pos1, float a_radius1, Math::Vector2 a_pos2, float a_radius2)
{
	float compareDist = a_radius1 + a_radius2;
	if (compareDist > GetDist(a_pos1, a_pos2))return true;
	return false;
}

bool C_Hit::RectCirclrHit(Math::Vector2 a_rectPos, RECT a_rect, Math::Vector2 a_circlePos, float a_circleRadius)
{
 	RECT rect = CreateRectPos(a_rect.right + a_circleRadius, a_rect.top, a_rectPos);
	if (RectInPos(rect, a_circlePos))return true;
	rect = CreateRectPos(a_rect.right, a_rect.top + a_circleRadius, a_rectPos);
	if (RectInPos(rect, a_circlePos))return true;
	Math::Vector2 corner;
	corner = { a_rectPos.x + a_rect.right,a_rectPos.y + a_rect.top };
	if (CircleHit(corner,a_circleRadius,a_circlePos,0))return true;
	corner = { a_rectPos.x + a_rect.right,a_rectPos.y + a_rect.bottom };
	if (CircleHit(corner,a_circleRadius,a_circlePos,0))return true;
	corner = { a_rectPos.x + a_rect.left,a_rectPos.y + a_rect.top };
	if (CircleHit(corner,a_circleRadius,a_circlePos,0))return true;
	corner = { a_rectPos.x + a_rect.left,a_rectPos.y + a_rect.bottom };
	if (CircleHit(corner,a_circleRadius,a_circlePos,0))return true;
	return false;
}

bool C_Hit::RectHit(Math::Vector2 a_pos1, RECT a_rect1, Math::Vector2 a_pos2, RECT a_rect2)
{
	RECT rect1 = CreateRectPos(a_rect1.right, a_rect1.top, a_pos1);
	RECT rect2 = CreateRectPos(a_rect2.right, a_rect2.top, a_pos2);
	const float left = std::max(rect1.left, rect2.left);
	const float right = std::min(rect1.right, rect2.right);
	const float bottom = std::max(rect1.bottom, rect2.bottom);
	const float top = std::min(rect1.top, rect2.top);
	return left < right && bottom < top;
}

RECT C_Hit::CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos)
{
	RECT rect;
	rect.left = a_pos.x - a_halfWidth;
	rect.top = a_pos.y + a_halfHeight;
	rect.right = a_pos.x + a_halfWidth;
	rect.bottom = a_pos.y - a_halfHeight;
	return rect;
}

bool C_Hit::RectInPos(RECT a_rect, Math::Vector2 a_pos)
{
	if (a_rect.left < a_pos.x && a_pos.x < a_rect.right &&
		a_rect.bottom < a_pos.y && a_pos.y < a_rect.top)return true;
	return false;
}
