#pragma once

// base class for our different types of game events that can be generated in the window message pump and passed to the 
// event buffer for processing after reading messages
// probably also want to keep track of a list of possible types of game events.
// these events should be single shot, not stuff that gets checked if it's still like that (our game state will handle that) ex. MoveForward.
class GameEvent {
public:
	enum class Type {
		CameraRotate, // Rotate camera (of main camera), and the degrees.
		Invalid
	};

public:
	GameEvent() = default;
	GameEvent(Type t) : t(t) {};

	Type GetType() const { return t; }

protected:
	Type t{ Type::Invalid };

};