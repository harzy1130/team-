#pragma once
class C_Cursor
{
public:
	C_Cursor() {};
	~C_Cursor() {};

	Math::Vector2 GetMousePos();

private:

	Math::Vector2 m_pos;
};

