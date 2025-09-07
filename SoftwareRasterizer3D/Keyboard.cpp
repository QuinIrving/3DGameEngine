#include "Keyboard.h"

bool Keyboard::IsKeyPressed(uint8_t key) {
	if (key < 0 || key >= 256) {
		return false;
	}

	return keyStates.test(key);
}

void Keyboard::OnKeyDown(uint8_t key) {
	if (key < 0 || key >= 256) {
		return;
	}

	keyStates.set(key, true);
}

void Keyboard::OnKeyUp(uint8_t key) {
	if (key < 0 || key >= 256) {
		return;
	}

	keyStates.set(key, false);
}