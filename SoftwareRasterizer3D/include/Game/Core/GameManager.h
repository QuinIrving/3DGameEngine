#pragma once
#include "BindingTable/BindingTable.h"
#include "Input/InputEvent.h"
#include "GameState.h"
#include "Input/Keyboard.h"

class GameManager {
public:
	static GameManager& GetHandle(BindingTable& inputMap);

	// handle key/button.
	void HandleInput(const InputEvent& e, GameState& state, Keyboard& kbd);
	GameState& GetGameState() { return state; }
	void UpdateDeltaTime(float updatedTime) { deltaTime = updatedTime; }
	float GetDeltaTime() { return deltaTime; }

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