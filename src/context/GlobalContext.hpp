#include <glm/glm.hpp>
#include <GL/glew.h>

#include <chrono>
#include <component/FirstPersonComponent.hpp>

struct BaseGlobalContext
{
    FirstPersonComponent firstPersonView;
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::milliseconds deltaTime;

    virtual bool Update() = 0;
};