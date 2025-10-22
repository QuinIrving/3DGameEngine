#include <windowsx.h>
#include "Window/Window.h"
#include <format>

LRESULT CALLBACK Window::WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	Window* pThis = NULL;

	if (uMsg == WM_NCCREATE) {
		CREATESTRUCT* pCreate = (CREATESTRUCT*)lParam;
		pThis = (Window*)pCreate->lpCreateParams;
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR)pThis);

		pThis->hwnd = hwnd;
	}
	else {
		pThis = (Window*)GetWindowLongPtr(hwnd, GWLP_USERDATA);
	}

	if (pThis) {
		return pThis->HandleMessage(uMsg, wParam, lParam);
	}
	
	// if pThis is NULL just default processing.
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

BOOL Window::Create(PCWSTR lpWindowName, DWORD dwStyle, DWORD dwExStyle,
	int x, int y, HWND hWndParent, HMENU hMenu) {

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_DBLCLKS

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		width, height, hWndParent, hMenu, GetModuleHandle(NULL), this
	);

	if (hwnd) {
		g_DPIScale = static_cast<float>(GetDpiForWindow(hwnd)) / USER_DEFAULT_SCREEN_DPI;
	}

	return (hwnd ? TRUE : FALSE);
}

LRESULT Window::HandleMessage(UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg)
	{
	/****************************************************
	*******            Keyboard Messages            *****
	****************************************************/
	case WM_SYSKEYDOWN:
	case WM_KEYDOWN:
	{
		// this means we are handling a "repeat" keydown message - ignore.
		if ((lParam & (1 << 30)) != 0) {
			break;
		}

		char key = static_cast<char>(wParam);
		
		// modify this later
		if (key == VK_ESCAPE) {
			// Not really a good way but just for testing purposes for now.
			/*RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			POINT ptLT = { clientRect.left, clientRect.top };
			POINT ptRB = { clientRect.right, clientRect.bottom };

			ClientToScreen(hwnd, &ptLT);
			ClientToScreen(hwnd, &ptRB);
			clientRect.left = ptLT.x;
			clientRect.top = ptLT.y;
			clientRect.right = ptRB.x;
			clientRect.bottom = ptRB.y;*/
			gameManager.GetGameState().IsCursorContainedInScreen = !gameManager.GetGameState().IsCursorContainedInScreen;
			/*if (gameManager.GetGameState().IsCursorContainedInScreen) {
				ShowCursor(FALSE);
				SetCapture(hwnd);
			}
			else {
				ShowCursor(TRUE);
				ReleaseCapture();
			}*/
			gameManager.GetGameState().IsCursorContainedInScreen ? ShowCursor(FALSE) : ShowCursor(TRUE);
			//gameManager.GetGameState().IsCursorContainedInScreen ? SetCapture(hwnd) : ReleaseCapture();
			//gameManager.GetGameState().IsCursorContainedInScreen ? ClipCursor(&clientRect) : ClipCursor(NULL);

	
			// could probably generate a one-off event to toggle stuff.
		}

		// here before the updates of keyboard, is when I want to do my call to GameManager
		// game state might need to contain the delta time.
		gameManager.HandleInput(InputEvent(InputEvent::EventType::KEYBOARD, key, 0, mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::DOWN), 0), gameManager.GetGameState(), eventBuffer, kbd);

		kbd.OnKeyDown(static_cast<uint8_t>(wParam));
		break;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		char key = static_cast<char>(wParam);
		gameManager.HandleInput(InputEvent(InputEvent::EventType::KEYBOARD, key, 0, mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::UP), 0), gameManager.GetGameState(), eventBuffer, kbd);

		kbd.OnKeyUp(static_cast<uint8_t>(wParam));
		break;
	}
	case WM_SYSCHAR:
	case WM_CHAR:
		kbd.AppendText(static_cast<wchar_t>(wParam));
		break;

	/****************************************************
	*******            Mouse    Messages            *****
	****************************************************/
	case WM_LBUTTONDOWN:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::LMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::DOWN), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateLMBDown(InputState(InputState::InputStatus::DOWN));
		break;
	case WM_LBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::LMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::UP), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateLMBDown(InputState(InputState::InputStatus::UP));
		break;
	case WM_RBUTTONDOWN:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::RMB, (char)InputEvent::EventType::RMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::DOWN), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateRMBDown(InputState(InputState::InputStatus::DOWN));
		break;
	case WM_RBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::RMB, (char)InputEvent::EventType::RMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::UP), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateRMBDown(InputState(InputState::InputStatus::UP));
		break;
	case WM_MBUTTONDOWN:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::MMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::DOWN), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateMMBDown(InputState(InputState::InputStatus::DOWN));
		break;
	case WM_MBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::MMB, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::UP), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.UpdateMMBDown(InputState(InputState::InputStatus::UP));
		break;

	// SWITCH TO RAW INPUT WHEN HANDLING OUR FPS CAMERA ROTATION. WM_MOUSEMOVE COULD BE GOOD FOR OTHER STUFF WHO KNOWS.
	case WM_MOUSEMOVE: {

		int xPos = 0;
		int yPos = 0;

		if (GetCapture() == hwnd) {
			POINT capturedPos;
			GetCursorPos(&capturedPos);

			ScreenToClient(hwnd, &capturedPos);

			xPos = capturedPos.x;
			yPos = capturedPos.y;
		}
		else {
			xPos = GET_X_LPARAM(lParam);
			yPos = GET_Y_LPARAM(lParam);
		}

		// WILL HANDLE DIPS LATER. Don't want to deal with the headache of refactoring right now. TODO:!!! Vec floats, and no stdPair
		// all of this needs to be float anyways....
		float diffX = (xPos - mouse.GetXPos()) / static_cast<float>(width); // may want client width and not regular width here.
		float diffY = (yPos - mouse.GetYPos()) / static_cast<float>(height);

		// UGLY AS WELL, should do it with my bindings or something to generate an event.
		if (gameManager.GetGameState().IsCursorContainedInScreen) {
			RECT clientRect;
			GetClientRect(hwnd, &clientRect);
			POINT ptLT = { clientRect.left, clientRect.top };
			POINT ptRB = { clientRect.right, clientRect.bottom };

			int wQuarter = (ptLT.x + ptRB.x) / 10;
			int hQuarter = (ptLT.y + ptRB.y) / 10;

			if (mouse.GetXPos() <= ptLT.x + wQuarter || mouse.GetXPos() >= ptRB.x - wQuarter || mouse.GetYPos() <= ptLT.y + hQuarter || mouse.GetYPos() >= ptRB.y - hQuarter) {
				int newXPos = (ptLT.x + ptRB.x) / 2;
				int newYPos = (ptLT.y + ptRB.y) / 2;
				ClientToScreen(hwnd, &ptLT);
				ClientToScreen(hwnd, &ptRB);

				int centerX = (ptLT.x + ptRB.x) / 2;
				int centerY = (ptLT.y + ptRB.y) / 2;

				SetCursorPos(centerX, centerY);
				xPos = newXPos;
				yPos = newYPos;
			}
			
			//xPos = width / 2;
			//yPos = height / 2;
		}

		// need to add difference between current and new as well in here
		gameManager.HandleInput(InputEvent(InputEvent::EventType::MOUSEMOVE, (char)InputEvent::EventType::MOUSEMOVE, gameManager.GetDeltaTime(), xPos, yPos, diffX, diffY, InputState(InputState::InputStatus::DOWN), 0), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.SetPos(xPos, yPos);
		break;
	}

	case WM_INPUT: {
		break;
	}

	case WM_MOUSEWHEEL: {
		// keep in mind what positive and negative means in this actions/acc context.
		int acc = mouse.GetMWheelAccumulator();
		InputEvent::EventType e = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? InputEvent::EventType::MWHEELUP : InputEvent::EventType::MWHEELDOWN;

		acc += GET_WHEEL_DELTA_WPARAM(wParam);
		int actions = acc / 120;
		acc %= 120;

		gameManager.HandleInput(InputEvent(e, (char)e, gameManager.GetDeltaTime(), mouse.GetPos(), Vec2<float>(), InputState(InputState::InputStatus::DOWN), actions), gameManager.GetGameState(), eventBuffer, kbd);
		mouse.SetAccumulator(acc);
		break;
	}

	/*
	case WM_MOUSELEAVE:
		break;
	case WM_MOUSEHOVER:
		break;
	case WM_LBUTTONDBLCLK:
		break; // Don't care fo
	case WM_RBUTTONDBLCLK:
		break;
	*/
	case WM_SIZE: {
		UINT width = LOWORD(lParam);
		UINT height = HIWORD(lParam);
		client_w = width;
		client_h = height;
		gfx.ResizeWindow(width, height);
		break;
	}

	case WM_CLOSE:
		DestroyWindow(hwnd);
		return 0;
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

void Window::SetClientSize(int width, int height) {
	client_w = width;
	client_h = height;
}

bool Window::IsEventBufferEmpty() const {
	return eventBuffer.size() == 0;
}

std::optional<std::unique_ptr<GameEvent>> Window::PopEventBuffer() {
	if (IsEventBufferEmpty()) {
		return std::nullopt;
	}

	std::unique_ptr<GameEvent> e = std::move(eventBuffer.front());
	eventBuffer.pop();
	
	return e;
}