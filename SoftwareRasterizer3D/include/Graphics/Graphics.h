#pragma once
#include <format>
#include <d3d11.h>
#include <dxgi.h>
#include <dxgi1_2.h>
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxguid.lib")
#include <vector>
#include "Graphics/VertexIn.h"
#include "Graphics/VertexOut.h"
#include "Math/Mat4.h"
#include "Models/Triangle.h"
#include "Scene/Objects/Camera.h"

class Graphics {
public:
	inline static constexpr uint32_t DEFAULT_COLOUR = 0xFF0000FF;
	//inline static consteval Mat4<float> PROJECTION_MATRIX = Mat4<float>([]); // need to make a constexpr method of the projection matrix from mat4
	Graphics() = default;

	HRESULT FirstInitialize(HWND hwnd);
	HRESULT Render();
	HRESULT SetupScreen();
	HRESULT ResizeWindow(int width, int height);

	void PutPixel(int x, int y, uint32_t colour);
	void DrawLine(int x0, int y0, int x1, int y1, uint32_t colour = DEFAULT_COLOUR);
	void DrawLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, uint32_t colour = DEFAULT_COLOUR);
	void DrawLine(const VertexIn& v1, const VertexIn& v2, uint32_t colour = DEFAULT_COLOUR);

	// only until we move to an ECS and have a better abstracted system of render queues and the like
	// hidden from user and managed by our own program.
	void ResetZBuffer();

	// This should also take in a VertexShader and FragmentShader on next refactor to allow it to be a "programmable" pipeline.
	void Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const Mat4<float>& modelMatrix);
private:
	//VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);
	VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);
	void RasterizeTriangle(const Triangle& t); // rasterize a triangle.

	// FragmentOut FragmentShader(const Fragment& frag);
private:
	int m_width = 0;
	int m_height = 0;

	Camera camera = Camera();
	Mat4<float> projectionMatrix;

	// just for testing for now, will utilize shaders or something later on.
	std::array<uint32_t, 12> colours{
		0xFF641a3a, 0xFFa8a000, 0xFF094c6a, 0xFF1eacb2,
		0xFF29d351, 0xFFdebf68, 0xFF753ea5, 0xFF93361a,
		0xFF351892, 0xFF9b7ad6, 0xFFe3e6a5, 0xFFa1c2c0
	};

	std::vector<float> zBuffer;

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