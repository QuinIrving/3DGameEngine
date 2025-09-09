#pragma once
#include <Windows.h>
#include "InputManager.h"
#include "Mouse.h"
#include "Keyboard.h"
#include "Graphics.h"

class Window {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Window(PCWSTR name, int w, int h) 
		: hwnd(NULL), className(name), mouse(Mouse()), kbd(Keyboard()), width(w), height(h), gfx(Graphics())
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

	int GetClientWidth() const { return client_w; }
	int GetClientHeight() const { return client_h; }

	void SetClientSize(int width, int height);

public:
	Mouse mouse;
	Keyboard kbd;
	Graphics gfx;

private:
	PCWSTR className;
	HWND hwnd;

	int width;
	int height;

	int client_w;
	int client_h;
};