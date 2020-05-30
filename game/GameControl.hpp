#pragma once
#include "Util.hpp"

class GameControl
{
private:
    GameControl ();
    ~GameControl();
    static std::vector <GameControl> objs;
public:
    static void StaticStart();
    static void StaticUpdate();
    virtual void Start(){};
    virtual void Update(){};
};
