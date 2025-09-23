#include "Window/Window.h"
#include <new>
#include <format>
#include <string>
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Models/Vertex.h"
#include "Models/Triangle.h"
#include "Models/Cube.h"
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

		std::pair<int, int> p1 = { win.GetClientWidth() / 2, win.GetClientHeight() / 2 };
		std::pair<int, int> p2 = { xPos, yPos };
		win.gfx.DrawLine(p1, p2);

		win.gfx.PutPixel(win.GetClientWidth() / 2, win.GetClientHeight() / 2, colour);

		/*Mat4<float> test = Mat4<float>::GetIdentity();
		test[2][3] = 0.5;

		float* vals = test.GetValues();

		for (int i = 0; i < 16; ++i) {
			OutputDebugString(std::format(L"Row: {}, Col: {}. Value: {}\n", i / 4, i % 4, vals[i]).c_str());
		}*/

		Vertex A = Vertex(20, 120, 11);
		Vertex B = Vertex(300, 200, 12);
		Vertex C = Vertex(524, 500, 22);

		//win.gfx.DrawLine(A, B, 0xFF00FFFF);
		//win.gfx.DrawLine(B, C, 0xFF00FFFF);
		//win.gfx.DrawLine(C, A, 0xFF00FFFF);

		//win.gfx.PutPixel(static_cast<int>(std::round(A.GetPosition().x)), static_cast<int>(std::round(A.GetPosition().y)), 0xFFFF0000);
		//win.gfx.PutPixel(A.GetPosition().x / A.GetPosition().z, A.GetPosition().y / A.GetPosition().z, 0xFF00FFFF);

		//win.gfx.PutPixel(static_cast<int>(std::round(B.GetPosition().x)), static_cast<int>(std::round(B.GetPosition().y)), 0xFFFF0000);
		//win.gfx.PutPixel(B.GetPosition().x / B.GetPosition().z, B.GetPosition().y / B.GetPosition().z, 0xFF00FFFF);

		//win.gfx.PutPixel(static_cast<int>(std::round(C.GetPosition().x)), static_cast<int>(std::round(C.GetPosition().y)), 0xFFFF0000);
		//win.gfx.PutPixel(C.GetPosition().x / C.GetPosition().z, C.GetPosition().y / C.GetPosition().z, 0xFF00FFFF);

		Triangle t = Triangle(A, B, C, 0xFFBBAA55);
		//Vec3<float> triNorm = t.ComputeFaceNormal();
		//OutputDebugString(std::format(L"\nx: {}, y: {}, z: {}\n\n", triNorm.x, triNorm.y, triNorm.z).c_str());
		//win.gfx.DrawTriangle(t);

		float size = 100.0;
		Cube c = Cube(size);
		std::array<int, 36> arr = c.GetVertexIds();
		std::array<Vertex, 8> v = c.GetVertices();
		Triangle t1 = Triangle(
			v[arr[0]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
			v[arr[1]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
			v[arr[2]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
			0xFFBBAA55);

		for (int i = 0; i < 6; i += 3) {
			win.gfx.DrawTriangle(Triangle(
				v[arr[i]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
				v[arr[i + 1]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
				v[arr[i + 2]].GetPosition() + Vec3<float>(size + 10, size + 10, size + 10),
				(i == 0) ? 0xFFBBAA55 : 0xFF0000FF));
		}

		float a1[16] = {1.f, 0.f, 0.f, 0.f,
						0.f, 1.f, 0.f, 0.f,
						0.f, 0.f, 1.f, 0.f,
						1.f, 2.f, 3.f, 1.f };

		Mat4<float> m = Mat4<float>(a1);

		float b2[16] = {2.f, 0.f, 0.f, 0.f,
						0.f, 3.f, 0.f, 0.f,
						0.f, 0.f, 4.f, 0.f,
						0.f, 0.f, 0.f, 1.f};

		Mat4<float> n = Mat4<float>(b2);

		Mat4<float> newM = m * n;
		
		for (int r = 0; r < 4; ++r) {
			for (int c = 0; c < 4; ++c) {
				OutputDebugString(std::format(L"| {} ", newM[r][c]).c_str());
			}
			OutputDebugString(L"|\n");
		}

		Vec4<float> point = Vec4<float>(3, 4, 7, -1);
		Vec4<float> result = point * newM;
		Vec4<float> result2 = point * m * n;

		OutputDebugString(std::format(L"\n[{}, {}, {}, {}]\n", result.x, result.y, result.z, result.w).c_str());
		OutputDebugString(std::format(L"\n[{}, {}, {}, {}]\n", result2.x, result2.y, result2.z, result2.w).c_str());

		win.gfx.Render();
		Sleep(10000);	
	}

	// SWITCH TO COMPTR's to handle all of this.
	//stagingTexture->Release();
	//renderTargetView->Release();
	return 0;
}
