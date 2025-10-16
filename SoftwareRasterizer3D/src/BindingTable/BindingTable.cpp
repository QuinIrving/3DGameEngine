#include "BindingTable/BindingTable.h"

void BindingTable::CallKeyboardAction(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer, char key) {
	auto func = keyboardActionMap[key];
	
	if (func == NULL) {
		return;
	}

	func->Execute(e, state, eventBuffer);
}

void BindingTable::CallMouseAction(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer, InputEvent::EventType type) {
	auto func = mouseActionMap[type];

	if (func == NULL) {
		return;
	}

	func->Execute(e, state, eventBuffer);
}