#include <framework/EasyDefine.hpp>
#include <framework/FeatureEnvMgr.hpp>
#include <framework/WindowManager.hpp>
#include <framework/InputManager.hpp>
#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

//glfw error callback
static void glfwErrorCallback(int error, const char* description)
{
    fprintf(stderr, "GLFW-Error: %s\n", description);
}

//glew keydown(not include mousebutton)
static void glfwKeyCallback(GLFWwindow* pWindow, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        glfwSetWindowShouldClose(pWindow, GLFW_TRUE);
    }
    InputManager::FireKeyInputCallback(key, action);
}
//glew mousemove
static void glfwMousemoveCallback(GLFWwindow* window, double xpos, double ypos)
{
    InputManager::FireMousemoveCallback(xpos, ypos);
}

//window resize
static void glfwWindowResizeCallback(GLFWwindow* window, int nWidth, int nHeight)
{
    WindowManager::WindowResize(nWidth, nHeight);
}

//framebuffer resize
static void glfwResizeCallback(GLFWwindow* pWindow, int nWidth, int nHeight)
{
    WindowManager::FrameBufferResize(nWidth, nHeight);
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
    glfwSetCursorPosCallback(pWindow, glfwMousemoveCallback);
    //glfwSetMouseButtonCallback()
    glfwSetWindowSizeCallback(pWindow, glfwWindowResizeCallback);
    glfwSetFramebufferSizeCallback(pWindow, glfwResizeCallback);

    WindowManager::SetWindowState(pWindow);

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