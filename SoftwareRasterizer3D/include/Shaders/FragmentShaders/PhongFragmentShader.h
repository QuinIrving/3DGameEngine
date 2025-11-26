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
	// reinhard tone mapping:
	//rgb = rgb / (rgb + Vec3<float>(1.f, 1.f, 1.f));

	// Luminance reinhard tone mapping:
	/*float luminance = Vec3<float>::DotProduct(rgb, Vec3<float>(0.2126f, 0.7152f, 0.0722f));
	float luminanceToneMapped = luminance / (luminance + 1.f);
	rgb = rgb * (luminanceToneMapped / luminance);*/

	// Luminance reinhard extended tone mapping:
	/*float luminance = Vec3<float>::DotProduct(rgb, Vec3<float>(0.2126f, 0.7152f, 0.0722f));
	float maxWhiteL = directionLight.intensity;
	float numerator = luminance * (1.f + (luminance / (maxWhiteL * maxWhiteL)));
	float luminanceToneMapped = numerator / (luminance + 1.f);
	rgb = rgb * (luminanceToneMapped / luminance);*/

	// Hable filmic tone mapping (uncharted 2)
	/*float A = 0.15f;
	float B = 0.50f;
	float C = 0.10f;
	float D = 0.20f;
	float E = 0.02f;
	float F = 0.30f;
	
	float exposure_bias = 2.0f;
	rgb *= exposure_bias;
	Vec3<float> curr = ((rgb * (rgb * A + C * B) + D * E) / (rgb * (rgb * A + B) + D * F)) - E / F;
	Vec3<float> W = Vec3<float>(11.2f, 11.2f, 11.2f);
	Vec3<float> whiteScale = Vec3<float>(1.0f, 1.0f, 1.0f) / ((W * (W * A + C * B) + D * E) / (W * (W * A + B) + D * F)) - E / F;
	rgb = curr * whiteScale;*/

	//Aces tone mapping
	/*Mat4<float> acesInputMatrix = {{0.59719f, 0.07600f, 0.02840f, 0.0f,
									 0.35458f, 0.90834f, 0.13383f, 0.0f,
									 0.04823f, 0.01566f, 0.83777f, 0.0f,
									 0.0f,     0.0f,     0.0f,     1.0f} };

	Mat4<float> acesOutputMatrix = { {1.60475f, -0.10208f, -0.00327f, 0.0f,
									  -0.53108f, 1.10813f, -0.07276f, 0.0f,
									  -0.07367f, -0.00605f, 1.07602f, 0.0f,
									   0.0f,	  0.0f,     0.0f,     1.0f} };
	rgb = Vec4<float>(rgb) * acesInputMatrix;
	Vec3<float> a = rgb * (rgb + 0.0245786f) - 0.000090537f;
	Vec3<float> b = rgb * (rgb * 0.983729f + 0.4329510f) + 0.238081f;
	rgb = a / b;
	rgb = Vec4<float>(rgb) * acesOutputMatrix;*/

	// Aces approximated tone mapping (for performance)
	rgb = rgb * 0.6f;
	float a = 2.51f;
	float b = 0.03f;
	float C = 2.43f;
	float d = 0.59f;
	float e = 0.14f;
	rgb = (rgb * (rgb * a + b)) / (rgb * (rgb * C + d) + e);
	rgb.x = std::clamp(rgb.x, 0.0f, 1.0f);
	rgb.y = std::clamp(rgb.y, 0.0f, 1.0f);
	rgb.z = std::clamp(rgb.z, 0.0f, 1.0f);

	//GT tone mapping
	/*Vec3<float> numerator = rgb * (rgb * 0.3f + 0.06f) + 0.003f;
	Vec3<float> denominator = rgb * (rgb * 0.3f + 0.6f) + 0.2f;
	rgb = numerator / denominator;
	//rgb = std::clamp(numerator / denominator, Vec3<float>(0.0f, 0.0f, 0.0f), Vec3<float>(1.0f, 1.0f, 1.0f));*/

	/*************
	Stylized Tone maps
	 *************/
	// Cross-channel max
	/*float m = std::max(rgb.x, std::max(rgb.y, rgb.z));
	float tm = m / (m + 1.0);
	rgb = (rgb + 0.0001).GetNormalized() * tm;*/

	// Log-saturation push
	/*float luminance = Vec3<float>::DotProduct(rgb, Vec3<float>(0.2126f, 0.7152f, 0.0722f));
	float saturationBoost = std::pow(1.0f - std::clamp(luminance, 0.0f, 1.0f), 1.3);

	//rgb = rg*/

	// Inverted shoulder
	//rgb = (rgb ^ 0.8f) * ( -(rgb ^ 0.8f) + 1.f);

	fragOut.colour = { rgb.x, rgb.y, rgb.z, c.a};
	return fragOut;
}