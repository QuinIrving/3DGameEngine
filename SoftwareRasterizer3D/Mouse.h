#pragma once
#include <utility>
#include "InputManager.h"

class Mouse {
public:
	Mouse() = default;

	int GetXPos() const;
	int GetYPos() const;
	std::pair<int, int> GetPos() const;

	void SetXPos(int);
	void SetYPos(int);
	void SetPos(int, int);

	void AddXPos(int);
	void AddYPos(int);
	void AddPos(int, int);

	bool IsLMBDown() const;
	bool IsRMBDown() const;
	bool IsMMBDown() const;

	void UpdateLMBDown(InputStatus);
	void UpdateRMBDown(InputStatus);
	void UpdateMMBDown(InputStatus);

	void SetAccumulator(int);
	int AddAccumulator(int);

private:
	bool lmbIsDown = false;
	bool rmbIsDown = false;
	bool mmbIsDown = false;

	int mwheelAccumulator = 0;
	int xPos = 0;
	int yPos = 0;
};

/*
We got a bunch of different types of mouse click events, Up & down
A.k.a pressed and released. LMB_UP, RMB_DOWN, etc

We also want to keep track of the mouse position last seen in the client area
at all times.

Perhaps on a mouse click event, it passes through also the coordinates of the mouse
at that moment as well.

So now we need a specific EventQueue that has a base event class
we can then pass a keyboard or mouse event, which has certain details.
Should also pass the key's currently down for the mouse event?
and potentially the mouse position when the button is pressed?
may not be needed as we could just simply check the event and poll the current
mouse position, but it would be somewhat laggy no? It will process it where it currently
is at, but not where it was pressed at?

Also want to keep track of scroll wheel.

int xPos = GET_X_LPARAM(lParam);
int yPos = GET_Y_LPARAM(lParam);

wParam has bitwise or flags of state of other mouse buttons plus
shift/ctrl keys

The absence of a flag means the corresponding button or key was not pressed
ex. wParam & MK_CONTROL

If needing to find state of other keys besides CTRl & SHIFT can use
GetKeyState

WM_LBUTONDBLCLK // double click should continue action that begins with the
first mouse click. ex. Single click -> select folder, double click -> opens folder

Non-client Mouse Messages.
WM_NCLBUTTONDOWN -> make sure to pass to DefWindowProc after anyways

To receive mouse-move messages past edge of window.

Call SetCapture function, as long as user holds at least one mouse button down

to release mouse capture use ReleaseCapture

Typicall when user presses left mouse button, call SetCapture to start capturing mouse
respond to mouse-move message
when user releases left mouse button call ReleaseCapture

call DPIScale::PixelsToDips to convert pixels to DIPs
that is to handle high-DPI settings correctly. DPIScale and all of that
not for us potentially?

DragDetect, for detecting drag on LBUTTONDOWN
^ if False suppreses WM_LBUTTONUP when user releases the mouse button
so don't call DragDetect unless program is in a mode that supports dragging

To Restrict Cursor, use ClipCursor, & get client coordinates to screen coordinates 
use ClientToScreen.

WM_MOUSELEAVE Hover/Leave

MouseWheel can check if MouseWheel is present.
WM_MOUSEWHEEL message if user rorates the mouse wheel
wParam parameter of message contains integer val delta for how far the wheel was rotated
120 units is defined as "one action",

sign of delta indicates direction:
Positive: rotate forward, away from user
Negative: rotate backward, toward user

val of delta is in wParam along with some flag.
Use GET_WHEEL_DELTA_WPARAM(wParam) to get the value of the delta.

if mouse wheel has a high resolution, abs val of delta might be less than 120.

Accumulate the total delta until the wheel rotates enough to perform the action. 
Store the unused delta in a variable and when 120 units accumulate (either positive or negative)
perform the action.

Meaning we should keep track of the delta in our mouse class.
*/