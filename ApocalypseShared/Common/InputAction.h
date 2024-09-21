#pragma once
#include "pch.h"
struct InputAction
{
    int button;
    int identifier;
    int screen;
    //TEMPORARY UNTIL NETWORKINGMANAGER BECOMES A CLASS
    inline static std::vector<InputAction> inputActions;
};