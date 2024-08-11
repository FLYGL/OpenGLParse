#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"
#include <iostream>

void FirstCube()
{
    std::cout << "hello cube" << std::endl;
}

RegisterFeaturetest(FirstCube);