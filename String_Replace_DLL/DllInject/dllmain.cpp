// dllmain.cpp : Defines the entry point for the DLL application.
#include "stdafx.h"

/*#include "stdafx.h"
#include <iostream>
#include <Windows.h> 
#include <math.h>
#include <time.h>
#include <fstream>
#include <string>
#include <stdio.h>
#include <psapi.h>
#include <tchar.h>
#include <vector>
#include <TlHelp32.h>
#include "DllInject.h"


#define MAX_LOADSTRING 100
#define ID_BUTTON 1001

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name
HWND hProgName, hOldString, hNewString, hInjectButton;
const int MainWindowHeight = 350;
const int MainWindowWidth = 650;

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);
DWORD GetProcessIdByProcessName(char* processName);
LRESULT onButton(HWND hWnd, UINT message);

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
	{
		//UNREFERENCED_PARAMETER(hPrevInstance);
		//UNREFERENCED_PARAMETER(lpCmdLine);

		// TODO: Place code here.

		// Initialize global strings
		//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
		//LoadStringW(hInstance, IDC_DYNAMICDLL, szWindowClass, MAX_LOADSTRING);
		MyRegisterClass(NULL);

		// Perform application initialization:
		if (!InitInstance(NULL, SW_RESTORE))
		{
			return FALSE;
		}

		//HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DYNAMICDLL));

		MSG msg;

		// Main message loop:
		while (GetMessage(&msg, nullptr, 0, 0))
		{
				TranslateMessage(&msg);
				DispatchMessage(&msg);
		}

		return (int)msg.wParam;
	}
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEXW wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(30, 144, 255));
	//wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_DYNAMICDLL);
	wcex.lpszClassName = szWindowClass;
	//wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	hInst = hInstance; // Store instance handle in our global variable

	HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0, MainWindowWidth, MainWindowHeight, nullptr, nullptr, hInstance, nullptr);

	if (!hWnd)
	{
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	return TRUE;
}



LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		// Parse the menu selections:
		switch (wmId)
		{
		case ID_BUTTON:
			return onButton(hWnd, message);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}

	}
	break;
	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);

		TextOut(hdc, 50, 50, "Program name", 12); // вывод текстовых сообщений
		TextOut(hdc, 50, 100, "Old string", 10); // вывод текстовых сообщений
		TextOut(hdc, 50, 150, "New string", 10); // вывод текстовых сообщений

		EndPaint(hWnd, &ps);
	}
	break;
	case WM_CREATE:
	{
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hProgName = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 75, 550, 20,
			hWnd, 0, hInst, NULL);
		hOldString = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 125, 550, 20,
			hWnd, 0, hInst, NULL);
		hNewString = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 175, 550, 20,
			hWnd, 0, hInst, NULL);
		hInjectButton = CreateWindow("button", "Replace",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			265, 220, 120, 30, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
	}
	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}


DWORD GetProcessIdByProcessName(char* processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 processEntry;

	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	bool isFound = false;
	while (Process32Next(hSnapshot, &processEntry) && !isFound)
	{
		if (!strcmp(processName, processEntry.szExeFile))
		{
			processId = processEntry.th32ProcessID;
			isFound = true;
		}
	}

	return processId;
}

LRESULT onButton(HWND hWnd, UINT message) {
	char programName[MAX_CLASS_NAME];
	GetWindowTextA(hProgName, &programName[0], 199);

	char oldString[MAX_CLASS_NAME];
	if (hOldString == INVALID_HANDLE_VALUE || hOldString == 0) {
		MessageBox(0, "Invalid string", 0, MB_OK);
	}
	else {
		GetWindowTextA(hOldString, &oldString[0], 199);
	}

	char newString[MAX_CLASS_NAME];
	if (hNewString == INVALID_HANDLE_VALUE || hNewString == 0) {
		MessageBox(0, "Invalid string", 0, MB_OK);
	}
	else {
		GetWindowTextA(hNewString, &newString[0], 199);
	}

	replaceString(GetProcessIdByProcessName(programName), (char*)oldString, (char*)newString);
	return 0;
}*/



#include <iostream>
#include <string>
#include "DllInject.h"
#include <Windows.h> 
#include <math.h>
#include <time.h>
#include <tchar.h>
#include <fstream>
#include <string>
#include <TlHelp32.h>

#define ID_BUTTON 1001

using namespace std;

const int MainWindowHeight = 350;
const int MainWindowWidth = 650;


HWND hProgName, hOldString, hNewString, hInjectButton;
const HBRUSH WindowBackground = CreateSolidBrush(RGB(147, 112, 219));
LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

DWORD GetProcessIdByProcessName(char* processName);
LRESULT onButton(HWND hWnd, UINT message);

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		WNDCLASSEX WinClass;
		const char* ClassName = "Dll Window";
		WinClass.cbSize = sizeof(WinClass);
		WinClass.lpfnWndProc = (WNDPROC)WndProc;
		WinClass.style = 0;
		WinClass.cbClsExtra = 0;
		WinClass.cbWndExtra = 0;
		WinClass.hInstance = NULL;
		WinClass.hIcon = LoadIcon(NULL, IDI_ERROR);
		WinClass.hCursor = LoadCursor(NULL, IDC_ARROW);
		WinClass.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(147, 112, 219));
		WinClass.lpszMenuName = NULL;
		WinClass.lpszClassName = ClassName;
		WinClass.hIconSm = NULL;
		if (!RegisterClassEx(&WinClass)) {
			MessageBox(NULL,
				"Can't register Class!",
				"Error", MB_OK);
			return NULL;
		}
		const char* WindowName = "MainWindow";
		HWND MainWindow = CreateWindowEx(0, ClassName, WindowName, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, 0, MainWindowWidth, MainWindowHeight, NULL, NULL, NULL, NULL);
		if (!MainWindow) {
			MessageBox(NULL,
				"Can't create Window",
				"Error", MB_OK);
			return NULL;
		}
		ShowWindow(MainWindow, SW_RESTORE);
		UpdateWindow(MainWindow);
		MSG RecMessage;
		while (GetMessage(&RecMessage, NULL, 0, 0)) {
			TranslateMessage(&RecMessage);
			DispatchMessage(&RecMessage);
		}
		DeleteObject(MainWindow);
		return RecMessage.wParam;

	}
	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	HINSTANCE hInst;
	PAINTSTRUCT ps;
	HDC hdc;
	switch (uMsg) {
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps); // начало перерисовки
		TextOut(hdc, 50, 50, "Program name", 12); // вывод текстовых сообщений
		TextOut(hdc, 50, 100, "Old string", 10); // вывод текстовых сообщений
		TextOut(hdc, 50, 150, "New string", 10); // вывод текстовых сообщений
		EndPaint(hWnd, &ps); // конец перерисовки
		break;
	case WM_CREATE:{
		hInst = ((LPCREATESTRUCT)lParam)->hInstance;
		hProgName = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 75, 550, 20,
			hWnd, 0, hInst, NULL);
		hOldString = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 125, 550, 20,
			hWnd, 0, hInst, NULL);
		hNewString = CreateWindow("edit", NULL,
			WS_EX_CLIENTEDGE | WS_BORDER | WS_CHILD | WS_VISIBLE, 50, 175, 550, 20,
			hWnd, 0, hInst, NULL);
		hInjectButton = CreateWindow("button", "Replace",
			WS_CHILD | WS_VISIBLE | WS_BORDER,
			265, 220, 120, 30, hWnd, (HMENU)ID_BUTTON, hInst, NULL);
		}
		break;
	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case ID_BUTTON:
			return onButton(hWnd, uMsg);
			break;
		default:
			return DefWindowProc(hWnd, uMsg, wParam, lParam);
		}
	}
		break;
	case WM_DESTROY:
		PostQuitMessage(NULL);
		break;
	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
	return NULL;
}

DWORD GetProcessIdByProcessName(char* processName)
{
	DWORD processId = 0;
	HANDLE hSnapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
	PROCESSENTRY32 processEntry;

	ZeroMemory(&processEntry, sizeof(processEntry));
	processEntry.dwSize = sizeof(processEntry);

	bool isFound = false;
	while (Process32Next(hSnapshot, &processEntry) && !isFound)
	{
		if (!strcmp(processName, processEntry.szExeFile))
		{
			processId = processEntry.th32ProcessID;
			isFound = true;
		}
	}

	return processId;
}

LRESULT onButton(HWND hWnd, UINT message) {
	char programName[MAX_CLASS_NAME];
	GetWindowTextA(hProgName, &programName[0], 199);

	char oldString[MAX_CLASS_NAME];
	if (hOldString == INVALID_HANDLE_VALUE || hOldString == 0) {
		MessageBox(0, "Invalid string", 0, MB_OK);
	}
	else {
		GetWindowTextA(hOldString, &oldString[0], 199);
	}

	char newString[MAX_CLASS_NAME];
	if (hNewString == INVALID_HANDLE_VALUE || hNewString == 0) {
		MessageBox(0, "Invalid string", 0, MB_OK);
	}
	else {
		GetWindowTextA(hNewString, &newString[0], 199);
	}

	replaceString(GetProcessIdByProcessName(programName), (char*)oldString, (char*)newString);
	return 0;
}