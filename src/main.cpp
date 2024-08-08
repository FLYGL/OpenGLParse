#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "loadTriangle.hpp"
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
    //glew loadlib
    if(glewInit() != GLEW_OK)
    {
        glfwDestroyWindow(pWindow);
        glfwTerminate();
        return -1;
    }

    // prepare vertices data
    std::string dumpPath = "";
    std::vector<Triangle> triangles = loadTriangleVertices(dumpPath);
    GLsizei uBufferSize = 1;
    GLuint uBufferName = 0;
    GLuint uVerticeName = 0;
    glGenBuffers(uBufferSize, &uBufferName);
    glGenVertexArrays(uBufferSize, &uVerticeName);
    glBindVertexArray(uVerticeName);
    glBindBuffer(GL_ARRAY_BUFFER, uBufferName);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), triangles.data()->vertices.data(), GL_STATIC_DRAW);
    //TODO loadShader

    GLuint trianglesVerticesLocation = 0;
    GLint pointCompNum = 3;
    
    while (!glfwWindowShouldClose(pWindow))
    {
        // Keep running
        // int width,height;
        // glfwGetFramebufferSize(pWindow, &width, &height);
        // glViewport(0, 0, width, height);
        // double passTime = glfwGetTime();
        glEnableVertexAttribArray(trianglesVerticesLocation);
        //describe how to read vertex buffer object(vbo)
        glVertexAttribPointer(trianglesVerticesLocation, pointCompNum, GL_FLOAT, GL_FALSE, 0, 0);

        glDrawArrays(GL_TRIANGLES, 0, triangles.size());
        glDisableVertexAttribArray(trianglesVerticesLocation);

        glfwSwapBuffers(pWindow);
        glfwPollEvents();
    }

    glfwDestroyWindow(pWindow);
    glfwTerminate();
}