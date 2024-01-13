#include "pch.h"
#include "ConverterLibrary.h"

const char* ColorToString(const int color, const bool capitalize)
{
	if (capitalize)
	{
		if (color == 0)
		{
			return "Black";
		}
		return "Red";
	}
	if (color == 0)
	{
		return "black";
	}
	return "red";
}

const char* GameResultToString(const int result)
{
	if (result == 0)
	{
		return "Black wins";
	}
	if (result == 1)
	{
		return "Red wins";
	}
	return "Draw";
}
