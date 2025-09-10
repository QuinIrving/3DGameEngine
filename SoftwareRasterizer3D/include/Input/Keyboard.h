#pragma once
#include <bitset>
#include <queue>
#include "InputManager.h"

class Keyboard {
public:
	Keyboard() = default;

	class KeyboardEvent {};

	void OnKeyUp(uint8_t key);
	void OnKeyDown(uint8_t key);
	bool IsKeyPressed(uint8_t key) const;

	void ClearTextBuffer();
	void AppendText(wchar_t c);
	wchar_t PopTextChar();
	bool IsTextEmpty() const;
	
private:
	std::bitset<256> keyStates;
	std::queue<wchar_t> textBuffer;
	static constexpr int MAX_BUFFER = 128;
};

/*
Keyboard has character input (text), Keyboard shotcuts and System commands
if ALT key is held down, pressing A key produces ALT+A
- Need to ensure we keep handling our keys even if special keys are pressed
- virtual-key codes don't generate ASCII codes since it's just the key
itself language independent.
0-9 keys as ascii 0x30-0x39,
ASCII 'A'-'Z' 0x41-0x5A

VK_LEFT for left arrow. 

NOTHING FOR A, so just use the numeric values.

WM_KEYDOWN, and WM_SYSKEYDOWN messages

ALT + any key, F10 for System messages (need this to extract the ALT + A) A key
for example.

WM_KEYUP, WM_SYSKEYUP same stuff here.

use bitset for our case
std::bitset<256>

if key is held down enough it will send multiple key-down messages followed
by a single key-up.

wParam is virtual-key code of key.
lParam is misc info. 
bit 30 in lParam is 'prev key state' flag. set to 1 for repeated key-down messages
So we only want to process the key-down once, and as such we can 
check if that is 1, if so then don't process the key-down anymore, as we already set it
wait until a keyup.

Use TranslateMessage to get keystrokes -> characters. 

WM_CHAR & WM_SYSCHAR the WM_CHAR gets the actual value of the key as a text
so SHIFT + A is WM_KEYDOWN Shift, WM_KEYDOWN A, WM_CHAR 'A'.

ALT + P

Is SYSKEYDOWN VK_MENU, SYSKEYDOWN 0X50,
SYSCHAR 'p'
WM_SYSKEYUP 0x50, WM_KEYUP VK_MENU

Alt is for VK_MENU.

to implement keyboard shortcuts use an accelerator table.

can test state of a key at any time via GetKeyState.
*/