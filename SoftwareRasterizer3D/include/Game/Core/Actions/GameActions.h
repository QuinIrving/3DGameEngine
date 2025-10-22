#pragma once
#include "IGameAction.h"
#include "Input/InputEvent.h"
#include "Game/Core/GameState.h"
#include "Game/Core/Events/GameEvent.h"
#include "Game/Core/Events/CameraRotate.h"
#include "Math/GeneralMath.h"
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <format>
#include <queue>

/*
onMouseMove, need to update camera via new positions in x/y to be the angles for rotation of the camera.
wasd needs to contribute to move, however, it will be while held, and acceleration based in game, current camera, while held it simply updates values.


On input event, need Mouse pos(x,y), wheel delta, what is currently held down, what key (or button) was the one that got pressed/triggered now
a bool for whether it was released or pressed/pushed down
deltaTime for time since last frame

all of them need to be able to access the current game state, to be able to modify it, and they need to be able to send back some form of event back
to the game for processing in the logic section of the game loop before rendering.

*/

class MouseMoveAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		// is a release commmand, don't care.
		if (!e.isPressed) {
			return;
		}
		
		//DEGREE_TO_RADIANS;
		
		Vec2<float> t = e.prevMousePosDiff;
		float pitch = -t.y * state.MouseSensitivity;
		float yaw = -t.x * state.MouseSensitivity;

		eventBuffer.push(std::make_unique<CameraRotate>(pitch, yaw));
	}
};

class MouseClickAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		// is a release commmand, don't care.
		if (!e.isPressed) {
			return;
		}

		OutputDebugString(std::format(L"Test Delta Time: {}\n", e.deltaTime).c_str());
	}
};

class MoveForwardAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		if (!e.isPressed) {
			state.MovingForward = false;
			return;
		}

		state.MovingForward = true;
	}
};

class MoveBackwardsAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		if (!e.isPressed) {
			state.MovingBackward = false;
			return;
		}

		state.MovingBackward = true;
	}
};

class MoveLeftAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		if (!e.isPressed) {
			state.MovingLeft = false;
			return;
		}

		state.MovingLeft = true;
	}
};

class MoveRightAction : public IGameAction {
public:
	void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) override {
		if (!e.isPressed) {
			state.MovingRight = false;
			return;
		}

		state.MovingRight = true;
	}
};

/*
void MouseMove(const InputEvent& e, GameState& state) {
	// update camera?
	// take x and y coords and utilize the difference from current state object to get then apply rotation as necessary to the camera.
	OutputDebugString(L"Test Mouse Move\n");
}

void TestThing(const InputEvent& e, GameState& state) {
	OutputDebugString(std::format(L"Test Delta Time: {}\n", e.deltaTime).c_str());
}*/