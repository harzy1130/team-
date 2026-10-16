#include"Hit.h"
#include"Map/Map.h"

//void C_Hit::HitCheck(HitBase* a_pHitBase)
//{
//	//引数のキャラベースから現在と未来の矩形を取得
//	RECT now_Rect = a_pHitBase->GetNowRect();
//	RECT fut_Rect = a_pHitBase->GetFutureRect();
//	//Math::Vector2 raw_move = a_pHitBase->GetMove();
//	//Math::Vector2 move = {0,0};
//	//while (raw_move.y >= 0) {
//	for (int h = 0;h < m_pMap->GetIdxMaxMin().second;h++) {
//		for (int w = 0;w < m_pMap->GetIdxMaxMin().first;w++) {
//			//当たらい判定がない場合はスキップ
//			//if (!MAP.GetCollisionFlg(h, w))continue;
//			//マップチップの矩形を生成
//			RECT chip_Rect;
//			Math::Vector2 chipPos = m_pMap->GetPosition(h,w);
//			chip_Rect.top = chipPos.y + CHIP_RADIUS / 2;
//			chip_Rect.bottom = chipPos.y - CHIP_RADIUS / 2;
//			chip_Rect.right = chipPos.x + CHIP_RADIUS / 2;
//			chip_Rect.left = chipPos.x - CHIP_RADIUS / 2;
//
//			if (now_Rect.top > chip_Rect.bottom &&
//				now_Rect.bottom < chip_Rect.top) {
//				//右の判定
//				if (fut_Rect.right > chip_Rect.right &&
//					fut_Rect.left < chip_Rect.right) {
//					a_pHitBase->SetPos(chip_Rect.right - a_pHitBase->GetRect().left, a_pHitBase->GetPos().y);
//					a_pHitBase->SetMove(0, a_pHitBase->GetMove().y);
//				}
//				//左の判定
//				else if (fut_Rect.right > chip_Rect.left &&
//					fut_Rect.left < chip_Rect.left) {
//					a_pHitBase->SetPos(chip_Rect.left - a_pHitBase->GetRect().right, a_pHitBase->GetPos().y);
//					a_pHitBase->SetMove(0, a_pHitBase->GetMove().y);
//				}
//			}
//			else if (now_Rect.right > chip_Rect.left &&
//				now_Rect.left < chip_Rect.right) {
//				//上の判定
//				if (fut_Rect.top > chip_Rect.top &&
//					fut_Rect.bottom < chip_Rect.top) {
//					a_pHitBase->SetPos(a_pHitBase->GetPos().x, chip_Rect.top - a_pHitBase->GetRect().bottom);
//					a_pHitBase->SetMove(a_pHitBase->GetMove().x, 0);
//				}
//				//下の判定
//				if (fut_Rect.top > chip_Rect.bottom &&
//					fut_Rect.bottom < chip_Rect.bottom) {
//					a_pHitBase->SetPos(a_pHitBase->GetPos().x, chip_Rect.bottom - a_pHitBase->GetRect().top);
//					a_pHitBase->SetMove(a_pHitBase->GetMove().x, 0);
//				}
//			}
//		}
//	}
//}
