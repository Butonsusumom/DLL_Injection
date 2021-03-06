// Dll.cpp : Defines the exported functions for the DLL application.

#include "stdafx.h"
#include <iostream>
#include <algorithm>
#include <windows.h>
#include <stdio.h>
#include <vector>
#include <fstream>
#include <utility>
#include <limits.h>
#include "Dll.h"

using namespace std;

void replaceString(DWORD processPid, char* substring, char* newString) {

	HANDLE processHandle = OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION |
		PROCESS_VM_OPERATION | PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, processPid);

	bool find = false;

	if (processHandle) {
		std::string substringToReplace(substring);

		unsigned char *regionAddress = NULL;
		MEMORY_BASIC_INFORMATION memoryBasicInformation;

		while (VirtualQueryEx(processHandle, regionAddress, //информация о диапазоне страниц в виртуальном адресном пространстве указанного процесса.
			&memoryBasicInformation, sizeof(memoryBasicInformation)) == sizeof(memoryBasicInformation)&&(!find)) {

			std::vector<char> tempBuffer;

			if (memoryBasicInformation.State == MEM_COMMIT && //страницы есть в физической памяти и доступны для чтения/записи
				(memoryBasicInformation.Type == MEM_MAPPED || memoryBasicInformation.Type == MEM_PRIVATE)) { 
				SIZE_T bytesRead;
				tempBuffer.resize(memoryBasicInformation.RegionSize);
				ReadProcessMemory(processHandle, regionAddress, &tempBuffer[0], memoryBasicInformation.RegionSize, &bytesRead);
				tempBuffer.resize(bytesRead);

				auto substingOffset = std::search(tempBuffer.begin(), tempBuffer.end(), substringToReplace.begin(), substringToReplace.end());

				if (substingOffset!= tempBuffer.end()) {

					if (WriteProcessMemory(processHandle, (LPVOID)(regionAddress +
						((substingOffset - tempBuffer.begin()))), newString, strlen(newString) + 1, NULL)) {
						char Buffer[100];
						MessageBox(0, "Substring replaced successfully ", 0, MB_OK);
						ReadProcessMemory(processHandle, regionAddress + ((substingOffset - tempBuffer.begin())), &Buffer, strlen(newString) + 1, &bytesRead);
						MessageBox(0, Buffer, 0, MB_OK);
					}
				}
			}
			regionAddress += memoryBasicInformation.RegionSize;
		}
		CloseHandle(processHandle);
	}
	else {
		MessageBox(0, "Such process doesn't exist", 0, MB_OK);
	}
}