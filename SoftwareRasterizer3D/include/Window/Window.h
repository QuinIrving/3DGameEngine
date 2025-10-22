#pragma once
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include "Input/InputManager.h"
#include "Input/Mouse.h"
#include "Input/Keyboard.h"
#include "Graphics/Graphics.h"
#include "Game/Core/GameManager.h"
#include "BindingTable/GameBindingTable.h"

class Window {
public:
	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

	Window(PCWSTR name, int w, int h) 
		: hwnd(NULL), className(name), mouse(Mouse()), kbd(Keyboard()), width(w), height(h), gfx(Graphics()), eventBuffer(std::queue<std::unique_ptr<GameEvent>>()), gameBindingTable(GameBindingTable()), gameManager(GameManager::GetHandle(gameBindingTable))
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

	bool IsEventBufferEmpty() const;
	std::optional<std::unique_ptr<GameEvent>> PopEventBuffer();

public:
	Mouse mouse;
	Keyboard kbd;
	Graphics gfx;
	std::queue<std::unique_ptr<GameEvent>> eventBuffer;
	GameBindingTable gameBindingTable;
	// other binding tables should also be created here

	GameManager& gameManager;

private:
	PCWSTR className;
	HWND hwnd;

	int width;
	int height;

	int client_w;
	int client_h;

	float g_DPIScale;
};