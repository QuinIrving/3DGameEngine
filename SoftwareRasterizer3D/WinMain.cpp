#include <windows.h>

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class
	const wchar_t CLASS_NAME[] = L"Test Class";

	WNDCLASSEX wc{};
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style =
		CS_HREDRAW // Redraw window when width change in client area 
		| CS_VREDRAW // Redraw window when height change in client area
		| CS_OWNDC // Unique device context for each window in the class.
		| CS_DBLCLKS; // Allow double click to window proc when sent via mouse.
	wc.lpfnWndProc = WindowProc; // pointer to window proc, defining most behaviour of window.
	wc.cbClsExtra = 0; // Extra bytes allocate following window-class structure.
	wc.cbWndExtra = 0; // Extra bytes following window instance.
	wc.hInstance = hInstance; // handle to application instance
	wc.hIcon = NULL; // Handle to an icon resource.
	wc.hCursor = NULL; // handle to cursor resource
	wc.hbrBackground = NULL; // Must paint our own background.
	wc.lpszMenuName = NULL; // string specifying the resource name of the class menu as appeared in resource file.
	wc.lpszClassName = CLASS_NAME; // identifies window class
	wc.hIconSm = NULL; // Handle to small icon, if null grabs hIcon
	
	RegisterClassEx(&wc); // REgister the window with the operating system.

	const wchar_t WND_TITLE[] = L"3DGameEngine";

	// Create a new instance of the window.
	HWND hwnd = CreateWindowEx(
		0, CLASS_NAME, WND_TITLE,
		WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW,

		// Size & position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, NULL
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	//while (true);
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}
