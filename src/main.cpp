#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"
#include "framework/InputManager.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

//glfw error callback
static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    }
    InputManager::FireInputCallback(key, action);
}

static void glfwResizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    glViewport(0, 0, nWidth, nHeight);
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
    pWindow = glfwCreateWindow(500, 500, "My Title", NULL, NULL);
    JUMP_IF_FAIL(pWindow);

    glfwSetKeyCallback(pWindow, glfwKeyCallback);
    glfwSetFramebufferSizeCallback(pWindow, glfwResizeCallback);

    glfwMakeContextCurrent(pWindow);
    //glew loadlib
    bRetCode = glewInit() == GLEW_OK;
    JUMP_IF_FAIL(bRetCode);

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    FeatureEnvWrapper::RunFeatures();
    while (!glfwWindowShouldClose(pWindow))
    {
        glClear(GL_COLOR_BUFFER_BIT);
        FeatureEnvWrapper::RunFrameFeatures();
        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    nResult = 0;
FAIL_STATE:
    glfwDestroyWindow(pWindow);
    glfwTerminate();

    return nResult;
}