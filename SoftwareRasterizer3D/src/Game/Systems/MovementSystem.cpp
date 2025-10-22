#include "Game/Systems/MovementSystem.h"

Vec3<float> MovementSystem::ComputeTranslation(const GameState& state, float deltaTime) {
	Vec3<float> v = Vec3<float>();

	if (state.MovingForward) {
		v.z -= state.MoveSpeed * deltaTime;
	}

	if (state.MovingBackward) {
		v.z += state.MoveSpeed * deltaTime;
	}

	if (state.MovingLeft) {
		v.x -= state.MoveSpeed * deltaTime;
	}

	if (state.MovingRight) {
		v.x += state.MoveSpeed * deltaTime;
	}

	return v;
}