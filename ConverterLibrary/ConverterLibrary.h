#pragma once

#ifdef CONVERTERLIBRARY_EXPORTS
#define CONVERTERLIBRARY_API __declspec(dllexport)
#else
#define CONVERTERLIBRARY_API __declspec(dllimport)
#endif

extern "C" CONVERTERLIBRARY_API  const char* ColorToString(const int color, const bool capitalize = false);
