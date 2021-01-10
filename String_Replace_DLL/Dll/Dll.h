#pragma once
#include <Windows.h>

#ifdef DLL_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif


extern "C" DLL_API void replaceString(DWORD processPid, char* substring, char* newString);