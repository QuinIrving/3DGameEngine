#include <windows.h>
#include <new>

struct StateInfo {
	int test = 42;
};


LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

const wchar_t WND_TITLE[] = L"3DGameEngine";

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pCmdLine, int nCmdShow) {
	// Register the window class
	const wchar_t CLASS_NAME[] = L"Test Class";
	StateInfo* pState = new (std::nothrow) StateInfo;

	if (pState == NULL) {
		return 0;
	}

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

	// Create a new instance of the window.
	HWND hwnd = CreateWindowEx(
		0, CLASS_NAME, WND_TITLE,
		WS_CAPTION | WS_SYSMENU | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_OVERLAPPEDWINDOW,

		// Size & position
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL, NULL, hInstance, pState
	);

	if (hwnd == NULL) {
		return 0;
	}

	ShowWindow(hwnd, nCmdShow);

	MSG msg = { };
	bool isRunning = true;

	while (isRunning) {
		while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
			if (msg.message == WM_QUIT) {
				isRunning = false;
				break;
			}

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (!isRunning) {
			break;
		}

		// game logic Update()
		// draw frame Render()
	}
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	// additional data for message is contained w/in wParam & lParam look up each message
	// as each is unique
	StateInfo* pState;
	if (uMsg == WM_CREATE) {
		CREATESTRUCT* pCreate = reinterpret_cast<CREATESTRUCT*>(lParam);
		pState = reinterpret_cast<StateInfo*>(pCreate->lpCreateParams);
		SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) pState);

		/*
		Can now get StateInfo pointer from GetWIndowLongPtr
		LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
		StateInfo *pState = reinterpret_cast<StateInfo*>(ptr);

		could then create a small helper function:
		inline StateInfo* GetAppState(HWND hwnd)
		{
			LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
			StateInfo *pState = reinterpret_cast<StateInfo*>(ptr);
			return pState;
		}
		*/
	}
	else {
		pState = GetAppState(hwnd);
	}

	switch (uMsg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	default:
		break;

	// can make separate functions handle specific messages to reduce code in one function.

	// if some lengthy processing needs to occur inside window procedure, it will block
	// all other messages so ensure to use something like:
	/* 
		Creating a new thread
		Using a thread pool
		Using asynchronous I/O calls
		Using asynchronous procedure calls.
	*/
	}

	return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

inline StateInfo* GetAppState(HWND hwnd) {
	LONG_PTR ptr = GetWindowLongPtr(hwnd, GWLP_USERDATA);
	StateInfo* pState = reinterpret_cast<StateInfo*>(ptr);
	return pState;
}
