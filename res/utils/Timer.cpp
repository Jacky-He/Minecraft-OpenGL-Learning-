#include "Timer.hpp"

Timer::Timer()
{
    Reset();
}

Timer::~Timer()
{
    
}

void Timer::Report()
{
    cout << ((std::clock() - m_Start)/(double)CLOCKS_PER_SEC) << endl;
}

void Timer::Reset()
{
    m_Start = std::clock();
}
