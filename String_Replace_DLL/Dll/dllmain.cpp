// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"
#include "Dll.h"

#define SUBSTRING_TO_REPLACE "Dead insade"
#define SUBSTRING_TO_PASTE   "Rainbow"

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     ) {
	DWORD processPid = GetCurrentProcessId();

switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH:
		//replaceString(processPid, (char*)SUBSTRING_TO_REPLACE, (char*)SUBSTRING_TO_PASTE);
	case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

