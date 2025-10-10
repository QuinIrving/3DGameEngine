#include "Graphics/Graphics.h"
#include <math.h>
#include <iostream>
#include "Math/GeneralMath.h"
#include <format>
#include <ranges>

// Should abstract away from user. They shouldn't know about this private list of objs/entities
// when they create a new object, it should add it into this list. Some entity manager. That's for a later
// refactor.
//void Graphics::Render(const std::vector<Entity/Object/Renderable>& objs/entities) {
// for each entity {
//  Pipeline(entity.vertices, entity.indices, entity.modelMatrix);
// 
// }
// }
//;

void Graphics::Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const Mat4<float>& modelMatrix) {
	/*OutputDebugString(std::format(L"{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n{} {} {} {}\n\n\n",
		modelMatrix[0][0], modelMatrix[0][1], modelMatrix[0][2], modelMatrix[0][3],
		modelMatrix[1][0], modelMatrix[1][1], modelMatrix[1][2], modelMatrix[1][3], 
		modelMatrix[2][0], modelMatrix[2][1], modelMatrix[2][2], modelMatrix[2][3], 
		modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2], modelMatrix[3][3]).c_str());*/
	Mat4<float> MVP = modelMatrix * camera.GetViewMatrix() * projectionMatrix; // could technically optimize this by pre-computed VP once a frame, not once per object.
	std::vector<VertexOut> clipVertices;
	clipVertices.reserve(vertices.size());

	// Vertex Shader:
	for (const VertexIn &v : vertices) {
		clipVertices.push_back(VertexShader(v, MVP));
	}

	std::vector<Triangle> tris;
	tris.reserve(indices.size() / 3);

	// Clipping check:
	for (int i = 0; i < indices.size(); i += 3) {
		VertexOut& v1 = clipVertices[indices[i]];
		VertexOut& v2 = clipVertices[indices[i + 1]];
		VertexOut& v3 = clipVertices[indices[i + 2]];

		// triangle isn't within our view frustum at all, just cull the triangle completely
		if (!v1.IsInFrustum() && !v2.IsInFrustum() && !v3.IsInFrustum()) {
			OutputDebugString(L"\nCulled Triangle\n");
			continue;
		}

		std::vector<int> postClipIds;
		std::vector<VertexPostClip> v; // Not sure if we want to copy instead, reference is probably better. Figure it out in a later refactor.
		v.reserve(6); // max possible amount of vertices after clipping
		postClipIds.reserve(6); // same for the id's. Max is 2 triangles, with 6 total id's.

		// some portion of the triangle isn't within our view frustum, need to do clipping to create new vertices and triangle(s)
		if (!v1.IsInFrustum() || !v2.IsInFrustum() || !v3.IsInFrustum()) {
			// this will require clipping the vertices, putting them into our v, and ensuring we keep winding order for our
			// postClipIds vector as well.
			OutputDebugString(L"\nCulled Partial Triangle\n");
		}
		else {
			// all are within it, simply add them to the list.
			v.push_back(v1.PerspectiveDivide());
			postClipIds.push_back(0);
			v.push_back(v2.PerspectiveDivide());
			postClipIds.push_back(1);
			v.push_back(v3.PerspectiveDivide());
			postClipIds.push_back(2);
		}

		// The perspective-divided vertices (could have new vertices, so need to iterate over them to create the multiple triangles.
		for (int vpcI = 0; vpcI < postClipIds.size(); vpcI += 3) {
			VertexPostClip& vpc1 = v[postClipIds[vpcI]];
			VertexPostClip& vpc2 = v[postClipIds[vpcI + 1]];
			VertexPostClip& vpc3 = v[postClipIds[vpcI + 2]];

			// primitive creation.
			// just for testing for now (colours), will utilize shaders or something later on.
			//Triangle t = (testIndex == 0) ? Triangle(vpc1, vpc2, vpc3, colours[(i / 3) % 12]) : Triangle(vpc1, vpc2, vpc3, Vec4<float>(255, 255, 255, 255));
			Triangle t = Triangle(vpc1, vpc2, vpc3, colours[(i / 6) % 12]);
			
			// back face-culling
			//if (Vec3<float>::DotProduct(t.GetFaceNormal(), Vec3<float>()) < 0.f) { // will do my camera later.
				//continue;
			//}

			// then viewport transformation
			t.ViewportTransform(m_width, m_height);
			tris.push_back(t);
		}
	}
	// Then rasterizer. which has the fragment shader within it as it calcualtes the fragments per triangle.
	for (Triangle& tri : tris) {
		RasterizeTriangle(tri);
	}

	// Rasterization -> and z-buffer checks
	// pass each triangle to be rasterized, which also involves z-buffer check
	// rasterizer creates fragments for each triangle, which are passed to the fragment shader (and z-buffer I believe), which will update the values
	// this is also the section of interpolation from vertices to pixels values (like normal interpolation, and colours etc).

}

VertexOut Graphics::VertexShader(const VertexIn& vin, const Mat4<float>& MVP) {
	// no shading for now;
	Vec4<float> clippedPos = vin * MVP;
	//OutputDebugString(std::format(L"x: {},y: {},z: {}\n", vin.GetPosition().x, vin.GetPosition().y, vin.GetPosition().z).c_str());
	//OutputDebugString(std::format(L"cX: {},cY: {},cZ: {},cW: {}\n\n", clippedPos.x, clippedPos.y, clippedPos.z, clippedPos.w).c_str());
	return VertexOut(clippedPos);
}

FragmentOut Graphics::FragmentShader(const FragmentIn& fragIn) {
	FragmentOut fragOut;
	fragOut.colour = fragIn.colour;
	return fragOut;
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

void Graphics::RasterizeTriangle(const Triangle& tri) {
	VertexPostClip A = tri.GetVertexA();
	VertexPostClip B = tri.GetVertexB();
	VertexPostClip C = tri.GetVertexC();
	
	// get the bounding box:
	Vec3<float> posA = A.GetPosition();
	Vec3<float> posB = B.GetPosition();
	Vec3<float> posC = C.GetPosition();

	// GetBoundingBox(tri, &top, &left, &bottom, &right); // that calculates all of this bottom stuff without as much clutter, and does the floor/ceiling instead.

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
	int t = static_cast<int>(floorf(top));
	int b = static_cast<int>(ceilf(bottom));
	int l = static_cast<int>(floorf(left));
	int r = static_cast<int>(ceilf(right));

	// first calculate the edges of the triangle for the edge function.
	float C0x = posA.y - posB.y; // B->A
	float C0y = -(posA.x - posB.x);
	float C1x = posC.y - posA.y; // A->C
	float C1y = -(posC.x - posA.x);
	float C2x = posB.y - posC.y; // C->B
	float C2y = -(posB.x - posC.x);

	float startX = floorf(left) + 0.5f;
	float startY = floorf(top) + 0.5f;

	float edge0 = ((startX - posB.x) * (posA.y - posB.y)) - ((startY - posB.y) * (posA.x - posB.x)); // B->A
	float edge1 = ((startX - posA.x) * (posC.y - posA.y)) - ((startY - posA.y) * (posC.x - posA.x)); // A->C
	float edge2 = ((startX - posC.x) * (posB.y - posC.y)) - ((startY - posC.y) * (posB.x - posC.x)); // C->B


	// Total area of triangle for back-face culling.
	float area = ((posC.x - posB.x) * (posA.y - posB.y)) - ((posC.y - posB.y) * (posA.x - posB.x));
	if (area <= 0.f) {
		return;
	}

	float invArea = 1.0f / area;

	float e0, e1, e2;

	/*
	Should also get per pixel, theinterpolated attributes and other stuff.
	*/

	for (int y = t; y <= b; ++y) {
		// need to reset to the left side of our edge function, but also incorporate the row we are now on.
		int yDiff = (y - t);
		e0 = edge0 + (yDiff * C0y);
		e1 = edge1 + (yDiff * C1y);
		e2 = edge2 + (yDiff * C2y);

		for (int x = l; x <= r; ++x) {
			if ((e0 > 0 || e0 > -EPSILON && IsTopLeftEdge(posB, posA))
				&& (e1 > 0 || e1 > -EPSILON && IsTopLeftEdge(posA, posC))
				&& (e2 > 0 || e2 > -EPSILON && IsTopLeftEdge(posC, posB))) {

				// Varying Varyings = interpolate_varyings (bary, verts);
				float u = e2 * invArea; // (PBC) opposite A area.
				float v = e1 * invArea; // (PAC) opposite B area
				float w = e0 * invArea; // (PAB) opposite C area.

				if (u >= 0 && v >= 0 && w >= 0) {
					// interpolate z. We do this with invW for perspective correct interpolation
					float interpInvW = u * A.GetInvW() + v * B.GetInvW() + w * C.GetInvW();
					float interpZ = u * (posA.z * A.GetInvW()) + v * (posB.z * B.GetInvW()) + w * (posC.z * C.GetInvW());
					// Bring it back to screen space to get final interpoalted value.
					interpZ /= interpInvW;

					if (interpZ < 0) {
						OutputDebugString(std::format(L"interp-Z coord: {}", interpZ).c_str());
					}

					if (zBuffer[y * m_width + x] < interpZ) {
						e0 += C0x;
						e1 += C1x;
						e2 += C2x;
						continue;
					}


					// interpolate other vertex attributes

					// create fragmentinput with our x,y,z and list of other attributes all interpolated
					FragmentOut f = FragmentShader(FragmentIn(x, y, interpZ, tri.GetColour()));

					// run Fragment shader and get frag_out.
					// Get the output colour, or whatever else final would be.
					// if we reach here, update the depth buffer, and write to backbuffer
					PutPixel(x, y, f.colour);
					zBuffer[y * m_width + x] = interpZ;
				}
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

	ResetZBuffer();

	return 0;
}

HRESULT Graphics::ResizeWindow(int width, int height) {
	m_width = width;
	m_height = height;

	float fovY = PI / 2; // 90 degrees in radians.
	float yScaleFactor = tanf(fovY / 2);
	float aspectRatio = static_cast<float>(m_width) / static_cast<float>(m_height); // Might want to change this to a constant selection in an options menu later but oh well.

	float n{1}; // near is defined for some reason
	float f{1000}; // same for far. maybe for direct3d or something.

	// REALLY POORLY DONE, SHOULD CALL A FUNCTION THAT SIMPLY UPDATES THE PROJECTION MATRIX WITH THE ASPECT RATIO LOL.
	// Row-major order.
	projectionMatrix = Mat4<float>(
		{ 
		1/(yScaleFactor * aspectRatio), 0,				0,						  0,
		0,								1/yScaleFactor, 0,						  0,
		0,								0,				-(f/(f - n)),		 -1,
		0,								0,				-((f * n) / (f - n)), 0
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