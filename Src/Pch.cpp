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
