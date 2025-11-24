#include "Graphics.h"

// Should abstract away from user. They shouldn't know about this private list of objs/entities
// when they create a new object, it should add it into this list. Some entity manager. That's for a later
// refactor.
static constexpr float GAMMA_CORRECTION = 1.f / 2.2f;
static constexpr float GAMMA_LINEAR = 2.2f;

void Graphics::Pipeline(const std::vector<VertexIn>& vertices, const std::vector<uint32_t>& indices, const ModelAttributes& modelAttributes, TVertexShader auto& VertexShader, TFragmentShader auto& FragmentShader) {
	const Mat4<float>& viewMatrix = camera.GetViewMatrix(); // could technically optimize this by pre-computed VP once a frame, not once per object.
	std::vector<VertexOut> clipVertices;
	clipVertices.reserve(vertices.size());

	// Vertex Shader:
	for (const VertexIn &v : vertices) {
		clipVertices.push_back(VertexShader(v, modelAttributes, viewMatrix, projectionMatrix));
	}

	std::vector<Triangle> tris;
	tris.reserve(indices.size() / 3);

	// Clipping check:
	for (int i = 0; i < indices.size(); i += 3) {
		VertexOut& v1 = clipVertices[indices[i]];
		VertexOut& v2 = clipVertices[indices[i + 1]];
		VertexOut& v3 = clipVertices[indices[i + 2]];

		// triangle isn't within our view frustum at all, just cull the triangle completely
		if (!VertexOut::IsTriangleInFrustum(v1, v2, v3)) {
			continue;
		}

		std::vector<int> postClipIds;
		std::vector<VertexPostClip> v; // Not sure if we want to copy instead, reference is probably better. Figure it out in a later refactor.
		v.reserve(6); // max possible amount of vertices after clipping
		postClipIds.reserve(6); // same for the id's. Max is 2 triangles, with 6 total id's.

		// Only will do near-plane for partial clipping for efficiency purposes.
		if (v1.IsNotInNearFrustum() || v2.IsNotInNearFrustum() || v3.IsNotInNearFrustum()) {
			// this will require clipping the vertices, putting them into our v, and ensuring we keep winding order for our
			// postClipIds vector as well.
			// clipping can be done, and as we know it's the near plane, we must interpoalte between the z values to the near plane value,
			// which in this case should be 0, as we map from 0->w instead of -w, w with our projection matrix.

			Vec4<float> pos1 = v1.GetPosition();
			Vec4<float> pos2 = v2.GetPosition();
			Vec4<float> pos3 = v3.GetPosition();

			if (v1.IsNotInNearFrustum()) {
				if (v2.IsNotInNearFrustum()) {
					//OutputDebugString(L"v1 & v2 out\n");
					ClipTwoOut(v, postClipIds, v1, v2, v3);
				}
				else if (v3.IsNotInNearFrustum()) {
					//OutputDebugString(L"v1 & v3 out\n"); //
					ClipTwoOut(v, postClipIds, v3, v1, v2);
				}
				else {
					//OutputDebugString(L"v1 only out\n"); //
					ClipOneOut(v, postClipIds, v1, v3, v2);
				}
			}
			else if (v2.IsNotInNearFrustum()) {
				// only need to worry about v3, as v1 must already be in the near frustum
				if (v3.IsNotInNearFrustum()) {
					//OutputDebugString(L"v2 & v3 out\n");
					ClipTwoOut(v, postClipIds, v2, v3, v1);
				}
				else {
					//OutputDebugString(L"v2 only out\n");
					ClipOneOut(v, postClipIds, v2, v1, v3);
				}
			}
			else {
				// v3 can't also have v1, or v2 due to prior if statements.
				ClipOneOut(v, postClipIds, v3, v2, v1);
				//OutputDebugString(L"v3 only out\n");
			}

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
			// Using world position, as we don't want to utilize 
			Triangle t = Triangle(vpc1, vpc2, vpc3, Triangle::ComputeFaceNormal(vpc1.GetViewPosition(), vpc2.GetViewPosition(), vpc3.GetViewPosition()), 
								Triangle::ComputeFaceNormal(vpc1.GetWorldPosition(), vpc2.GetWorldPosition(), vpc3.GetWorldPosition()), colours[(i / 6) % 12]);
		
			if (Vec3<float>::DotProduct(t.GetFaceNormal(), vpc1.GetViewPosition()) > 0) {
				continue;
			}

			// then viewport transformation
			t.ViewportTransform(m_width, m_height);
			tris.push_back(t);
		}
	}
	// Then rasterizer. which has the fragment shader within it as it calcualtes the fragments per triangle.
	for (Triangle& tri : tris) {
		RasterizeTriangle(tri, modelAttributes, FragmentShader);
	}

	// Rasterization -> and z-buffer checks
	// pass each triangle to be rasterized, which also involves z-buffer check
	// rasterizer creates fragments for each triangle, which are passed to the fragment shader (and z-buffer I believe), which will update the values
	// this is also the section of interpolation from vertices to pixels values (like normal interpolation, and colours etc).

}

void Graphics::RasterizeTriangle(const Triangle& tri, const ModelAttributes& modelAttributes, TFragmentShader auto& FragmentShader) {
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
	float C0x = posB.y - posA.y; // A->B
	float C0y = -(posB.x - posA.x);
	float C1x = posC.y - posB.y; // B->C
	float C1y = -(posC.x - posB.x);
	float C2x = posA.y - posC.y; // C->A
	float C2y = -(posA.x - posC.x);

	float startX = floorf(left) + 0.5f;
	float startY = floorf(top) + 0.5f;

	float edge0 = ((startX - posA.x) * (posB.y - posA.y)) - ((startY - posA.y) * (posB.x - posA.x)); // A->B
	float edge1 = ((startX - posB.x) * (posC.y - posB.y)) - ((startY - posB.y) * (posC.x - posB.x)); // B->C
	float edge2 = ((startX - posC.x) * (posA.y - posC.y)) - ((startY - posC.y) * (posA.x - posC.x)); // C->A


	// Total area of triangle for back-face culling.
	float area = (((posC.x - posB.x) * (posA.y - posB.y)) - ((posC.y - posB.y) * (posA.x - posB.x)));
	if (area <= 0.f) { // <=
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

		if (y < 0) {
			y = -1; // so continue will get to the correct value of 0;
			continue;
		}
		else if (y >= m_height) {
			break;
		}
		
		e0 = edge0 + (yDiff * C0y);
		e1 = edge1 + (yDiff * C1y);
		e2 = edge2 + (yDiff * C2y);

		for (int x = l; x <= r; ++x) {
			if (x < 0) {
				int diff = -x;
				e0 += diff * C0x;
				e1 += diff * C1x;
				e2 += diff * C2x;
				x = -1; // so continue will get to the correct value of 0;
				continue;
			}
			else if (x >= m_width) {
				break;
			}

			if ((e0 < 0 || e0 < EPSILON && IsTopLeftEdge(posA, posB))
				&& (e1 < 0 || e1 < EPSILON && IsTopLeftEdge(posB, posC))
				&& (e2 < 0 || e2 < EPSILON && IsTopLeftEdge(posC, posA))) {

				// Varying Varyings = interpolate_varyings (bary, verts);
				float u = -e1 * invArea; // (PBC) opposite B (B->C).
				float v = -e2 * invArea; // (PAC) opposite C (C->A)
				float w = -e0 * invArea; // (PAB) opposite A (A->B).

				if (u >= 0 && v >= 0 && w >= 0) {
					// interpolate z. We do this with invW for perspective correct interpolation
					float interpInvW = u * A.GetInvW() + v * B.GetInvW() + w * C.GetInvW();
					float interpZ = u * (posA.z * A.GetInvW()) + v * (posB.z * B.GetInvW()) + w * (posC.z * C.GetInvW());
					Vec3<float> interpVertNormal = (A.GetNormal() * A.GetInvW()) * u + (B.GetNormal() * B.GetInvW()) * v + (C.GetNormal() * C.GetInvW()) * w;
					Vec2<float> interpUV = (A.GetUV() * A.GetInvW()) * u + (B.GetUV() * B.GetInvW()) * v + (C.GetUV() * C.GetInvW()) * w;
					Vec3<float> interpTangent = (A.GetTangent() * A.GetInvW()) * u + (B.GetTangent() * B.GetInvW()) * v + (C.GetTangent() * C.GetInvW()) * w;
					Vec3<float> interpBitangent = (A.GetBitangent() * A.GetInvW()) * u + (B.GetBitangent() * B.GetInvW()) * v + (C.GetBitangent() * C.GetInvW()) * w;
					// Bring it back to screen space to get final interpolated value.
					interpZ /= interpInvW;
					interpVertNormal /= interpInvW;
					interpUV /= interpInvW;
					interpTangent /= interpInvW;
					interpBitangent /= interpInvW;

					if (interpZ < 0) {
						OutputDebugString(std::format(L"interp-Z coord: {}\n", interpZ).c_str());
						continue;
					}

					if (zBuffer[y * m_width + x] < interpZ) {
						e0 += C0x;
						e1 += C1x;
						e2 += C2x;
						continue;
					}

					//!!!! MAY WANT TO CALCULATE BITANGENT ON THE FLY PER FRAGMENT, RATHER THAN IN VERTEX SHADER.

					// interpolate other vertex attributes
					//FragmentIn(int x, int y, float z, const Vec3<float>& normal, const Vec4<float>& colour, const Vec2<float>& uv, const Vec3<float>& tangent, const Vec3<float>& bitangent) 
					// create fragmentinput with our x,y,z and list of other attributes all interpolated
					FragmentOut f = FragmentShader(FragmentIn(x, y, interpZ, interpVertNormal, tri.GetFaceNormal(), tri.GetFaceWorldNormal(), tri.GetColour(), interpUV, interpTangent, interpBitangent), modelAttributes.material);

					f.colour = Vec4<float>((Vec3<float>(f.colour) ^ GAMMA_CORRECTION), f.colour.w);
					f.colour *= 255.f;
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