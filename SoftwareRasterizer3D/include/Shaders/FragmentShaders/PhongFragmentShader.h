#pragma once
#include "Shaders/ShaderSignatures.h"
#include "Graphics/ModelAttributes.h"
#include <algorithm>

/**********
Custom Addition for testing out flat shading in basic respect.
*******/
#include "Scene/Lights/Delta/DirectionalLight.h"

/**
End of custom addition
*/

FragmentOut PhongFragmentShader(const FragmentIn& fragIn, const Material& material) {
	FragmentOut fragOut;
	Colour c;

	// no textures, so just handle base colour.
	if (!material.albedoTexture || *material.albedoTexture == NULL) {
		c = Colour(material.baseColour);
	}
	else {
		// some other flows here needed to add in as we add in PBR.
		switch (material.samplingMode) {
		case TextureSamplingMode::Nearest:
			c = (*material.albedoTexture)->SampleNearest(fragIn.uv.x, fragIn.uv.y);
			break;
		case TextureSamplingMode::Bilinear:
			c = (*material.albedoTexture)->SampleBilinear(fragIn.uv.x, fragIn.uv.y);
			break;
		default:
			c = { 1.f, 1.f, 1.f, 1.f }; // should be base colour instead.
		}

		// need to transform sRGB to linear space for albedo values:
		c = Colour(c.GetVectorizedValues() ^ GAMMA_LINEAR, c.a);
	}

	/**********
	Custom Addition for testing out flat shading in basic respect.
	*******/
	Vec3<float> normal = fragIn.pointNormal;
	if (material.normalMap && *material.normalMap != NULL) {
		Mat4<float> TBN = {{ fragIn.tangent.x,		fragIn.tangent.y,		fragIn.tangent.z,		0,
							 fragIn.bitangent.x,	fragIn.bitangent.y,		fragIn.bitangent.z,		0,
							 fragIn.pointNormal.x,	fragIn.pointNormal.y,	fragIn.pointNormal.z,	0,
							 0,						0,						0,						1} };

		Colour nSample = (*material.normalMap)->SampleNearest(fragIn.uv.x, fragIn.uv.y);
		Vec3<float> n = (nSample.GetVectorizedValues() * 2.0f) - Vec3<float>(1.f, 1.f, 1.f);
		n.y *= -1; // flipping b/c openGL and directX textures winding, but since the texture map is also wrong this should be fine. Should probably change around our stuff
		// though so we can pass in the handedness to determine what to do with whichever texture. 
		normal = (Vec4<float>(n, 0.f) * TBN).GetNormalized();
	}

	// fancy name just for the diffuse intensity
	float lambertianReflectance = std::max(0.0f, normal.DotProduct(-directionLight.direction));
	lambertianReflectance *= directionLight.intensity;
	// don't forget about colour, and looping through later, and better ways to do some of this.

	c *= lambertianReflectance;
	/**
	End of custom addition
	*/

	Vec3<float> rgb = c.GetVectorizedValues();
	// reinhard tone mapping
	rgb = rgb / (rgb + Vec3<float>(1.f, 1.f, 1.f));

	fragOut.colour = { rgb.x, rgb.y, rgb.z, c.a};
	return fragOut;
}