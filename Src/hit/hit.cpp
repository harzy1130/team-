#include<algorithm>
#include"Hit.h"
#include"Map/Map.h"

void C_Hit::HitCheck(HitBase* a_pHitBase)
{
	std::pair<int,int> chip;
	int start_w, end_w, start_h, end_h;
	chip = m_pMap->GetIndex(a_pHitBase->GetPos());

	a_pHitBase->ResetHitRect();
	//引数のキャラベースから現在と未来の矩形を取得
	RECT now_Rect = a_pHitBase->GetNowRect();
	RECT fut_Rect = a_pHitBase->GetFutureRect();
	for (int h = 0;h <= m_pMap->GetIdxMaxMin().second;h++) 
	{
		for (int w = 0;w <= m_pMap->GetIdxMaxMin().first;w++) 
		{
			//当たり判定がない場合はスキップ
			if (!m_pMap->GetCollisionFlg(h, w))continue;
			//マップチップの矩形を生成
			RECT chip_Rect;
			Math::Vector2 chipPos = m_pMap->GetPosition(h, w);
			chip_Rect.left = chipPos.x - CHIP_RADIUS;
			chip_Rect.top = chipPos.y + CHIP_RADIUS;
			chip_Rect.right = chipPos.x + CHIP_RADIUS;
			chip_Rect.bottom = chipPos.y - CHIP_RADIUS;

			if (now_Rect.top > chip_Rect.bottom &&
				now_Rect.bottom < chip_Rect.top) {
				//右の判定
				if (fut_Rect.right > chip_Rect.right &&
					fut_Rect.left < chip_Rect.right) {
					a_pHitBase->SetHitLeft(chip_Rect.right);
				}
				//左の判定
				else if (fut_Rect.right > chip_Rect.left &&
					fut_Rect.left < chip_Rect.left) {
					a_pHitBase->SetHitRight(chip_Rect.left);
				}
			}
			else if (now_Rect.right > chip_Rect.left &&
				now_Rect.left < chip_Rect.right) {
				//上の判定
				if (fut_Rect.top > chip_Rect.top &&
					fut_Rect.bottom < chip_Rect.top) {
					a_pHitBase->SetHitBottom(chip_Rect.top);
				}
				//下の判定
				if (fut_Rect.top > chip_Rect.bottom &&
					fut_Rect.bottom < chip_Rect.bottom) {
					a_pHitBase->SetHitTop(chip_Rect.bottom);
				}
			}
		}
	}
	return;
}