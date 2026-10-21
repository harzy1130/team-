#include"Hit.h"
#include"Map/Map.h"
#include"Pch.h"

void C_Hit::MapHitCheck(HitBase* a_pHitBase)
{
	std::pair<int,int> chip;
	int start_w, end_w, start_h, end_h;
	chip = m_pMap->GetIndex(a_pHitBase->GetPos());
	start_w = Clamp(chip.first - 1, m_pMap->GetIdxMaxMin().first, 0);
	end_w = Clamp(chip.first + 2, m_pMap->GetIdxMaxMin().first, 0);
	start_h = Clamp(chip.second - 1, m_pMap->GetIdxMaxMin().second, 0);
	end_h = Clamp(chip.second + 2, m_pMap->GetIdxMaxMin().second, 0);

	a_pHitBase->ResetHitRect();
	//引数のキャラベースから現在と未来の矩形を取得
	RECT now_Rect = a_pHitBase->GetNowRect();
	RECT fut_Rect = a_pHitBase->GetFutureRect();
	//for (int h = 0;h <= m_pMap->GetIdxMaxMin().second;h++) 
	for(int h = start_h;h <= end_h;h++)
	{
		//for (int w = 0;w <= m_pMap->GetIdxMaxMin().first;w++) 
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

RECT C_Hit::CreateRectPos(float a_halfWidth, float a_halfHeight, Math::Vector2 a_pos)
{
	RECT rect;
	rect.left = a_pos.x - a_halfWidth;
	rect.top = a_pos.y + a_halfHeight;
	rect.right = a_pos.x + a_halfWidth;
	rect.bottom = a_pos.y - a_halfHeight;
	return rect;
}
