class Timer
{
public:
    Timer()
    {
        App::Initialize();

        preCount = GetCounter();
        frequency = GetCountFrequency();
    }
    ~Timer()
    {
    }
    float GetTime() const
    {
        return time;
    }
    float GetDeltaTime() const
    {
        return deltaTime;
    }
    int GetFrameRate() const
    {
        return frameRate;
    }
    void Update()
    {
        LARGE_INTEGER count = GetCounter();
        deltaTime = static_cast<float>(count.QuadPart - preCount.QuadPart) / frequency.QuadPart;
        preCount = GetCounter();

        time += deltaTime;

        frameCount++;
        second += deltaTime;
        if (second >= 1.0f)
        {
            frameRate = frameCount;
            frameCount = 0;
            second -= 1.0f;
        }
    }

private:
    float time = 0.0f;
    float deltaTime = 0.0f;
    int frameRate = 0;
    float second = 0.0f;
    int frameCount = 0;
    LARGE_INTEGER preCount;
    LARGE_INTEGER frequency;

    LARGE_INTEGER GetCounter() const
    {
        LARGE_INTEGER counter;
        QueryPerformanceCounter(&counter);
        return counter;
    }
    LARGE_INTEGER GetCountFrequency() const
    {
        LARGE_INTEGER frequency;
        QueryPerformanceFrequency(&frequency);
        return frequency;
    }
};