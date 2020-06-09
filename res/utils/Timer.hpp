#pragma once
#include "Util.hpp"

class Timer
{
private:
    std::chrono::time_point<std::chrono::high_resolution_clock> m_Start;
public:
    Timer();
    ~Timer();
    double Report(bool print);
    
    void Reset();
};
