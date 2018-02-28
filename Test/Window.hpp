class Window
{
	PROTECTED HWND handle;

	PUBLIC Window()
	{
		Initialize();
	}
	PUBLIC ~Window()
	{
	}
	PROTECTED void Initialize()
	{
		HINSTANCE instance = GetModuleHandleW(nullptr);

		WNDCLASSW windowClass = {};
		windowClass.style = 0;
		windowClass.lpfnWndProc = Proceed;
		windowClass.cbClsExtra = 0;
		windowClass.cbWndExtra = 0;
		windowClass.hInstance = instance;
		windowClass.hIcon = nullptr;
		windowClass.hCursor = static_cast<HCURSOR>(LoadImageW(nullptr, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_DEFAULTSIZE | LR_SHARED));
		windowClass.hbrBackground = nullptr;
		windowClass.lpszMenuName = nullptr;
		windowClass.lpszClassName = App::NAME;
		RegisterClassW(&windowClass);

		handle = CreateWindowW(App::NAME, App::NAME, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, nullptr, nullptr, instance, nullptr);

		SetSize(1280, 720);

		ShowWindow(handle, SW_SHOWNORMAL);
	}
	PUBLIC HWND GetHandle() const
	{
		return handle;
	}
	PUBLIC DirectX::XMINT2 GetSize() const
	{
		RECT clientRect = {};
		GetClientRect(handle, &clientRect);

		return DirectX::XMINT2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}
	PUBLIC void SetSize(int width, int height)
	{
		RECT windowRect = {};
		RECT clientRect = {};
		GetWindowRect(handle, &windowRect);
		GetClientRect(handle, &clientRect);

		int w = (windowRect.right - windowRect.left) - (clientRect.right - clientRect.left) + width;
		int h = (windowRect.bottom - windowRect.top) - (clientRect.bottom - clientRect.top) + height;
		int x = (GetSystemMetrics(SM_CXSCREEN) - w) / 2;
		int y = (GetSystemMetrics(SM_CYSCREEN) - h) / 2;

		SetWindowPos(handle, nullptr, x, y, w, h, SWP_FRAMECHANGED);
	}
	PROTECTED static LRESULT CALLBACK Proceed(HWND handle, UINT message, WPARAM wParam, LPARAM lParam)
	{
		switch (message)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		}
		return DefWindowProcW(handle, message, wParam, lParam);
	}
};
