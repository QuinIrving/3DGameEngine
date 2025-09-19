#pragma once
#include <format>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#include "Models/Vertex.h"
#include "Models/Triangle.h"

class Graphics {
public:
	inline static constexpr uint32_t DEFAULT_COLOUR = 0xFF0000FF;
	Graphics() = default;

	HRESULT FirstInitialize(HWND hwnd);
	HRESULT Render();
	HRESULT SetupScreen();
	HRESULT ResizeWindow(int width, int height);

	void PutPixel(int x, int y, uint32_t colour);
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t colour = DEFAULT_COLOUR);
	void DrawLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, uint32_t colour = DEFAULT_COLOUR);
	void DrawLine(const Vertex& v1, const Vertex& v2, uint32_t colour = DEFAULT_COLOUR);
	void DrawTriangle(const Triangle& t); // rasterize a triangle.

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