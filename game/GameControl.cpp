#include "GameControl.hpp"

GameControl::GameControl()
{
    objs.push_back(*this);
}

GameControl::~GameControl()
{
    objs.clear();
}

void GameControl::StaticStart()
{
    for (auto& each : objs) each.Start();
}

void GameControl::StaticUpdate()
{
    for (auto& each : objs) each.Update();
}

