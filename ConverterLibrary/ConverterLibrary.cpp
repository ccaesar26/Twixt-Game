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
		else
		{
			return "Red";
		}
	}
	else
	{
		if (color == 0)
		{
			return "black";
		}
		else
		{
			return "red";
		}
	}
}