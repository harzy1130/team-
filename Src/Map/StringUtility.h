#pragma once
#include"IntParseResult.h"

static class StringUtility
{
public:
	static IntParseResult TryIntParse(const std::string value)
	{
		try
		{
			int num = std::stoi(value);
			return IntParseResult::Success(num, value);
		}
		catch(...)
		{
			return IntParseResult::Failure(value);
		}
	}
};