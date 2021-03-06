// TestProcessApp.cpp : Defines the entry point for the application.
//


#include "stdafx.h"
#include "TestProcessApp.h"


#include <Windows.h>
#include <iostream>
#include <vector>
#include <string>
#include <windows.h>
#include <algorithm>
#include <iterator>
#include <stdio.h>
#include <tchar.h>
#include <psapi.h>

#define TEXT_COLOR 0x442F2A
#define IDT_TIMER1 0x03E9
#define WINDOW_BACKGROUND_COLOR 0xFFFFFF
#define MOVABLE_RECT_COLOR 0xFFBEB7

//Global brushes
const HBRUSH WINDOW_BACKGROUND_BRUSH = CreateSolidBrush(WINDOW_BACKGROUND_COLOR);

RECT clientRect;
HINSTANCE hInstance;

char* testString;
//wchar_t stringToPrint[] = L"I hate math";

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

INT WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, INT nCmdShow) {

	HWND mainWindow;
	WNDCLASSEX windowClass;
	MSG receivedMessage;

	const wchar_t* const className = L"Test Class";
	const wchar_t* const windowName = L"Test";

	windowClass.cbSize = sizeof(windowClass);
	windowClass.style = 0;
	windowClass.lpfnWndProc = (WNDPROC)WindowProc;
	windowClass.cbClsExtra = 0;
	windowClass.cbWndExtra = 0;
	windowClass.hInstance = hInstance;
	windowClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	windowClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	windowClass.hbrBackground = NULL;
	windowClass.lpszMenuName = NULL;
	windowClass.lpszClassName = className;
	windowClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&windowClass)) {
		const wchar_t* const CLASS_REG_ERROR = L"Cannot register class";
		MessageBoxW(NULL, CLASS_REG_ERROR, NULL, MB_OK);
		return 0;
	}

	mainWindow = CreateWindowEx(
		0, className,
		windowName,
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		500, 100,
		200, 200,
		NULL, NULL,
		hInstance, NULL
	);

	if (!mainWindow) {
		const wchar_t* const WND_CREATE_ERROR = L"Cannot create window";
		MessageBoxW(NULL, WND_CREATE_ERROR, NULL, MB_OK);
		return 0;
	}

	while (GetMessage(&receivedMessage, NULL, 0, 0)) {
		TranslateMessage(&receivedMessage);
		DispatchMessage(&receivedMessage);
	}

	return receivedMessage.wParam;
}

LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {

	PAINTSTRUCT paintStruct;
	HDC hdc;
	HPEN oldPen;
	LPMINMAXINFO minMaxInfo;

	char str[] = "I wanna die";

	switch (uMsg) {
	case WM_CREATE:
		SetTimer(hWnd, IDT_TIMER1, 10000, (TIMERPROC)NULL);

		testString = (char*)malloc(100);
		memset((void*)testString, 0x00, 100);
		
		strcpy_s(testString ,100,str);
		break;

	case WM_TIMER:
		InvalidateRect(hWnd, &clientRect, false);
		//UpdateWindow(hWnd);
		break;

	case WM_PAINT:
		GetClientRect(hWnd, &clientRect);
		hdc = BeginPaint(hWnd, &paintStruct);

		FillRect(hdc, &clientRect, WINDOW_BACKGROUND_BRUSH);

		DrawTextA(hdc, testString, -1, &clientRect, DT_WORDBREAK | DT_CENTER);

		EndPaint(hWnd, &paintStruct);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	default:
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}
}