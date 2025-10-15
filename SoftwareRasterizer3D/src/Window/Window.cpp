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

		// here before the updates of keyboard, is when I want to do my call to GameManager
		// game state might need to contain the delta time.
		gameManager.HandleInput(InputEvent(InputEvent::EventType::KEYBOARD, key, 0, mouse.GetPos(), true, 0), gameManager.GetGameState(), kbd);

		kbd.OnKeyDown(static_cast<uint8_t>(wParam));
		break;
	}
	case WM_SYSKEYUP:
	case WM_KEYUP:
	{
		char key = static_cast<char>(wParam);
		gameManager.HandleInput(InputEvent(InputEvent::EventType::KEYBOARD, key, 0, mouse.GetPos(), false, 0), gameManager.GetGameState(), kbd);

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
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::LMB, gameManager.GetDeltaTime(), mouse.GetPos(), true, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateLMBDown(InputStatus::DOWN);
		break;
	case WM_LBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::LMB, gameManager.GetDeltaTime(), mouse.GetPos(), false, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateLMBDown(InputStatus::UP);
		break;
	case WM_RBUTTONDOWN:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::RMB, (char)InputEvent::EventType::RMB, gameManager.GetDeltaTime(), mouse.GetPos(), true, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateRMBDown(InputStatus::DOWN);
		break;
	case WM_RBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::RMB, (char)InputEvent::EventType::RMB, gameManager.GetDeltaTime(), mouse.GetPos(), false, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateRMBDown(InputStatus::UP);
		break;
	case WM_MBUTTONDOWN:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::MMB, gameManager.GetDeltaTime(), mouse.GetPos(), true, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateMMBDown(InputStatus::DOWN);
		break;
	case WM_MBUTTONUP:
		gameManager.HandleInput(InputEvent(InputEvent::EventType::LMB, (char)InputEvent::EventType::MMB, gameManager.GetDeltaTime(), mouse.GetPos(), false, 0), gameManager.GetGameState(), kbd);
		mouse.UpdateMMBDown(InputStatus::UP);
		break;

	case WM_MOUSEMOVE: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);

		// need to add difference between current and new as well in here
		gameManager.HandleInput(InputEvent(InputEvent::EventType::MOUSEMOVE, (char)InputEvent::EventType::MOUSEMOVE, gameManager.GetDeltaTime(), xPos, yPos, false, 0), gameManager.GetGameState(), kbd);
		mouse.SetPos(xPos, yPos);
		break;
	}

	case WM_MOUSEWHEEL: {
		// keep in mind what positive and negative means in this actions/acc context.
		int acc = mouse.GetMWheelAccumulator();
		InputEvent::EventType e = (GET_WHEEL_DELTA_WPARAM(wParam) > 0) ? InputEvent::EventType::MWHEELUP : InputEvent::EventType::MWHEELDOWN;

		acc += GET_WHEEL_DELTA_WPARAM(wParam);
		int actions = acc / 120;
		acc %= 120;

		gameManager.HandleInput(InputEvent(e, (char)e, gameManager.GetDeltaTime(), mouse.GetPos(), false, actions), gameManager.GetGameState(), kbd);
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