#include"Camera.h"
#include"Constants/Constants.h"
#include"Pch.h"

void C_Camera::Update()
{
	m_range = {
			{  m_pMap->GetWidth() * PTEX_SIZE, 0},	//ç≈ëÂ
			{ -PTEX_RADIUS, (TOP_EDGE - ((m_pMap->GetHeight() - 1) * PTEX_SIZE) - PTEX_RADIUS) - BOTTOM_EDGE}	//ç≈è≠
	};

	m_pos.x += int(((m_target - m_pos) * CAMERA_SPEED).x);
	m_pos.y += int(((m_target - m_pos) * CAMERA_SPEED).y);
	m_pos.x = Clamp(m_pos.x, m_range.first.x, m_range.second.x);
	m_pos.y = Clamp(m_pos.y, m_range.first.y, m_range.second.y);
}

void C_Camera::Init(C_Map* a_pMap)
{
	m_pMap = a_pMap;

	m_pos = { 0,0 };
	m_target = { 0,0 };
	m_range = {
		{  m_pMap->GetWidth() * PTEX_SIZE, 0 },	//ç≈ëÂ
		{ -PTEX_RADIUS, (TOP_EDGE - ((m_pMap->GetHeight() - 1) * PTEX_SIZE) - PTEX_RADIUS) - BOTTOM_EDGE }	//ç≈è≠
	};
}
