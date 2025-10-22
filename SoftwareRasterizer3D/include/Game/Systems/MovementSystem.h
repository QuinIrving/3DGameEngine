#pragma once
#include "Math/Vec3.h"
#include "Game/Core/GameState.h"

class MovementSystem {
public:
	static Vec3<float> ComputeTranslation(const GameState& state, float deltaTime);
};