#include "Cursor.h"
#include"Application/main.h"

Math::Vector2 C_Cursor::GetMousePos()
{
	POINT mouse;
	GetCursorPos(&mouse);
	ScreenToClient(APP.m_window.GetWndHandle(), &mouse);
	mouse.x -= 640;
	mouse.y -= 360;
	mouse.y *= -1;
	return Math::Vector2() = {(float)mouse.x,(float)mouse.y};
}
