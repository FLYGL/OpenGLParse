#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

//glfw error callback
void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    }
}

int main(int argc, char* argv)
{
    int nResult = -1;
    bool bRetCode = false;
    glfwSetErrorCallback(glfwErrorCallback);

    GLFWwindow* pWindow = nullptr;

    JUMP_IF_FAIL(glfwInit());

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    pWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    JUMP_IF_FAIL(pWindow);

    glfwSetKeyCallback(pWindow, key_callback);
    glfwMakeContextCurrent(pWindow);
    //glew loadlib
    bRetCode = glewInit() == GLEW_OK;
    JUMP_IF_FAIL(bRetCode);

    FeatureEnvWrapper::RunFeatures();
    while (!glfwWindowShouldClose(pWindow))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        FeatureEnvWrapper::RunFrameFeatures();
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

SUCCESS_STATE:
    nResult = 0;

FAIL_STATE:
    glfwDestroyWindow(pWindow);
    glfwTerminate();

    return nResult;
}