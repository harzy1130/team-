#include "Pch.h"

float Clamp(float a_value, float a_max, float a_min)
{
	if (a_max <= a_min)return a_value;
	a_value = a_max < a_value ? a_max : a_value;
	a_value = a_min > a_value ? a_min : a_value;
	return a_value;
}
