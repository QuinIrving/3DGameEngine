#include "Input/Keyboard.h"

bool Keyboard::IsKeyPressed(uint8_t key) const {
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
	keysHeldDown.insert(key);
}

void Keyboard::OnKeyUp(uint8_t key) {
	if (key < 0 || key >= 256) {
		return;
	}

	keyStates.set(key, false);
	keysHeldDown.erase(key);
}

void Keyboard::ClearTextBuffer() {
	std::queue<wchar_t> empty;
	std::swap(empty, textBuffer);
}

void Keyboard::AppendText(wchar_t c) {
	textBuffer.push(c);
	while (textBuffer.size() > MAX_BUFFER) {
		textBuffer.pop();
	}
}

wchar_t Keyboard::PopTextChar() {
	if (textBuffer.empty()) {
		return 0;
	}

	wchar_t c = textBuffer.front();
	textBuffer.pop();
	return c;
}

bool Keyboard::IsTextEmpty() const {
	return textBuffer.empty();
}