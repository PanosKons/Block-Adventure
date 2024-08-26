#pragma once
#include "pch.h"
struct InputAction
{
    int button;
    int identifier;
    //TEMPORARY UNTIL NETWORKINGMANAGER BECOMES A CLASS
    inline static std::vector<InputAction> inputActions;
};