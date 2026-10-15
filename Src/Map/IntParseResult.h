#pragma once
struct IntParseResult
{
public:
	bool IsSuccess;
	int Number;
	std::string Value;
public:
	static IntParseResult Success(int number, std::string value)
	{
		return IntParseResult() = { true,number,value };
	}

	static IntParseResult Failure(std::string value)
	{
		return IntParseResult() = { false,0,value };
	}
};