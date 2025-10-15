#include "BindingTable/BindingTable.h"

void BindingTable::CallKeyboardAction(const InputEvent& e, GameState& state, char key) {
	std::function<void(const InputEvent& e, GameState& state)> func = keyboardActionMap[key];
	
	if (func == NULL) {
		return;
	}

	func(e, state);
}

void BindingTable::CallMouseAction(const InputEvent& e, GameState& state, InputEvent::EventType type) {
	std::function<void(const InputEvent& e, GameState& state)> func = mouseActionMap[type];

	if (func == NULL) {
		return;
	}

	func(e, state);
}