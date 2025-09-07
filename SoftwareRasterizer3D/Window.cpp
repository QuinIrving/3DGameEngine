#include <windowsx.h>
#include "Window.h"
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
	int x, int y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu) {

	WNDCLASS wc = { 0 };
	wc.lpfnWndProc = Window::WindowProc;
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = ClassName();
	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC; // CS_DBLCLKS

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
		dwExStyle, ClassName(), lpWindowName, dwStyle, x, y,
		nWidth, nHeight, hWndParent, hMenu, GetModuleHandle(NULL), this
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
		// this means we are handling a "repeat" keydown message - ignore.
		if ((lParam & (1 << 30)) != 0) {
			break;
		}
		kbd.OnKeyDown(static_cast<uint8_t>(wParam));
		break;

	case WM_SYSKEYUP:
	case WM_KEYUP:
		kbd.OnKeyUp(static_cast<uint8_t>(wParam));
		break;

	case WM_SYSCHAR:
	case WM_CHAR:
		kbd.AppendText(static_cast<wchar_t>(wParam));
		break;

	/****************************************************
	*******            Mouse    Messages            *****
	****************************************************/
	case WM_LBUTTONDOWN:
		mouse.UpdateLMBDown(InputStatus::DOWN);
		break;
	case WM_LBUTTONUP:
		mouse.UpdateLMBDown(InputStatus::UP);
		break;
	case WM_RBUTTONDOWN:
		mouse.UpdateRMBDown(InputStatus::DOWN);
		break;
	case WM_RBUTTONUP:
		mouse.UpdateRMBDown(InputStatus::UP);
		break;
	case WM_MBUTTONDOWN:
		mouse.UpdateMMBDown(InputStatus::DOWN);
		break;
	case WM_MBUTTONUP:
		mouse.UpdateMMBDown(InputStatus::UP);
		break;

	case WM_MOUSEMOVE: {
		int xPos = GET_X_LPARAM(lParam);
		int yPos = GET_Y_LPARAM(lParam);
		mouse.SetPos(xPos, yPos);
		break;
	}

	case WM_MOUSEWHEEL: {
		// keep in mind what positive and negative means in this actions/acc context.
		int acc = mouse.GetMWheelAccumulator();
		acc += GET_WHEEL_DELTA_WPARAM(wParam);
		int actions = acc / 120;
		acc %= 120;
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


	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	
	return DefWindowProc(hwnd, uMsg, wParam, lParam);;
}