#pragma once
#include "BindingTable/BindingTable.h"
#include "Input/InputEvent.h"
#include "GameState.h"
#include "Input/Keyboard.h"

class GameManager {
public:
	static GameManager& GetHandle(BindingTable& inputMap);

	// handle key/button.
	void HandleInput(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer, Keyboard& kbd);
	GameState& GetGameState() { return state; }
	void UpdateDeltaTime(float updatedTime) { deltaTime = updatedTime; }
	float GetDeltaTime() { return deltaTime; }

	// my main idea is this:
	// Need an event buffer for any one-off stuff, such as mouse move which will trigger a rotation,
	// while actions that are for held stuff goes into my game state,
	// such as moveLeft held down, as then we can keep updating state in the messages quickly, and in processing after messages check on those game states
	// to do the different work per frame.
	// Events for single-action are created in message processing to be read in a queue, and should have minimal processing in and of themselves.
	//

private:
	BindingTable& inputMap;
	GameState state;
	float deltaTime{ 0.f };

private: // singleton initialization
	GameManager(BindingTable& inputMap) : inputMap(inputMap), state() {};
	GameManager(const GameManager&) = delete;
	GameManager& operator=(const GameManager&) = delete;
	GameManager(GameManager&&) = delete;
	GameManager& operator=(GameManager&&) = delete;
};