#include "Window/Window.h"
#include <new>
#include <format>
#include <string>
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Graphics/VertexIn.h"
#include "Models/Triangle.h"
#include "Scene/Objects/Cube.h"
#include "Math/MatrixVectorOps.h"

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

	
	//---------------//
	// Create an initial object for now:
	float size = 10.f;
	Cube c = Cube(size);
	c.Translate(0, 0, -13);
	//---------------//


	// Run the message loop
	MSG msg = { };
	bool isRunning = true;
	// don't forget delta time!!!!!!!
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

		float r = (static_cast<float>(xPos) * 255) / w; // least significant
		float g = (static_cast<float>(yPos) * 255) / h; // 2nd least
		float b = 187.f;
		float a = 255.f;

		// Render
		win.gfx.SetupScreen();

		std::pair<int, int> p1 = { win.GetClientWidth() / 2, win.GetClientHeight() / 2 };
		std::pair<int, int> p2 = { xPos, yPos };
		win.gfx.DrawLine(p1, p2);

		win.gfx.PutPixel(win.GetClientWidth() / 2, win.GetClientHeight() / 2, Vec4<float>{r, g, b, a});

		win.gfx.Pipeline(c.GetVertices(), c.GetVertexIds(), c.GetModelMatrix());
		c.Rotate(-0.02, 0.01, 0);

		win.gfx.Render();
		Sleep(1);	
	}

	// SWITCH TO COMPTR's to handle all of this.
	//stagingTexture->Release();
	//renderTargetView->Release();
	return 0;
}
