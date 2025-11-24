#include "Window/Window.h"
#include <new>
#include <format>
#include <string>
#include "Math/Mat4.h"
#include "Math/Vec4.h"
#include "Math/MatrixVectorOps.h"
#include "Graphics/VertexIn.h"
#include "Models/Triangle.h"
#include "Scene/Objects/Cube.h"
#include "Scene/Objects/Sphere.h"
#include "Scene/Objects/Camera.h"
#include "Scene/Objects/UnfoldedCube.h"
#include "Scene/Objects/Quad.h"
#include <chrono>
#include "Game/Core/Events/GameEvent.h"
#include "Game/Core/Events/CameraRotate.h"
#include "Game/Systems/MovementSystem.h"
#include "Shaders/FragmentShaders/DefaultFragmentShader.h"
#include "Shaders/VertexShaders/DefaultVertexShader.h"
#include "Graphics/Texture.h"
#include "Shaders/FragmentShaders/BasicTextureMapFragmentShader.h"
#include "Shaders/FragmentShaders/FlatShadedFragmentShader.h"
#include "Shaders/FragmentShaders/PhongFragmentShader.h"
#include "Models/Mesh.h"
#include "Scene/Objects/Object.h"
#include "Loaders/OBJLoader.h"

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
	//Cube c = Cube(size);
	//c.Translate(0, 2, -3.5); // x translate as -15 is good for checking clipping capabilities.
	//c.Rotate(-61, -75, -45);
	Sphere s = Sphere(1.f,48,24);
	s.Translate(0, 0, -8);
	s.Scale(3, 3, 3);

	UnfoldedCube c = UnfoldedCube(size);
	c.Translate(13, 3, -5.5);
	c.Scale(6, 6, 6);

	Quad quad = Quad(50, 50);
	quad.Translate(10, 0, 2);
	quad.Rotate(0, 45, 0);

	//---------------//
	// Create an initial object for testing
	Object dragon = loadOBJ("resources/Models/dragon-1500.obj", 16.f);
	dragon.Translate(24, -12, -12);
	Object bunny = loadOBJ("resources/Models/bunny.obj", 36.f);
	bunny.Translate(-12, -4, 4);

	//---------------//
	// Create an initial texture for testing.
	static const Texture t1{"resources/Textures/Cobblestone/CobblestoneWall01_1K_BaseColor.png"};
	static const Texture t2{ "resources/Textures/StoneWall/StoneWall03_2K_BaseColor.png" };

	//s.material.albedoTexture = &t1;
	c.material.albedoTexture = &t2;
	dragon.material.albedoTexture = &t1;
	quad.material.albedoTexture = &t1;

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
		win.gameManager.UpdateDeltaTime(deltaTime);

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

		// ALL OF THIS IS SUPER UGLY. Need to do some abstraction to get this stuff looking manageable.
		Vec3<float> cameraRot{};
		while (!win.IsEventBufferEmpty()) {
			auto gOpt = win.PopEventBuffer();
			GameEvent& g = **gOpt; //optional -> unique_ptr -> GameEvent reference.

			switch (g.GetType()) {
				case GameEvent::Type::CameraRotate: 
				{
					CameraRotate& cr = static_cast<CameraRotate&>(g);
					cameraRot += cr.GetFreecamAngles();
				}

			}

		}

		// still ugly here.
		if (win.gameManager.GetGameState().InFpsCameraMode) {
			win.gfx.camera.RotateXY(cameraRot.x, cameraRot.y);
		}
		else { // in freecam do something else
			
		}

		// process
		int xPos = win.mouse.GetXPos();
		int yPos = win.mouse.GetYPos();

		float r = (static_cast<float>(xPos) * 255) / w; // least significant
		float g = (static_cast<float>(yPos) * 255) / h; // 2nd least
		float b = 187.f;
		float a = 255.f;

		float speed = 4.1f;

		// CAMERA TEST:
		//GameState& state = win.gameManager.GetGameState();
		
		win.gfx.camera.Translate(MovementSystem::ComputeTranslation(win.gameManager.GetGameState(), deltaTime));

		// Render
		win.gfx.SetupScreen();

		//std::pair<int, int> p1 = { win.GetClientWidth() / 2, win.GetClientHeight() / 2 };
		//std::pair<int, int> p2 = { xPos, yPos };
		//win.gfx.DrawLine(p1, p2);

		//win.gfx.PutPixel(win.GetClientWidth() / 2, win.GetClientHeight() / 2, Vec4<float>{r, g, b, a});

		win.gfx.testIndex = 0;

		// cube
		//float speed1 = -0.04 * deltaTime;
		win.gfx.Pipeline(c.GetVertices(), c.GetVertexIds(), c.GetModelAttributes(), DefaultVertexShader, PhongFragmentShader);
		c.Rotate(deltaTime, -0.6f * deltaTime, deltaTime);
		//c.Translate(0, 0, -0.1f * deltaTime);


		win.gfx.testIndex = 1;
		// sphere
		win.gfx.Pipeline(s.GetVertices(), s.GetVertexIds(), s.GetModelAttributes(), DefaultVertexShader, PhongFragmentShader);
		s.Rotate(-1.f * deltaTime, 0.5f * deltaTime, 0.08f * deltaTime);

		win.gfx.Pipeline(dragon.GetVertices(), dragon.GetVertexIds(), dragon.GetModelAttributes(), DefaultVertexShader, PhongFragmentShader);

		win.gfx.Pipeline(bunny.GetVertices(), bunny.GetVertexIds(), bunny.GetModelAttributes(), DefaultVertexShader, FlatShadedFragmentShader);

		win.gfx.Pipeline(quad.GetVertices(), quad.GetVertexIds(), quad.GetModelAttributes(), DefaultVertexShader, PhongFragmentShader);

		win.gfx.Render();
		Sleep(1);	
	}

	// SWITCH TO COMPTR's to handle all of this.
	//stagingTexture->Release();
	//renderTargetView->Release();
	return 0;
}
