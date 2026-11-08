#include "Pch.h"

float Clamp(float a_value, float a_max, float a_min)
{
	if (a_max <= a_min)return a_value;
	a_value = a_max < a_value ? a_max : a_value;
	a_value = a_min > a_value ? a_min : a_value;
	return a_value;
}

int GetPlusMinus(float a_value)
{
	if (a_value >= 0)return 1;
	return -1;
}

float GetDist(Math::Vector2 a_posA, Math::Vector2 a_posB)
{
	float a = a_posA.x - a_posB.x;
	float b = a_posA.y - a_posB.y;
	float c = sqrt(a * a + b * b);
	return c;
}

RECT CreateRect(long a_halfWidth, long a_halfHeight)
{
	RECT rect;
	a_halfWidth = std::max(0L, a_halfWidth);
	a_halfHeight = std::max(0L, a_halfHeight);
	rect = { -a_halfWidth, a_halfHeight, a_halfWidth, -a_halfHeight };
	return rect;
}

Math::Rectangle CreateRectFromTex(KdTexture* a_pTex, RECT& a_rect)
{
	if (a_pTex == nullptr)return Math::Rectangle();
	//DirectXの構造体で画像のサイズを取得。
	const auto& info = a_pTex->GetInfo();
	a_rect.left = 0;
	a_rect.top = 0;
	a_rect.right = static_cast<long>(info.Width);
	a_rect.bottom = static_cast<long>(info.Height);
	return Math::Rectangle(a_rect.left, a_rect.top, a_rect.right, a_rect.bottom);
}

void DrawTexFromTex(KdTexture* a_pTex, Math::Vector2 a_pos, Math::Vector2 a_scale,
	float a_angle, Math::Color a_color)
{
	if (a_pTex == nullptr)return;
	Math::Matrix mat, sMat, rMat, tMat;
	sMat = Math::Matrix::CreateScale(a_scale.x, a_scale.y, 1);
	rMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(a_angle));
	tMat = Math::Matrix::CreateTranslation(a_pos.x, a_pos.y, 0);
	mat = sMat * rMat * tMat;
	SHADER.m_spriteShader.SetMatrix(mat);
	SHADER.m_spriteShader.DrawTex(a_pTex, 0, 0, &CreateRectFromTex(a_pTex), &a_color);
}
