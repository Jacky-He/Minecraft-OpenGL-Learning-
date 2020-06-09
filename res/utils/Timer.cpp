#include "Timer.hpp"

Timer::Timer()
{
    Reset();
}

Timer::~Timer()
{
    
}

double Timer::Report(bool print)
{
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> diff = end - m_Start;
    double res = diff.count();
    if (print) cout << res << endl;
    return res;
}

void Timer::Reset()
{
    m_Start = std::chrono::high_resolution_clock::now();
}
