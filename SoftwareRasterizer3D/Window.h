#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "Mouse.h"
#include "Keyboard.h"

class Window {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Window(PCWSTR name, int w, int h) 
		: hwnd(NULL), className(name), mouse(Mouse()), kbd(Keyboard()), width(w), height(h)
	{};

	BOOL Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle = 0,
		int x = CW_USEDEFAULT, int y = CW_USEDEFAULT,
		HWND hWndParent = 0, HMENU hMenu = 0);

	LRESULT HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam);
	
	HWND GetWindow() const {
		return hwnd;
	}

	PCWSTR ClassName() const { return className; }

	int GetWidth() const { return width; }
	int GetHeight() const { return height; }

public:
	Mouse mouse;
	Keyboard kbd;

private:
	PCWSTR className;
	HWND hwnd;

	int width;
	int height;
};