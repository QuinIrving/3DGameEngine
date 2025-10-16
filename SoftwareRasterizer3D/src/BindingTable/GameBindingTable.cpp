#include "BindingTable/GameBindingTable.h"
#include "Game/Core/Actions/GameActions.h"

GameBindingTable::GameBindingTable() {
	keyboardActionMap['A'] = std::make_shared<MouseMoveAction>(); // always capital letters for keydown/up message.
	keyboardActionMap['B'] = std::make_shared<MouseMoveAction>();

	mouseActionMap[InputEvent::EventType::LMB] = std::make_shared<MouseClickAction>();
}