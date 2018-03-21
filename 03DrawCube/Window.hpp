class Window
{
public:
	Window()
	{
        App::Initialize();

        HINSTANCE instance = GetModuleHandleW(nullptr);

        HCURSOR cursor = static_cast<HCURSOR>(LoadImageW(nullptr, MAKEINTRESOURCEW(OCR_NORMAL), IMAGE_CURSOR, 0, 0, LR_SHARED));

        WNDCLASSW windowClass = {};
        windowClass.lpfnWndProc = ProceedMessage;
        windowClass.hInstance = instance;
        windowClass.hCursor = cursor;
        windowClass.lpszClassName = App::NAME;
        RegisterClassW(&windowClass);

        handle = CreateWindowW(App::NAME, App::NAME, WS_OVERLAPPEDWINDOW, 0, 0, 0, 0, nullptr, nullptr, instance, nullptr);

        SetSize(1280, 720);

        ShowWindow(handle, SW_SHOWNORMAL);
	}
	~Window()
	{
        UnregisterClassW(App::NAME, GetModuleHandleW(nullptr));
	}
	HWND GetHandle() const
	{
		return handle;
	}
	DirectX::XMINT2 GetSize() const
	{
		RECT clientRect = {};
		GetClientRect(handle, &clientRect);

		return DirectX::XMINT2(clientRect.right - clientRect.left, clientRect.bottom - clientRect.top);
	}
	void SetSize(int width, int height)
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
	bool Update()
	{
		MSG message = {};

		while (PeekMessageW(&message, nullptr, 0, 0, PM_REMOVE))
		{
			if (message.message == WM_QUIT)
                return false;

			TranslateMessage(&message);
			DispatchMessageW(&message);
		}

		return true;
	}

private:
    HWND handle;

    static LRESULT CALLBACK ProceedMessage(HWND window, UINT message, WPARAM wParam, LPARAM lParam)
    {
        if (message == WM_DESTROY)
            PostQuitMessage(0);

        return DefWindowProcW(window, message, wParam, lParam);
    }
};