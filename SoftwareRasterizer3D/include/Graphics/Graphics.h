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
	void DrawTriangle(const Triangle& t); // rasterize a triangle.

	// also need to keep track of and utilize a z-buffer
	void Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const Mat4<float>& modelMatrix);
private:
	//VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);
	VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);

	// Pipeline takes a VertexIn for vertices
	// VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);
	// ^ that MVP could simply be computed once per pipeline call, and VP don't change so we'll see if we can 
	// each frame just pre-compute VP, then multiply like M(VP) way of calculating. WIll see.
	// Then clipping in clipping space, to remove triangles that aren't in view (and clip triangles partially out
	// to new triangles that are within the space). -> looks at 3 vertices at a time.
	// Then perspective divide to get to NDC space
	// Then Viewport transform to get our vertices into Raster Space.
	// void RasterizeTriangle(const VertexOut& v0, const VertexOut& v1, const VertexOut& v2);
	// FragmentOut FragmentShader(const Fragment& frag);
	/*
	-
	For our vertices provided, we can go through the indices, iterating + 3 and getting vertex 0, 1, 2
	with them being modified by vertex shader each.
	Then rasterize the triangle for those 3 vertices.
	Inside of rasterizer we get each barycentric coords pixel that we know is within the triangle,
	and from there after z-buffer test, we can pass it to the fragment shader to do work, before
	finally updating the back-buffer with our updated putpixel call for that specific fragment
	-
	*/
private:
	int m_width = 0;
	int m_height = 0;

	Camera camera = Camera();
	Mat4<float> projectionMatrix;

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