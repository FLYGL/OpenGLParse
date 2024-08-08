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

    glfwSetErrorCallback(glfwErrorCallback);
    if(!glfwInit())
    {
        printf("Hello,world!");
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    GLFWwindow* pWindow = glfwCreateWindow(640, 480, "My Title", NULL, NULL);
    if(!pWindow)
    {
        return -1;
    }
    glfwSetKeyCallback(pWindow, key_callback);
    glfwMakeContextCurrent(pWindow);
    //TODO glew loadlib
    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
        return -1;
    }

    while (!glfwWindowShouldClose(pWindow))
    {
        // Keep running
        // int width,height;
        // glfwGetFramebufferSize(pWindow, &width, &height);
        // glViewport(0, 0, width, height);
        // double passTime = glfwGetTime();

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}