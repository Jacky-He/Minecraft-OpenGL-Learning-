#pragma once
#include "Util.hpp"


class Controller
{
public:
    virtual void Start() = 0;
    virtual void Update() = 0;
};


class GameControl
{
private:
    static GameControl* instance;
public:
    GameControl ();
    ~GameControl();
    static GameControl sharedInstance();
    void Start();
    void Update();
};
