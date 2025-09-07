#include "Window.h"
#include <new>
#include <format>

constexpr wchar_t WND_TITLE[] = L"3DGameEngine";
constexpr wchar_t WND_NAME[] = L"Main Window Class";

std::wstring text = L"";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	Window win(WND_NAME);
	
	if (!win.Create(WND_TITLE, WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW)) {
		return 0;
	}

	ShowWindow(win.GetWindow(), nCmdShow);

	// Run the message loop
	MSG msg = { };
	bool isRunning = true;

	while (isRunning) {
		win.kbd.ClearTextBuffer();
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isRunning = false;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!isRunning) {
			break;
		}

		int xPos = win.mouse.GetXPos();
		int yPos = win.mouse.GetYPos();

		//SetWindowText(win.GetWindow(), std::format(L"({}, {})", xPos, yPos).c_str());
		//SetWindowText(win.GetWindow(), std::format(L"Q: {}, W: {}, E: {}", win.kbd.IsKeyPressed('Q'), win.kbd.IsKeyPressed('W'), win.kbd.IsKeyPressed('E')).c_str());
		
		wchar_t c = win.kbd.PopTextChar();
		while (c != 0) {
			text.push_back(c);
			c = win.kbd.PopTextChar();
		}
		SetWindowText(win.GetWindow(), text.c_str());

		// game logic Update()
		// draw frame Render()
	}
}
