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

void DrawFan(Math::Vector2 a_pos, float a_radius, float a_deg, float a_angle, Math::Color* a_color, bool a_bCenter)
{
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	if ((int)a_deg == 0)return;
	int lr = GetPlusMinus(a_deg);
	int deg = Clamp((int)a_deg, 360, 0);
	float centorAngle;
	if (a_bCenter)
	{
		lr = 1;
		a_angle = a_angle - a_deg / 2;
	}
	//扇形の角度が360以上なら円を描画する
	if (fabs(a_deg) >= 360)
	{
		SHADER.m_spriteShader.DrawCircle((int)a_pos.x, (int)a_pos.y, (int)a_radius, a_color);
		return;
	}
	//扇形の角度が360未満なら扇形を描画する
	for (int i = 0; i < deg; i++)
	{
		float rad = ToRadians(a_angle + (float)i * lr);
		float radNext = ToRadians(a_angle + (float)(i + 1) * lr);
		int x = (int)a_pos.x + (int)(cos(rad) * a_radius);
		int y = (int)a_pos.y + (int)(sin(rad) * a_radius);
		int xNext = (int)a_pos.x + (int)(cos(radNext) * a_radius);
		int yNext = (int)a_pos.y + (int)(sin(radNext) * a_radius);
		SHADER.m_spriteShader.DrawTriangle((int)a_pos.x, (int)a_pos.y, x, y, xNext, yNext, a_color);
	}
}

Math::Matrix CreateMat(Math::Vector2 a_pos, Math::Vector2 a_scale, float a_angle)
{
	Math::Matrix mat, sMat, rMat, tMat;
	sMat = Math::Matrix::CreateScale(a_scale.x, a_scale.y, 1);
	rMat = Math::Matrix::CreateRotationZ(ToRadians(a_angle));
	tMat = Math::Matrix::CreateTranslation(a_pos.x, a_pos.y, 0);
	mat = sMat * rMat * tMat;
	return mat;
}

void CreateFanTex(KdTexture* a_pTex, KdTexture* a_pOutTex, Math::Rectangle a_rect, float a_deg, float a_angle, bool a_bCenter)
{
	if(a_pTex == nullptr||a_pOutTex == nullptr)return;
	KdTexture mask;
	mask.CreateRenderTarget(1280, 720);
	mask.ClearRenerTarget(Math::Color{ 0, 0, 0, 0 });
	mask.SetRenderTarget();
	D3D.SetBlendState(BlendMode::NoBlend);
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	SHADER.m_spriteShader.DrawBox(0, 0, a_rect.width / 2.0f, a_rect.height / 2.0f, &Math::Color{ 0,0,0,1 });
	float radius = Math::Vector2::Distance({ 0,0 }, { (float)a_rect.width / 2.0f, (float)a_rect.height / 2.0f });
	DrawFan({ 0,0 }, radius, a_deg, a_angle, &Math::Color{ 1,1,1,1 }, a_bCenter);

	a_pOutTex->ClearRenerTarget(Math::Color{0, 0, 0, 0});
	a_pOutTex->SetRenderTarget();
	D3D.SetBlendState(BlendMode::NoBlend);
	DrawTexFromTex(a_pTex);
	D3D.SetBlendState(BlendMode::Stencil);
	DrawTexFromTex(&mask);
	D3D.SetBackBuffer();
	SHADER.m_spriteShader.SetMatrix(Math::Matrix::Identity);
	mask.Release();
}

