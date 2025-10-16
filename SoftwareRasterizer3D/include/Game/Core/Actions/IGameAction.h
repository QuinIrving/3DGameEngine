#pragma once
#include "Game/Core/GameState.h"
#include "Input/InputEvent.h"
#include "Game/Core/Events/GameEvent.h"
#include <queue>

// Interface for all of the different game actions we will have. Could allow for stuff like replays. Also might be good when I add in networking.
class IGameAction {
public:
	virtual ~IGameAction() = default;
	virtual void Execute(const InputEvent& e, GameState& state, std::queue<std::unique_ptr<GameEvent>>& eventBuffer) = 0;
};