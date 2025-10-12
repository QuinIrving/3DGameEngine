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
#include "Scene/Objects/Sphere.h"
#include <chrono>

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
	float size = 1.f;
	Cube c = Cube(size);
	//c.Translate(0, 2, -1.5); // x translate as -15 is good for checking clipping capabilities.
	c.Translate(3, -2, -1.5);
	c.Rotate(-61, -75, -45);
	Sphere s = Sphere(1.f,12,24);
	//s.Translate(0, 0, -8);
	//s.Scale(2, 2, 2);
	//---------------//


	// Run the message loop
	MSG msg = { };
	bool isRunning = true;
	// don't forget delta time!!!!!!!
	
	std::chrono::time_point timeSinceLastFrame = std::chrono::steady_clock::now();
	int currFrame = 0;
	int frameCount = 0;
	float elapsedTime = 0.f;

	while (isRunning) {
		std::chrono::time_point<std::chrono::steady_clock> currentTime = std::chrono::steady_clock::now();
		std::chrono::duration<float> timeDiff = currentTime - timeSinceLastFrame;
		timeSinceLastFrame = currentTime;
		float deltaTime = timeDiff.count();

		elapsedTime += deltaTime;
		frameCount++;


		if (elapsedTime >= 1.f) {
			float fps = frameCount / elapsedTime;

			//OutputDebugString(std::format(L"Delta Time: {}\n", deltaTime).c_str());
			//OutputDebugString(std::format(L"FPS: {}\n", fps).c_str());

			elapsedTime = 0.f;
			frameCount = 0;
		}

		currFrame++;

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

		win.gfx.testIndex = 0;

		// cube
		//float speed1 = -0.04 * deltaTime;
		win.gfx.Pipeline(c.GetVertices(), c.GetVertexIds(), c.GetModelMatrix());
		//c.Rotate(deltaTime, 0, 0);
		c.Translate(0, 0, -0.1 * deltaTime);


		win.gfx.testIndex = 1;
		// sphere
		//win.gfx.Pipeline(s.GetVertices(), s.GetVertexIds(), s.GetModelMatrix());
		//s.Rotate(-1 * deltaTime, 0.5 * deltaTime, 0);

		win.gfx.Render();
		Sleep(1);	
	}

	// SWITCH TO COMPTR's to handle all of this.
	//stagingTexture->Release();
	//renderTargetView->Release();
	return 0;
}
