#include "Game/Core/GameManager.h"

GameManager& GameManager::GetHandle(BindingTable& inputMap) {
	static GameManager g{inputMap};
	return g;
}

void GameManager::HandleInput(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer, Keyboard& kbd) {
	if (e.type == InputEvent::EventType::INVALID) {
		return;
	}

	if (e.type == InputEvent::EventType::KEYBOARD) {
		inputMap.CallKeyboardAction(e, state, eventBuffer, e.key);

		// if it's a specific type of action, we can also clear the text buffer now, either pass in the function to CallKeyboard Action or do a check if it's
		// an action that will do the specific thing <- probably best way.
		// so we grab the action, see the name, and then clear it if we want? or just pass the clear function into the action.

		return;
	}

	inputMap.CallMouseAction(e, state, eventBuffer, e.type);
}