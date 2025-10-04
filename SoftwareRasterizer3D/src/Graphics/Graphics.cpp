#include "Graphics/Graphics.h"
#include <math.h>
#include <iostream>
#include "Math/GeneralMath.h"


void Graphics::Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const Mat4<float>& modelMatrix) {
	Mat4<float> MVP = modelMatrix * camera.GetViewMatrix() * projectionMatrix; // could technically optimize this by pre-computed VP once a frame, not once per object.
	std::vector<VertexOut> clipVertices;
	clipVertices.reserve(vertices.size());

	for (const VertexIn &v : vertices) {
		clipVertices.push_back(VertexShader(v, MVP));
	}

	// primitive assembly
	for (int i = 0; i < indices.size(); i += 3) {
		//Triangle (clipVerts[i], clipVerts[i+1], clipVerts[i+2]
	}

	// Clipping check.

	// Perspective Divide (To NDC).
	for (VertexOut& v : clipVertices) {
		/*
		-
		Vertex Out should have a perspective divide function. Perhaps include viewport transformation right after? & inside
		v.x / w,
		v.y / w,
		v.z / w,
		------
		Viewport transform.
		(v.x + 1) * (w/2)
		(v.y + 1) * (h/2)

		// if z is in [-1, 1]
		(v.z + 1) / 2
		// if z is in [0, 1]
		v.z
		-
		*/
	}

	// Viewport Transform, so size of our image. Need to make sure aspect ratio is maintained. I believe it is.
	

	// primitive assembly, new triangles with our new coords
	// go through our indices and create triangles based on our new positions.
	// might do back-face culling here

	// Rasterization -> and z-buffer checks
	// pass each triangle to be rasterized, which also involves z-buffer check
	// rasterizer creates fragments for each triangle, which are passed to the fragment shader (and z-buffer I believe), which will update the values

}

VertexOut Graphics::VertexShader(const VertexIn& vin, const Mat4<float>& MVP) {
	// no shading for now;
	Vec4<float> clippedPos = vin * MVP;
	//return vin * MVP;
	return VertexOut(clippedPos);
}
/*
For RASTERIZER:
- Scanline approach (Simplest, although flat top vs flat bottom problem)
- Pineda approach (Best for CPU multithreading)

Make it so in the game I can swap between the type as well.
*/

void Graphics::PutPixel(int x, int y, uint32_t colour) {
	if (x < 0 || x >= m_width || y < 0 || y >= m_height) {
		OutputDebugString(std::format(L"\n[WARNING] Attempted to draw outside of the window.\nPos: ({}, {}) Window w, h: ({}, {})\n", x, y, m_width, m_height).c_str());
		return;
	}

	pixels[y * (mapped.RowPitch / sizeof(uint32_t)) + x] = colour;
}

// utilizes Bresenham's line drawing algorithm for quick drawing.
void Graphics::DrawLine(int x0, int y0, int x1, int y1, uint32_t colour) {
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

void Graphics::DrawLine(const std::pair<int, int>& p1, const std::pair<int, int>& p2, uint32_t colour) {
	Graphics::DrawLine(p1.first, p1.second, p2.first, p2.second, colour);
}

void Graphics::DrawLine(const VertexIn& v1, const VertexIn& v2, uint32_t colour) {
	// should change this to a const Vertex but need to figure out the auto with Vec.
	auto pos1 = v1.GetPosition();
	auto pos2 = v2.GetPosition();

	Graphics::DrawLine(static_cast<int>(pos1.x), static_cast<int>(pos1.y), static_cast<int>(pos2.x), static_cast<int>(pos2.y), colour);
}

// NEED TO INCLUDE A WAY TO NOT ATTEMPT DRAW IF not in CCW. Backface culling!!!!
void Graphics::DrawTriangle(const Triangle& tri) {
	VertexIn A = tri.GetVertexA();
	VertexIn B = tri.GetVertexB();
	VertexIn C = tri.GetVertexC();
	
	// get the bounding box:
	Vec3<float> posA = A.GetPosition();
	Vec3<float> posB = B.GetPosition();
	Vec3<float> posC = C.GetPosition();

	float top = posA.y;
	float left = posA.x;
	float bottom = posA.y;
	float right = posA.x;

	if (posB.x < left) {
		left = posB.x;
	}
	else if (posB.x > right) {
		right = posB.x;
	}

	if (posB.y < top) {
		top = posB.y;
	}
	else if (posB.y > bottom) {
		bottom = posB.y;
	}

	if (posC.x < left) {
		left = posC.x;
	}
	else if (posC.x > right) {
		right = posC.x;
	}

	if (posC.y < top) {
		top = posC.y;
	}
	else if (posC.y > bottom) {
		bottom = posC.y;
	}

	// now we have the bounding box of the triangle. We can go from top left to bottom right of the pixels to check which pixels to draw
	int t = static_cast<int>(floor(top));
	int b = static_cast<int>(ceil(bottom));
	int l = static_cast<int>(floor(left));
	int r = static_cast<int>(ceil(right));

	// first calculate the edges of the triangle for the edge function.
	int C0x = static_cast<int>(posA.y - posB.y); // B->A
	int C0y = static_cast<int>(-(posA.x - posB.x));
	int C1x = static_cast<int>(posC.y - posA.y); // A->C
	int C1y = static_cast<int>(-(posC.x - posA.x));
	int C2x = static_cast<int>(posB.y - posC.y); // C->B
	int C2y = static_cast<int>(-(posB.x - posC.x));

	int edge0 = static_cast<int>(((l - posB.x) * (posA.y - posB.y)) - ((t - posB.y) * (posA.x - posB.x))); // B->A
	int edge1 = static_cast<int>(((l - posA.x) * (posC.y - posA.y)) - ((t - posA.y) * (posC.x - posA.x))); // A->C
	int edge2 = static_cast<int>(((l - posC.x) * (posB.y - posC.y)) - ((t - posC.y) * (posB.x - posC.x))); // C->B

	int e0;
	int e1;
	int e2;

	for (int y = t; y <= b; ++y) {
		// need to reset to the left side of our edge function, but also incorporate the row we are now on.
		int yDiff = (y - t);
		e0 = edge0 + (yDiff * C0y);
		e1 = edge1 + (yDiff * C1y);
		e2 = edge2 + (yDiff * C2y);

		for (int x = l; x <= r; ++x) {
			//OutputDebugString(std::format(L"\nx: {}, y: {}, e0: {}, e1: {}, e2: {}\n", x, y, e0, e1, e2).c_str());

			if (e0 >= 0 && e1 >= 0 && e2 >= 0) {
				PutPixel(x, y, tri.GetColour());
			}
			
			// move to the right.
			e0 += C0x;
			e1 += C1x;
			e2 += C2x;
		}
	}
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

	return 0;
}

HRESULT Graphics::ResizeWindow(int width, int height) {
	m_width = width;
	m_height = height;

	float fovY = PI / 2; // 90 degrees in radians.
	float yScaleFactor = tanf(fovY / 2);
	float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height); // Might want to change this to a constant selection in an options menu later but oh well.

	float n{0.1}; // near is defined for some reason
	float f{1000}; // same for far. maybe for direct3d or something.

	// REALLY POORLY DONE, SHOULD CALL A FUNCTION THAT SIMPLY UPDATES THE PROJECTION MATRIX WITH THE ASPECT RATIO LOL.
	// Row-major order.
	projectionMatrix = Mat4<float>(
		{ 
		1/(yScaleFactor * aspectRatio), 0,				0,						  0,
		0,								1/yScaleFactor, 0,						  0,
		0,								0,				-((f + n)/(f - n)),		 -1,
		0,								0,				-((2 * f * n) / (f - n)), 0
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