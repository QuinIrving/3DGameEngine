#pragma once
#include "Input/InputEvent.h"
#include "GameState.h"
#define WIN32_LEAN_AND_MEAN
#include "Windows.h"
#include <format>

/*
onMouseMove, need to update camera via new positions in x/y to be the angles for rotation of the camera.
wasd needs to contribute to move, however, it will be while held, and acceleration based in game, current camera, while held it simply updates values.


On input event, need Mouse pos(x,y), wheel delta, what is currently held down, what key (or button) was the one that got pressed/triggered now
a bool for whether it was released or pressed/pushed down
deltaTime for time since last frame

all of them need to be able to access the current game state, to be able to modify it, and they need to be able to send back some form of event back
to the game for processing in the logic section of the game loop before rendering.

*/

void MouseMove(const InputEvent& e, GameState& state) {
	// update camera?
	// take x and y coords and utilize the difference from current state object to get then apply rotation as necessary to the camera.
	OutputDebugString(L"Test Mouse Move\n");
}

void TestThing(const InputEvent& e, GameState& state) {
	OutputDebugString(std::format(L"Test Delta Time: {}\n", e.deltaTime).c_str());
}