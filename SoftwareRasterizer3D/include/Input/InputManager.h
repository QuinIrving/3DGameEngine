#pragma once

struct InputState {
	enum class InputStatus : bool {
		DOWN = true,
		UP = false,
	} value;

	constexpr InputState(InputStatus v) : value(v) {}
	constexpr operator bool() const { return value == InputStatus::DOWN; }
};

/*enum class InputStatus : bool {
	DOWN = true,
	UP = false
};*/