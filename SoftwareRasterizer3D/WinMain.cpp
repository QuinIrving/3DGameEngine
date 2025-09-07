#include "Window.h"
#include <new>
#include <format>

constexpr wchar_t WND_TITLE[] = L"3DGameEngine";
constexpr wchar_t WND_NAME[] = L"Main Window Class";

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

		SetWindowText(win.GetWindow(), std::format(L"({}, {})", xPos, yPos).c_str());
		// game logic Update()
		// draw frame Render()
	}
}
