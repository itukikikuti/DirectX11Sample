class Input
{
public:
    Input()
    {
        App::Initialize();

        Update();
    }
    ~Input()
    {
    }
    bool GetKey(int keyCode) const
    {
        return keyState[keyCode] & 0x80;
    }
    bool GetKeyUp(int keyCode) const
    {
        return !(keyState[keyCode] & 0x80) && (preKeyState[keyCode] & 0x80);
    }
    bool GetKeyDown(int keyCode) const
    {
        return (keyState[keyCode] & 0x80) && !(preKeyState[keyCode] & 0x80);
    }
    Float2 GetMousePosition() const
    {
        return mousePosition;
    }
    void SetMousePosition(float x, float y)
    {
        if (GetActiveWindow() != App::GetWindowHandle())
            return;

        mousePosition.x = x;
        mousePosition.y = y;
        POINT point = { static_cast<int>(x), static_cast<int>(y) };
        ClientToScreen(App::GetWindowHandle(), &point);
        SetCursorPos(point.x, point.y);
    }
    void SetShowCursor(bool isShowCursor)
    {
        if (this->isShowCursor == isShowCursor)
            return;

        this->isShowCursor = isShowCursor;
        ShowCursor(isShowCursor);
    }
    void Update()
    {
        POINT point = {};
        GetCursorPos(&point);

        ScreenToClient(App::GetWindowHandle(), &point);
        mousePosition = Float2(static_cast<float>(point.x), static_cast<float>(point.y));

        for (int i = 0; i < 256; i++)
        {
            preKeyState[i] = keyState[i];
        }

        GetKeyboardState(keyState);
    }

private:
    Float2 mousePosition;
    BYTE preKeyState[256];
    BYTE keyState[256];
    bool isShowCursor = true;
};