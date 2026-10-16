#include"Hit.h"
#include"Map/Map.h"

//void C_Hit::HitCheck(HitBase* a_pHitBase)
//{
//	//引数のキャラベースから現在と未来の矩形を取得
//	RECT now_Rect = {
//			a_pHitBase->GetPos().x - 32,
//			a_pHitBase->GetPos().y + 32,
//			a_pHitBase->GetPos().x + 32,
//			a_pHitBase->GetPos().y - 32
//					};
//	RECT fut_Rect = {
//			a_pHitBase->GetFuturePos().x - 32,
//			a_pHitBase->GetFuturePos().y + 32,
//			a_pHitBase->GetFuturePos().x + 32,
//			a_pHitBase->GetFuturePos().y - 32
//					};
//	//Math::Vector2 raw_move = a_pHitBase->GetMove();
//	//Math::Vector2 move = {0,0};
//	//while (raw_move.y >= 0) {
//	for (int h = 0;h < m_pMap->GetIdxMaxMin().second;h++) 
//	{
//		for (int w = 0;w < m_pMap->GetIdxMaxMin().first;w++) 
//		{
//			//当たらい判定がない場合はスキップ
//			if (!m_pMap->GetCollisionFlg(h, w))continue;
//			//マップチップの矩形を生成
//			RECT chip_Rect;
//			Math::Vector2 chipPos = m_pMap->GetPosition(h, w);
//			chip_Rect.top = chipPos.y + CHIP_RADIUS;
//			chip_Rect.bottom = chipPos.y - CHIP_RADIUS;
//			chip_Rect.right = chipPos.x + CHIP_RADIUS;
//			chip_Rect.left = chipPos.x - CHIP_RADIUS;
//
//			if (now_Rect.top > chip_Rect.bottom &&
//				now_Rect.bottom < chip_Rect.top) {
//				//右の判定
//				if (fut_Rect.right > chip_Rect.right &&
//					fut_Rect.left < chip_Rect.right) {
//					a_pHitBase->SetPos(chip_Rect.right + 32, a_pHitBase->GetPos().y);
//					a_pHitBase->SetMove(0, a_pHitBase->GetMove().y);
//					return;
//				}
//				//左の判定
//				else if (fut_Rect.right > chip_Rect.left &&
//					fut_Rect.left < chip_Rect.left) {
//					a_pHitBase->SetPos(chip_Rect.left - 32, a_pHitBase->GetPos().y);
//					a_pHitBase->SetMove(0, a_pHitBase->GetMove().y);
//					return;
//				}
//			}
//			else if (now_Rect.right > chip_Rect.left &&
//				now_Rect.left < chip_Rect.right) {
//				//上の判定
//				if (fut_Rect.top > chip_Rect.top &&
//					fut_Rect.bottom < chip_Rect.top) {
//					a_pHitBase->SetPos(a_pHitBase->GetPos().x, chip_Rect.top + 32);
//					a_pHitBase->SetMove(a_pHitBase->GetMove().x, 0);
//					return;
//				}
//				//下の判定
//				if (fut_Rect.top > chip_Rect.bottom &&
//					fut_Rect.bottom < chip_Rect.bottom) {
//					a_pHitBase->SetPos(a_pHitBase->GetPos().x, chip_Rect.bottom - 32);
//					a_pHitBase->SetMove(a_pHitBase->GetMove().x, 0);
//					return;
//				}
//			}
//		}
//	}
//}

S_HitBase C_Hit::HitCheck(S_HitBase a_sHitBase)
{
	S_HitBase hitBase = a_sHitBase;
	RECT now_Rect;
	now_Rect.left    =	a_sHitBase.pos.x + a_sHitBase.rect.left;
	now_Rect.top     =	a_sHitBase.pos.y + a_sHitBase.rect.top;
	now_Rect.right   =	a_sHitBase.pos.x + a_sHitBase.rect.right;
	now_Rect.bottom	 =	a_sHitBase.pos.y + a_sHitBase.rect.bottom;
	RECT fut_Rect;
	fut_Rect.left    =	a_sHitBase.move.x + now_Rect.left;
	fut_Rect.top     =	a_sHitBase.move.y + now_Rect.top;
	fut_Rect.right   =	a_sHitBase.move.x + now_Rect.right;
	fut_Rect.bottom  =	a_sHitBase.move.y + now_Rect.bottom;
	for (int h = 0;h < m_pMap->GetIdxMaxMin().second;h++)
	{
		for (int w = 0;w < m_pMap->GetIdxMaxMin().first;w++)
		{
			if (!m_pMap->GetCollisionFlg(h, w))continue;
			RECT chip_Rect;
			chip_Rect.left	= m_pMap->GetPosition(h, w).x - CHIP_RADIUS;
			chip_Rect.top	= m_pMap->GetPosition(h, w).y + CHIP_RADIUS;
			chip_Rect.right = m_pMap->GetPosition(h, w).x + CHIP_RADIUS;
			chip_Rect.bottom= m_pMap->GetPosition(h, w).y - CHIP_RADIUS;
			
			if (now_Rect.right > chip_Rect.left && 
				now_Rect.left < chip_Rect.right){
				
				if (fut_Rect.top > chip_Rect.top &&
					fut_Rect.bottom < chip_Rect.top) {
					hitBase.move.y = 0;
					hitBase.pos.y = chip_Rect.top + 32;
					hitBase.hitBottom = true;
				}
				else if (fut_Rect.top > chip_Rect.bottom &&
					fut_Rect.bottom < chip_Rect.bottom) {
					hitBase.move.y = 0;
					hitBase.pos.y = chip_Rect.bottom + hitBase.rect.bottom;
					hitBase.hitTop = true;
				}
			}
			else
			if (now_Rect.top > chip_Rect.bottom &&
				now_Rect.bottom < chip_Rect.top) {
				if (fut_Rect.right > chip_Rect.right &&
					fut_Rect.left < chip_Rect.right) {
					hitBase.move.x = 0;
					hitBase.pos.x = chip_Rect.right + hitBase.rect.right;
					hitBase.hitLeft = true;
				}
				if (fut_Rect.right > chip_Rect.left &&
					fut_Rect.left < chip_Rect.left) {
					hitBase.move.x = 0;
					hitBase.pos.x = chip_Rect.left + hitBase.rect.left;
					hitBase.hitRight = true;
				}
			}
		}
	}
	return hitBase;
}