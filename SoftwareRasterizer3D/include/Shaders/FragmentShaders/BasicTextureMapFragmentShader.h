#pragma once
#include "Shaders/ShaderSignatures.h"
#include "Graphics/Texture.h"

static const Texture staticTestCobble{ "resources/Textures/Cobblestone/CobblestoneWall01_1K_BaseColor.png" };
static const Texture staticTestStone{ "resources/Textures/StoneWall/StoneWall03_2K_BaseColor.png" };

FragmentOut BasicTextureMapFragmentShader(const FragmentIn& fragIn) {
	// only for now, will make this better in a bit.
	Colour c = staticTestStone.Sample(fragIn.uv.x, fragIn.uv.y);
	return FragmentOut(c.r * 255.f, c.g * 255.f, c.b * 255.f, c.a * 255.f);

	// should really change from concepts to a runtime base class, so that we can just accept another parameter just for this type of class which is the texture to read in from
	// will do a test without this refactor to just see how textures are doing.
}