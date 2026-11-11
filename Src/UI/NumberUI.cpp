#include "NumberUI.h"
#include "Camera/Camera.h"
#include"Pch.h"

void C_NumberUI::Init(C_Camera* a_pCamera)
{
	m_pCamera = a_pCamera;
	m_number = 99999;
	m_pos = { 0,0 };
	m_size = { 1.0f, 1.0f };
	m_angle = 0;
	m_color = { 1,0,0,1 };
	m_tenGap = { 0,0 };
	m_oneGap = { 0,0 };
	m_lifespan = 120;
	m_drawFlg = false;
	m_minus = false;
	m_number = Clamp(m_number, 9999, 1);
	m_thousand = m_number / 1000;
	m_hundred = (m_number - m_thousand * 1000) / 100;
	m_ten = (m_number - m_thousand * 1000 - m_hundred * 100) / 10;
	m_one = (m_number - m_thousand * 1000 - m_hundred * 100 - m_ten * 10) / 1;
}

void C_NumberUI::Update()
{
	m_drawPos = m_pos - m_pCamera->GetPos();

	m_lifespanTimer += 1;

	if (m_lifespanTimer >= m_lifespan)
	{
		//寿命が尽きた場合の処理
		m_drawFlg = false;
	}

	//数字の間隔計算
	//千の位がある場合
	if (m_thousand > 0) {
		m_thousandGap = { 0,0 };
		m_hundredGap = { cosf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * m_size.y };
		m_tenGap = { cosf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 2 * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 2 * m_size.y };
		m_oneGap = { cosf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 3 * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 3 * m_size.y };
	}

	//百の位からの場合
	if (m_thousand <= 0 && m_hundred > 0) {
		m_hundredGap = { 0,0 };
		m_tenGap = { cosf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * m_size.y };
		m_oneGap = { cosf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 2 * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle)) * m_numbersTexWidth * 2 * m_size.y };
	}

	//十の位からの場合
	if(m_thousand <= 0 && m_hundred <= 0 && m_ten > 0)
	{
		m_tenGap = { 0,0 };
		m_oneGap = { cosf(DirectX::XMConvertToRadians(m_angle))*m_numbersTexWidth * m_size.x,sinf(DirectX::XMConvertToRadians(m_angle))*m_numbersTexWidth * m_size.y };
	}
	
	//一の位からの場合
	if (m_thousand <= 0 && m_hundred <= 0 && m_ten <= 0 && m_one > 0)
	{
		m_oneGap = { 0,0 };
	}

	//マイナスの場合
	if(m_minus)
	{
		m_thousandGap += { cosf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.x, sinf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.y };
		m_hundredGap += { cosf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.x, sinf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.y };
		m_tenGap += { cosf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.x, sinf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.y };
		m_oneGap += { cosf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.x, sinf(DirectX::XMConvertToRadians(m_angle))* m_numbersTexWidth* m_size.y };
	}

	//行列合成
	//マイナス(記号用)
	m_transMat = Math::Matrix::CreateTranslation(
		m_drawPos.x, m_drawPos.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_size.x, m_size.y, 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_minusMat = m_scaleMat * m_rotMat * m_transMat;

	//千の位 
	m_transMat = Math::Matrix::CreateTranslation(
		m_drawPos.x + m_thousandGap.x, m_drawPos.y + m_thousandGap.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_size.x, m_size.y, 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_thousandMat = m_scaleMat * m_rotMat * m_transMat;

	//百の位
	m_transMat = Math::Matrix::CreateTranslation(
		m_drawPos.x + m_hundredGap.x, m_drawPos.y + m_hundredGap.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_size.x, m_size.y, 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_hundredMat = m_scaleMat * m_rotMat * m_transMat;

	//十の位
	m_transMat = Math::Matrix::CreateTranslation(
		m_drawPos.x + m_tenGap.x, m_drawPos.y + m_tenGap.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_size.x, m_size.y, 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_tenMat = m_scaleMat * m_rotMat * m_transMat;

	//一の位
	m_transMat = Math::Matrix::CreateTranslation(
		m_drawPos.x + m_oneGap.x, m_drawPos.y + m_oneGap.y, 0);
	m_scaleMat = Math::Matrix::CreateScale(m_size.x, m_size.y, 1);
	m_rotMat = Math::Matrix::CreateRotationZ(DirectX::XMConvertToRadians(m_angle));
	m_oneMat = m_scaleMat * m_rotMat * m_transMat;
}

void C_NumberUI::Draw()
{
	if (!m_drawFlg)return;
	if (m_minus)
	{
		SHADER.m_spriteShader.SetMatrix(m_minusMat);
		Math::Rectangle srcRect = { 0,34,28,34 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(m_pNumberTex, 0, 0, &srcRect, &m_color);
		SHADER.m_spriteShader.DrawTex(m_pFrameTex, 0, 0, &srcRect, &m_frameColor);
	}

	if(m_thousand > 0)
	{
		SHADER.m_spriteShader.SetMatrix(m_thousandMat);
		Math::Rectangle srcRect = { 28 * (m_thousand),0,28,34 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(m_pNumberTex, 0, 0, &srcRect, &m_color);
		SHADER.m_spriteShader.DrawTex(m_pFrameTex, 0, 0, &srcRect, &m_frameColor);
	}

	if (m_thousand > 0||m_hundred > 0)
	{
		SHADER.m_spriteShader.SetMatrix(m_hundredMat);
		Math::Rectangle srcRect = { 28 * (m_hundred),0,28,34 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(m_pNumberTex, 0, 0, &srcRect, &m_color);
		SHADER.m_spriteShader.DrawTex(m_pFrameTex, 0, 0, &srcRect, &m_frameColor);
	}
	if (m_thousand > 0 || m_hundred > 0 || m_ten > 0)
	{
		SHADER.m_spriteShader.SetMatrix(m_tenMat);
		Math::Rectangle srcRect = { 28 * (m_ten),0,28,34 };	//テクスチャ座標
		SHADER.m_spriteShader.DrawTex(m_pNumberTex, 0, 0, &srcRect, &m_color);
		SHADER.m_spriteShader.DrawTex(m_pFrameTex, 0, 0, &srcRect, &m_frameColor);
	}
	SHADER.m_spriteShader.SetMatrix(m_oneMat);
	Math::Rectangle srcRect = { 28 * (m_one),0,28,34 };	//テクスチャ座標
	SHADER.m_spriteShader.DrawTex(m_pNumberTex, 0, 0, &srcRect, &m_color);
	SHADER.m_spriteShader.DrawTex(m_pFrameTex, 0, 0, &srcRect, &m_frameColor);
}

void C_NumberUI::Release()
{

}

void C_NumberUI::MoveDamage()
{
	if (!m_drawFlg)return;
	
	m_moveSpeed.x -= 0.1f;
	m_moveSpeed.y -= 0.1f;
	m_moveAngle = (m_lifespanTimer / m_lifespan * 180.0f)+90;
	if(m_lifespanTimer > m_lifespan/2)
	{
		m_alpha -= m_alphaSpeed*2;
	}
	m_color = { m_color.R(), m_color.G(), m_color.B(), m_alpha };
	m_frameColor = { m_frameColor.R(), m_frameColor.G(), m_frameColor.B(), m_alpha };
	if (m_moveAngle >= 360.0f) 
	{
		m_moveAngle = 0.0f;
	}
	if(m_movingDirectionX)
	{
		m_pos.x -= cosf(DirectX::XMConvertToRadians(m_moveAngle)) * m_moveSpeed.x;
	}
	else
	{
		m_pos.x += cosf(DirectX::XMConvertToRadians(m_moveAngle)) * m_moveSpeed.x;
	}
	m_pos.y += sinf(DirectX::XMConvertToRadians(m_moveAngle)) * m_moveSpeed.y;

}

void C_NumberUI::MoveEffectDamage()
{
	if (!m_drawFlg)return;
	m_moveSpeed.y -= 0.1f;
	m_moveAngle = (m_lifespanTimer / m_lifespan * 180.0f) + 90;
	if (m_lifespanTimer > m_lifespan / 2)
	{
		m_alpha -= m_alphaSpeed * 2;
	}
	m_color = {m_color.R(), m_color.G(), m_color.B(), m_alpha };
	m_frameColor = { m_frameColor.R(), m_frameColor.G(), m_frameColor.B(), m_alpha };
	m_pos.y += sinf(DirectX::XMConvertToRadians(m_moveAngle)) * m_moveSpeed.y;
}

void C_NumberUI::Generate(int a_number, Math::Vector2 a_pos,Math::Vector2 a_moveSpeed, Math::Vector2 a_size,float a_angle, Math::Color a_color, Math::Color a_frameColor, float a_lifespan,bool a_movingDirectionX)
{
	if (a_number < 0)
	{
		m_minus = true;
		a_number = abs(a_number);
	}
	else
	{
		m_minus = false;
	}
	m_drawFlg = true;
	m_number = Clamp(a_number, 9999, 1);
	m_thousand = m_number / 1000;
	m_hundred = (m_number - m_thousand * 1000) / 100;
	m_ten = (m_number - m_thousand * 1000 - m_hundred * 100) / 10;
	m_one = (m_number - m_thousand * 1000 - m_hundred * 100 - m_ten * 10) / 1;
	m_pos = a_pos;
	m_size = a_size;
	m_angle = a_angle;
	m_color = a_color;
	m_frameColor = a_frameColor;
	m_lifespan = a_lifespan;
	m_lifespanTimer = 0;
	m_moveSpeed = a_moveSpeed;
	m_movingDirectionX = a_movingDirectionX;
	m_alpha = 1.0f;
	m_alphaSpeed = 1.0f / m_lifespan;
}
