#pragma once
//#include <map>
#include <unordered_map>
#include <functional>
#include "Input/InputEvent.h"
#include "Game/Core/GameState.h"

class BindingTable {
	/*
	We need a map of key to function, and function can be dynamically mapped (re-mapping of keys allowed).
	Remapping is only allowed for everything except for settings, so pressing ~ always pulls up the settings and will never change how you can
	update your settings.

	map<keyChar, action>
	that action can either generate an event, or update a state, or do some basic coniditional, such as in level designer, check if object is clicked
	and if so generate an object move event for that specific object with the mouse in the direction,
	else just do regular game movement of the camera.

	Need some basic values on initialization, maybe a txt file that we can read in, as we will update it on remap save so when user re-opens
	window it will still work as wanted.
	*/

public:
	BindingTable() = default; // need to link to some initialization that maps our keys and mouse from default (and saves the re-mapped versions? with a default provided still)

	void CallKeyboardAction(const InputEvent& e, GameState& state, char key); // just being explicit, not technically needed lol.
	void CallMouseAction(const InputEvent& e, GameState& state, InputEvent::EventType type); // just being explicit like key, eventype isn't technically needed as e contains it.

protected:
	// may want 1 map for keyboard stuff, a different map for mouse stuff.
	std::unordered_map<char, std::function<void(const InputEvent& e, GameState& state)>> keyboardActionMap;

	// mwheelup vs mwheel down? not sure if it matters, seems to be different in most game mappings
	// LMB, RMB, MMB, MouseMove. MouseMove on held? not sure how we are going to handle all of this, maybe don't care and handle in function
	//I guess mouse move will not be re-bindable, as I will only use it for looking around game area, or adjusting objects/blocks?
	std::unordered_map<InputEvent::EventType, std::function<void(const InputEvent& e, GameState& state)>> mouseActionMap;

	// can either update state or trigger events or both.
};