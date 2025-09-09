#pragma once
#include <format>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")

class Graphics {
public:
	Graphics() = default;

	HRESULT FirstInitialize(HWND hwnd) {
		HRESULT err;

		D3D_FEATURE_LEVEL levels[] = { D3D_FEATURE_LEVEL_11_0 };
		err = D3D11CreateDevice(NULL, D3D_DRIVER_TYPE_HARDWARE, NULL, D3D11_CREATE_DEVICE_DEBUG, levels, 1u, D3D11_SDK_VERSION, &device, NULL, &context);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to create device. Error Code: {}\n\n", err).c_str());
			return err;
		}
		
		err = device->QueryInterface(IID_PPV_ARGS(&dxgiDevice));
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI device from D3D11 device. Error Code: {}\n\n", err).c_str());
			return err;
		}

		
		err = dxgiDevice->GetAdapter(&dxgiAdapter);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI adapter from DXGI device. Error Code: {}\n\n", err).c_str());
			return err;
		}

		
		err = dxgiAdapter->GetParent(IID_PPV_ARGS(&dxgiFactory));
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to get DXGI factory from DXGI adapter. Error Code: {}\n\n", err).c_str());
			return err;
		}

		ZeroMemory(&swapDesc, sizeof(swapDesc));
		swapDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		swapDesc.SampleDesc.Count = 1;
		swapDesc.SampleDesc.Quality = 0;
		swapDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapDesc.BufferCount = 2;

		err = dxgiFactory->CreateSwapChainForHwnd(device, hwnd, &swapDesc, NULL, NULL, &swapChain);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to create swapchain from DXGI factory. Error Code: {}\n\n", err).c_str());
			return err;
		}

		err = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to create a backbuffer from the swapchain. Error Code: {}\n\n", err).c_str());
			return err;
		}

		dxgiFactory->MakeWindowAssociation(hwnd, DXGI_MWA_NO_ALT_ENTER);


		/*
		dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();
		swapChain->Release();
		device->Release();
		context->Release();
		*/
		return 0;
	}

	void PutPixel(int x, int y, uint32_t colour) {
		pixels[y * (mapped.RowPitch / sizeof(uint32_t)) + x] = colour;
	}

	HRESULT Render() {
		context->Unmap(stagingTexture, 0);

		// copy staging texture to backbuffer;
		context->CopyResource(backBuffer, stagingTexture);

		// Bind render target and present
		context->OMSetRenderTargets(1, &renderTargetView, NULL);
		HRESULT err = swapChain->Present(0, 0);
		if (FAILED(err)) {
			return err;
		}

		return 0;
	}

	HRESULT SetupScreen() {
		HRESULT err = context->Map(stagingTexture, 0, D3D11_MAP_WRITE, 0, &mapped);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to map the staged texture to our subresource. Error Code: {}\n\n", err).c_str());
			return err;
		}

		pitch = mapped.RowPitch / sizeof(uint32_t);
		// Set pixels as black.
		pixels = static_cast<uint32_t*>(mapped.pData);
		
		for (uint32_t y = 0; y < stagingDesc.Height; ++y) {
			std::fill(pixels + (y * pitch), pixels + y * pitch + stagingDesc.Width, 0xFF000000);
		}

		return 0;
	}

	HRESULT ResizeWindow(int width, int height) {
		m_width = width;
		m_height = height;
		
		HRESULT err;

		if (renderTargetView != NULL) {
			renderTargetView->Release();
			swapChain->ResizeBuffers(2, width, height, DXGI_FORMAT_R8G8B8A8_UNORM, 0);

			err = swapChain->GetBuffer(0, IID_PPV_ARGS(&backBuffer));
			if (FAILED(err)) {
				OutputDebugStringW(std::format(L"\n\nOutput failed to create a backbuffer from the swapchain. Error Code: {}\n\n", err).c_str());
				return err;
			}
		}

		err = device->CreateRenderTargetView(backBuffer, NULL, &renderTargetView);
		backBuffer->Release();
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to create a render target view from the device & backbuffer. Error Code: {}\n\n", err).c_str());
			return err;
		}

		stagingDesc.Width = width;
		stagingDesc.Height = height;
		stagingDesc.MipLevels = 1;
		stagingDesc.ArraySize = 1;
		stagingDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
		stagingDesc.SampleDesc.Count = 1;
		stagingDesc.SampleDesc.Quality = 0;
		stagingDesc.Usage = D3D11_USAGE_STAGING;
		stagingDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;

		err = device->CreateTexture2D(&stagingDesc, NULL, &stagingTexture);
		if (FAILED(err)) {
			OutputDebugStringW(std::format(L"\n\nOutput failed to create a 2D texture from the staging description. Error Code: {}\n\n", err).c_str());
			return err;
		}

		/*dxgiFactory->Release();
		dxgiAdapter->Release();
		dxgiDevice->Release();
		swapChain->Release();
		device->Release();
		context->Release();*/
		return 0;
	}

private:
	int m_width = 0;
	int m_height = 0;

	ID3D11Device* device;
	ID3D11DeviceContext* context;
	IDXGIDevice* dxgiDevice;
	IDXGIAdapter* dxgiAdapter;
	IDXGIFactory2* dxgiFactory;
	DXGI_SWAP_CHAIN_DESC1 swapDesc = {};
	IDXGISwapChain1* swapChain;
	ID3D11Texture2D* backBuffer;
	ID3D11RenderTargetView* renderTargetView;


	D3D11_MAPPED_SUBRESOURCE mapped;
	D3D11_TEXTURE2D_DESC stagingDesc = {};
	ID3D11Texture2D* stagingTexture;
	uint32_t* pixels;

	size_t pitch;
};