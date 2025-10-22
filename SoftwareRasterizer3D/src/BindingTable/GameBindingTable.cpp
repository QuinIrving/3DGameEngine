#include "BindingTable/GameBindingTable.h"
#include "Game/Core/Actions/GameActions.h"

GameBindingTable::GameBindingTable() {
	//keyboardActionMap['A'] = std::make_shared<MouseMoveAction>(); // always capital letters for keydown/up message.
	//keyboardActionMap['B'] = std::make_shared<MouseMoveAction>();
	
	// Default Keyboard Bindings:
	keyboardActionMap['W'] = std::make_shared<MoveForwardAction>();
	keyboardActionMap['A'] = std::make_shared<MoveLeftAction>();
	keyboardActionMap['S'] = std::make_shared<MoveBackwardsAction>();
	keyboardActionMap['D'] = std::make_shared<MoveRightAction>();


	// Default Mouse Bindings:
	mouseActionMap[InputEvent::EventType::LMB] = std::make_shared<MouseClickAction>();
	mouseActionMap[InputEvent::EventType::MOUSEMOVE] = std::make_shared<MouseMoveAction>();
}