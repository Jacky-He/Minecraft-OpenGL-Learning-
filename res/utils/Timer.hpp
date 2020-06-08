#pragma once
#include "Util.hpp"

class Timer
{
private:
    std::clock_t m_Start;
public:
    Timer();
    ~Timer();
    void Report();
    
    void Reset();
};
