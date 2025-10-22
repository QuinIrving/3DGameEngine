#pragma once

struct GameState {
	float MoveSpeed = 8.5f;
	bool MovingForward{ false };
	bool MovingBackward{ false };
	bool MovingLeft{ false };
	bool MovingRight{ false };
	// enum for Fps mode, instead of true/false. So we can have multiple camera modes
	bool InFpsCameraMode{ true };
	float MouseSensitivity{ 5.f };
	// resolution
	bool IsCursorContainedInScreen{ false };
};