
#include "pch.h"
#include <iostream>
#include <windows.h>
#include <psapi.h>
#include <string>
#include <tchar.h>
#include <fstream>
#include <vector>
#include <TlHelp32.h>

using namespace std;
DWORD GetProcessIdByProcessName(const char* processName);

BOOL InjectLIb(DWORD ProcID, PCSTR LibFile) {
	BOOL flag = FALSE;
	HANDLE hProcess = NULL, hThread = NULL;
	PCSTR LibFileRemote = NULL;
	__try {
		hProcess = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_VM_OPERATION | PROCESS_VM_WRITE, FALSE, ProcID);
		if (hProcess == NULL)__leave;
		int cch = 1 + lstrlen(LibFile);
		int cb = cch * sizeof(CHAR);
		LibFileRemote = (PCSTR)VirtualAllocEx(hProcess, NULL, cb, MEM_COMMIT, PAGE_READWRITE);
		if (LibFileRemote == NULL) __leave;

		if (!WriteProcessMemory(hProcess, (LPVOID)LibFileRemote,
			(PVOID)LibFile, cb, NULL)) __leave;

		PTHREAD_START_ROUTINE pfnThreadRtn = (PTHREAD_START_ROUTINE)
			GetProcAddress(GetModuleHandle(TEXT("Kernel32")), "LoadLibraryA");
		if (pfnThreadRtn == NULL) __leave;

		hThread = CreateRemoteThread(hProcess, NULL, 0,
			pfnThreadRtn, (LPVOID)LibFileRemote, 0, NULL);
		if (hThread == NULL) __leave;
		WaitForSingleObject(hThread, INFINITE);
		flag = TRUE;
	}
	__finally {
		if (LibFileRemote != NULL)
			VirtualFreeEx(hProcess, (LPVOID)LibFileRemote, 0, MEM_RELEASE);
		if (hThread != NULL)
			CloseHandle(hThread);
		if (hProcess != NULL)
			CloseHandle(hProcess);
	}
	return(flag);
}


int main()
{
	cout << "Write program name" << endl;
	std::string strtmp;
	cin >> strtmp;
	const char* name = strtmp.c_str();
	InjectLIb(GetProcessIdByProcessName(name), "C:/Users/Ksusha/Source/Repos/String_Replace_DLL/Debug/DllInject.dll");
	return 0;
}

DWORD GetProcessIdByProcessName(const char* processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	PROCESSENTRY32 processEntry;

	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	bool isFound = false;
	while (Process32Next(hSnapshot, &processEntry) && !isFound)
	{

		if (!strcmp(processEntry.szExeFile, processName))
		{
			processId = processEntry.th32ProcessID;
			isFound = true;
		}
	}

	return processId;
}


