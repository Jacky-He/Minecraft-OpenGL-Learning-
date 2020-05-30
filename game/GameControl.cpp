#include "GameControl.hpp"

GameControl* GameControl::instance = new GameControl();

GameControl::GameControl()
{
}

GameControl::~GameControl()
{
}

void GameControl::Start()
{
//    for (auto& each : *objs) each.Start();
}

void GameControl::Update()
{
//    for (auto& each : *objs) each.Update();
}

GameControl GameControl::sharedInstance()
{
    return *instance;
}



