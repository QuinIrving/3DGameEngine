#pragma once
#include <utility>

class InputEvent { // maybe struct instead?
public:

	enum class EventType {
		LMB, // 0
		RMB, // 1
		MMB, // 2
		MWHEELDOWN, // 3
		MWHEELUP, // 4
		MOUSEMOVE, // 5
		// worry about double clicks later if we want.
		KEYBOARD,
		INVALID
	};

	InputEvent() = default;

	InputEvent(EventType type, char key, float deltaTime, std::pair<int, int> mousePos, bool isPressed = false, int wheelDelta = 0)
		: type(type), key(key), deltaTime(deltaTime), mousePos(mousePos), isPressed(isPressed), wheelDelta(wheelDelta) {}

	InputEvent(EventType type, char key, float deltaTime, int mouseX, int mouseY, bool isPressed = false, int wheelDelta = 0)
		: type(type), key(key), deltaTime(deltaTime), mousePos({mouseX, mouseY}), isPressed(isPressed), wheelDelta(wheelDelta) {}

public:
	const EventType type{ EventType::INVALID };
	const char key{ 0 }; // if LMB/RMB etc, it is the value of EventType enum.
	const float deltaTime{ 0 }; // time since last frame
	const bool isPressed{ false }; // true if keydown, false if keyup
	const std::pair<int, int> mousePos{}; // in pixels not DIPs for now (see if we should auto convert here?)
	// https://learn.microsoft.com/en-us/windows/win32/learnwin32/dpi-and-device-independent-pixels
	const int wheelDelta{ 0 };
	// difference of mouse position from before and now
	// textbuffer
	// keys held down,
	// mouse buttons held down
	// for deltaTime, may need to inject from winmain the deltaTime to my win.GameManager, and then from there we can retrieve it in Window.cpp? to get the
	// win.GameManager value for the deltaTime
};