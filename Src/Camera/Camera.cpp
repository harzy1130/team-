#include"Camera.h"
#include"Constans/Constants.h"

void C_Camera::Update()
{
	m_pos += (m_target - m_pos) * CAMERA_SPEED;
}

void C_Camera::Init()
{
	m_pos = { 0,0 };
	m_target={0,0};
}
