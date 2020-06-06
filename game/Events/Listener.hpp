#pragma once
#include "Util.hpp"

template <typename ... Args>
class Listener
{
private:
    void* handlerObj;
    std::shared_ptr<std::map<void*, std::function<void(Args...)>>> listeners;
public:
    Listener()
    {
        handlerObj = nullptr;
        listeners = nullptr;
    }
    
    Listener(void* obj, std::shared_ptr<std::map<void*, std::function<void(Args...)>>> listenerList)
    {
        handlerObj = obj;
        listeners = listenerList;
    }
    
    void Remove ()
    {
        listeners -> erase(handlerObj);
    }
};
