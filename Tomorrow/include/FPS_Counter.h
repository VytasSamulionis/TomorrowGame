#include <Windows.h>

class FpsCounter {
public:
    FpsCounter () {
        QueryPerformanceFrequency (&m_Frequency);
    }
    void StartCounter () {
        QueryPerformanceCounter (&m_Start);
    }
    void EndCounter () {
        QueryPerformanceCounter (&m_End);
    }
    long double GetTimeDelta () {
        return (long double)((m_End.QuadPart - m_Start.QuadPart) / (long double)m_Frequency.QuadPart);
    }
    long double GetFramesPerSecond () {
        return (long double) (1 / (long double)((m_End.QuadPart - m_Start.QuadPart) / (long double)m_Frequency.QuadPart));
    }
private:
    LARGE_INTEGER m_Start;
    LARGE_INTEGER m_End;
    LARGE_INTEGER m_Frequency;
};