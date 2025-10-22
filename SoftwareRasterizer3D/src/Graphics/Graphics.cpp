#include "Graphics/Graphics.h"
#include <math.h>
#include <iostream>
#include "Math/GeneralMath.h"
#include <format>
#include <ranges>

void Graphics::ClipOneOut(std::vector<VertexPostClip>& v, std::vector<int>& postClipIds, const VertexOut& v1, const VertexOut& v2, const VertexOut& v3) {
	const Vec4<float>& clip1 = v1.GetPosition();
	const Vec4<float>& in1 = v2.GetPosition();
	const Vec4<float>& in2 = v3.GetPosition();
	
	// just v1.
	float t = (-clip1.z) / (in1.z - clip1.z);
	Vec4<float> v12New = clip1 + ((in1 - clip1) * t);
	// need to also interpolate attributes?
	t = (-clip1.z) / (in2.z - clip1.z);
	Vec4<float> v13New = clip1 + ((in2 - clip1) * t);
	v.push_back(v2.PerspectiveDivide()); // 0
	v.push_back(v3.PerspectiveDivide()); // 1
	v.push_back(VertexOut(v12New).PerspectiveDivide()); // 2
	v.push_back(VertexOut(v13New).PerspectiveDivide()); // 3
	postClipIds.push_back(0);
	postClipIds.push_back(3);
	postClipIds.push_back(1);

	postClipIds.push_back(0);
	postClipIds.push_back(2);
	postClipIds.push_back(3);
}

void Graphics::ClipTwoOut(std::vector<VertexPostClip>& v, std::vector<int>& postClipIds, const VertexOut& v1, const VertexOut& v2, const VertexOut& v3) {
	const Vec4<float>& clip1 = v1.GetPosition();
	const Vec4<float>& clip2 = v2.GetPosition();
	const Vec4<float>& in1 = v3.GetPosition();
	
	float t = (-clip1.z) / (in1.z - clip1.z);
	Vec4<float> v2New = clip1 + ((in1 - clip1) * t);
	// need to also interpolate attributes.
	t = (-clip2.z) / (in1.z - clip2.z);
	Vec4<float> v3New = clip2 + ((in1 - clip2) * t);

	

	v.push_back(v3.PerspectiveDivide());
	postClipIds.push_back(0);
	v.push_back(VertexOut(v2New).PerspectiveDivide());
	postClipIds.push_back(1);
	v.push_back(VertexOut(v3New).PerspectiveDivide());
	postClipIds.push_back(2);
}

void Graphics::ResetZBuffer() {
	std::ranges::fill(zBuffer, 1.f);
}
/*
For RASTERIZER:
- Scanline approach (Simplest, although flat top vs flat bottom problem)
- Pineda approach (Best for CPU multithreading)

Make it so in the game I can swap between the type as well.
*/

void Graphics::PutPixel(int x, int y, Vec4<float> colour) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		OutputDebugString(std::format(L"\n[WARNING] Attempted to draw outside of the window.\nPos: ({}, {}) Window w, h: ({}, {})\n", x, y, m_width, m_height).c_str());
		return;
	}

	uint32_t packedColour = 0;
	packedColour |= static_cast<uint32_t>(colour.w) << 24;
	packedColour |= static_cast<uint32_t>(colour.z) << 16;
	packedColour |= static_cast<uint32_t>(colour.y) << 8;
	packedColour |= static_cast<uint32_t>(colour.x);

	pixels[y * (mapped.RowPitch / sizeof(uint32_t)) + x] = packedColour;
}

// utilizes Bresenham's line drawing algorithm for quick drawing.
void Graphics::DrawLine(int x0, int y0, int x1, int y1, const Vec4<float>& colour) {
	bool isYMajorAxis = std::abs(y1 - y0) > std::abs(x1 - x0);
	
	if (isYMajorAxis) {
		std::swap(x0, y0);
		std::swap(x1, y1);
	};

	if (x0 > x1) {
		std::swap(x0, x1);
		std::swap(y0, y1);
	}

	int dx = x1 - x0;
	int dy = y1 - y0;

	int dir = (dy < 0) ? -1 : 1;
	dy *= dir;

	if (dx == 0) {
		return;
	}

	int y = y0;
	int p = (2 * dy) - dx;

	for (int i = 0; i <= dx; ++i) {
		isYMajorAxis ? PutPixel(y, x0 + i, colour) : PutPixel(x0 + i, y, colour);

		if (p >= 0) {
			y += dir;
			p -= 2 * dx;
		}

		p += 2 * dy;
	}
}

void Graphics::DrawLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, const Vec4<float>& colour) {
	Graphics::DrawLine(p1.first, p1.second, p2.first, p2.second, colour);
}

void Graphics::DrawLine(const VertexIn& v1, const VertexIn& v2, const Vec4<float>& colour) {
	// should change this to a const Vertex but need to figure out the auto with Vec.
	auto pos1 = v1.GetPosition();
	auto pos2 = v2.GetPosition();

	Graphics::DrawLine(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), colour);
}

/* void Graphics::DrawTriangleScanLine(Triangle& t) {
*   // this should utilize the other method just so that it properly is able to swap between the two.
* }
*/

HRESULT Graphics::FirstInitialize(HWND hwnd) {
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

HRESULT Graphics::Render() {
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

HRESULT Graphics::SetupScreen() {
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

	ResetZBuffer();

	return 0;
}

HRESULT Graphics::ResizeWindow(int width, int height) {
	m_width = width;
	m_height = height;

	float fovY = PI / 2; // 90 degrees in radians.
	float yScaleFactor = tanf(fovY / 2);
	//float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height); // Might want to change this to a constant selection in an options menu later but oh well.
	float aspectRatio = 1.777;

	float n{1}; // near is defined for some reason
	float f{1000}; // same for far. maybe for direct3d or something.

	// REALLY POORLY DONE, SHOULD CALL A FUNCTION THAT SIMPLY UPDATES THE PROJECTION MATRIX WITH THE ASPECT RATIO LOL.
	// Row-major order.
	projectionMatrix = Mat4<float>(
		{ 
		1/(yScaleFactor * aspectRatio), 0,				0,						  0,
		0,								1/yScaleFactor, 0,						  0,
		0,								0,				-(f/(f - n)),			 -1,
		0,								0,				-((f * n) / (f - n)),	  0
		});

	zBuffer = std::vector<float>(width * height, 1.f); // occurs during rasterization. After perspective divide, z isn't touched while x & y are transformed to screen resolution coords

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

bool Graphics::IsTopLeftEdge(const Vec3<float>& A, const Vec3<float>& B) const {
	if (A.y == B.y) {
		return A.x > B.x; // Horizontal edges going left are top edges. 
	}

	return A.y < B.y; // non-horizontal edges pointing up are top edges.
}