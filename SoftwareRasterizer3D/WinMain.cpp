#include "Window.h"
#include <new>
#include <format>
#include <string>

constexpr wchar_t WND_TITLE[] = L"3DGameEngine";
constexpr wchar_t WND_NAME[] = L"Main Window Class";
constexpr int WND_WIDTH = 1440;
constexpr int WND_HEIGHT = 900;

std::wstring text = L"";
std::wstring oldText = L"";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	Window win(WND_NAME, WND_WIDTH, WND_HEIGHT);
	
	if (!win.Create(WND_TITLE, WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW)) {
		return 0;
	}

	RECT clientRect;
	GetClientRect(win.GetWindow(), &clientRect);
	LONG w = clientRect.right - clientRect.left;
	LONG h = clientRect.bottom - clientRect.top;
	win.SetClientSize(w, h); // one-time initalization.

	win.gfx.FirstInitialize(win.GetWindow());
	win.gfx.ResizeWindow(w, h);

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

		// process
		int xPos = win.mouse.GetXPos();
		int yPos = win.mouse.GetYPos();

		int r = (xPos * 255) / w; // least significant
		int g = (yPos * 255) / h; // 2nd least
		int b = 0xBB;
		int a = 0xFF;

		uint32_t colour = 0;
		colour |= static_cast<uint32_t>(a) << 24;
		colour |= static_cast<uint32_t>(b) << 16;
		colour |= static_cast<uint32_t>(g) << 8;
		colour |= static_cast<uint32_t>(r);

		// Render
		win.gfx.SetupScreen();

		win.gfx.PutPixel(win.GetClientWidth() / 2, win.GetClientHeight() / 2, colour);
		std::pair<int, int> p1 = { win.GetClientWidth() / 2, win.GetClientHeight() / 2 };
		std::pair<int, int> p2 = { xPos, yPos };

		win.gfx.DrawLine(p1, p2);

		win.gfx.Render();
		Sleep(1);	
	}

	//stagingTexture->Release();
	//renderTargetView->Release();
}
