#include "Window.h"
#include <new>
#include <format>
#include <string>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

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

	HRESULT err;

	ID3D11Device* device;
	ID3D11DeviceContext* context;

	D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
	err = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, levels, 1u, D3D11_SDK_VERSION, &device, NULL, &context);
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to create device. Error Code: {}\n\n", err).c_str());
		return -1;
	}
	IDXGIDevice* dxgiDevice;
	err = device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI device from D3D11 device. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	IDXGIAdapter* dxgiAdapter;
	err = dxgiDevice->GetAdapter(&dxgiAdapter);
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI adapter from DXGI device. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	IDXGIFactory2* dxgiFactory;
	err = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI factory from DXGI adapter. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
	ZeroMemory(&swapDesc, sizeof(swapDesc));
	swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapDesc.SampleDesc.Count = 1;
	swapDesc.SampleDesc.Quality = 0;
	swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapDesc.BufferCount = 2;

	IDXGISwapChain1* swapChain;
	err = dxgiFactory->CreateSwapChainForHwnd(device, win.GetWindow(), &swapDesc, NULL, NULL, &swapChain);
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to create swapchain from DXGI factory. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	D3D11_TEXTURE2D_DESC stagingDesc = {};
	stagingDesc.Width = w;
	stagingDesc.Height = h;
	stagingDesc.MipLevels = 1;
	stagingDesc.ArraySize = 1;
	stagingDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	stagingDesc.SampleDesc.Count = 1;
	stagingDesc.SampleDesc.Quality = 0;
	stagingDesc.Usage = D3D11_USAGE_STAGING;
	stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

	ID3D11Texture2D* stagingTexture;
	err = device->CreateTexture2D(&stagingDesc, NULL, &stagingTexture);
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to create a 2D texture from the staging description. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	ID3D11Texture2D* backBuffer;
	err = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to create a backbuffer from the swapchain. Error Code: {}\n\n", err).c_str());
		return -1;
	}
	
	ID3D11RenderTargetView* renderTargetView;
	err = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
	backBuffer->Release();
	if (FAILED(err)) {
		OutputDebugStringW(std::format(L"\n\nOutput failed to create a render target view from the device & backbuffer. Error Code: {}\n\n", err).c_str());
		return -1;
	}

	dxgiFactory->MakeWindowAssociation(win.GetWindow(), DXGI_MWA_NO_ALT_ENTER);
	dxgiFactory->Release();
	dxgiAdapter->Release();
	dxgiDevice->Release();
	swapChain->Release();
	device->Release();
	context->Release();

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

		// rendering stage
		D3D11_MAPPED_SUBRESOURCE mapped;
		err = context->Map(stagingTexture, 0, D3D11_MAP_WRITE, 0, &mapped);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to map the staged texture to our subresource. Error Code: {}\n\n", err).c_str());
			return -1;
		}
		
		int r = (xPos * 255) / w; // least significant
		int g = (yPos * 255) / h; // 2nd least
		int b = 0xBB;
		int a = 0xFF;

		uint32_t colour = 0;
		colour |= static_cast<uint32_t>(a) << 24;
		colour |= static_cast<uint32_t>(b) << 16;
		colour |= static_cast<uint32_t>(g) << 8;
		colour |= static_cast<uint32_t>(r);


		uint32_t* pixels = static_cast<uint32_t*>(mapped.pData);
		for (uint32_t y = 0; y < stagingDesc.Height; ++y) {
			for (uint32_t x = 0; x < stagingDesc.Width; ++x) {
				pixels[y * (mapped.RowPitch / sizeof(uint32_t)) + x] = colour;
			}
		}
		context->Unmap(stagingTexture, 0);

		// copy staging texture to backbuffer;
		context->CopyResource(backBuffer, stagingTexture);

		// Bind render target and present
		context->OMSetRenderTargets(1, &renderTargetView, NULL);
		swapChain->Present(0, 0);

		Sleep(1);	
	}

	stagingTexture->Release();
	renderTargetView->Release();
}
