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
#include "Graphics/FragmentIn.h"
#include "Graphics/FragmentOut.h"
#include "Graphics/ModelAttributes.h"
#include "Math/Mat4.h"
#include "Models/Triangle.h"
#include "Scene/Objects/Camera.h"
#include "Shaders/ShaderSignatures.h"

class Graphics {
public:
	inline static constexpr Vec4<float> DEFAULT_COLOUR{255, 0, 0, 255};
	//inline static consteval Mat4<float> PROJECTION_MATRIX = Mat4<float>([]); // need to make a constexpr method of the projection matrix from mat4
	Graphics() = default;

	HRESULT FirstInitialize(HWND hwnd);
	HRESULT Render();
	HRESULT SetupScreen();
	HRESULT ResizeWindow(int width, int height);

	void PutPixel(int x, int y, Vec4<float> colour);
	void DrawLine(int x0, int y0, int x1, int y1, const Vec4<float>& colour = DEFAULT_COLOUR);
	void DrawLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, const Vec4<float>& colour = DEFAULT_COLOUR);
	void DrawLine(const VertexIn& v1, const VertexIn& v2, const Vec4<float>& colour = DEFAULT_COLOUR);

	// only until we move to an ECS and have a better abstracted system of render queues and the like
	// hidden from user and managed by our own program.
	void ResetZBuffer();

	// Make this an explicit template later.
	void Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const ModelAttributes& modelAttributes, TVertexShader auto& VertexShader, TFragmentShader auto& FragmentShader);

public:
	int testIndex = 0;
	Camera camera = Camera();

private:
	//VertexOut VertexShader(const VertexIn& vin, const Mat4<float>& MVP);
	void RasterizeTriangle(const Triangle& t, const ModelAttributes& modelAttributes, TFragmentShader auto& FragmentShader); // utilize an explicit template later.
	bool IsTopLeftEdge(const Vec3<float>& A, const Vec3<float>& B) const;
	void ClipOneOut(std::vector<VertexPostClip>& v, std::vector<int>& postClipIds, const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	void ClipTwoOut(std::vector<VertexPostClip>& v, std::vector<int>& postClipIds, const VertexOut& v1, const VertexOut& v2, const VertexOut& v3);
	//FragmentOut FragmentShader(const FragmentIn& frag);

private:
	int m_width = 0;
	int m_height = 0;

	Mat4<float> projectionMatrix;

	// just for testing for now, will utilize shaders or something later on.
	std::array<Vec4<float>, 12> colours{
		Vec4<float>(178, 10, 216, 255), Vec4<float>(155, 59, 239, 255), Vec4<float>(37, 16, 31, 255), Vec4<float>(84, 50, 37, 255),
		Vec4<float>(78, 139, 142, 255), Vec4<float>(76, 117, 42, 255), Vec4<float>(161, 252, 213, 255), Vec4<float>(33, 40, 61, 255),
		Vec4<float>(35, 255, 207, 255), Vec4<float>(166, 193, 75, 255), Vec4<float>(53, 29, 39, 255), Vec4<float>(170, 90, 162, 255)
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

#include "Graphics/Graphics.hpp"