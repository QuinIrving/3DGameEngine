#include "BindingTable/GameBindingTable.h"
#include "Game/Core/GameActions.h"

GameBindingTable::GameBindingTable() {
	keyboardActionMap['A'] = MouseMove; // always capital letters for keydown/up message.
	//keyboardActionMap['B'] = TestThing;

	mouseActionMap[InputEvent::EventType::LMB] = TestThing;
}