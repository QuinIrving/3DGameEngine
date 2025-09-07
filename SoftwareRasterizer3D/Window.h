#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Window(PCWSTR name) 
		: hwnd(NULL), className(name), mouse(Mouse()), kbd(Keyboard())
	{};

	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		int nWidth = CW_USEDEFAULT, int nHeight = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HWND GetWindow() const {
		return hwnd;
	}

	PCWSTR ClassName() const { return className; }

public:
	Mouse mouse;
	Keyboard kbd;

private:
	PCWSTR className;
	HWND hwnd;
};