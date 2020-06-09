#pragma once
#include "Util.hpp"
#include "Listener.hpp"

template <typename ... Args>
class Event
{
public:
    std::shared_ptr<std::map<void*, std::function<void(Args...)>>> listeners;
    Event()
    {
        listeners = std::make_shared<std::map<void*, std::function<void(Args...)>>>();
    }
    ~Event(){};
    
    void Trigger(Args... args)
    {
        for (auto& f : *listeners)
        {
            f.second(args...);
        }
    }
    
    Listener <Args...> AddEventListener (void* obj, std::function<void(Args...)> f)
    {
        listeners -> insert(std::pair <void*, std::function<void(Args...)>> (obj, f));
        return Listener <Args...> (obj, listeners);
    }
    
    void RemoveEventListener (Listener <Args...> listener)
    {
        listener.Remove();
    }
};
