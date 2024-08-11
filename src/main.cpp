#include "framework/EasyDefine.hpp"
#include "framework/FileSystemWrapper.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "LoadTriangle.hpp"
#include "GPUCodeWrapper.hpp"
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

    std::vector<Triangle> triangles;
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

    // prepare vertices data
    triangles = std::move(loadTriangleVertices(""));
    GLsizei uBufferSize = 1;
    GLuint uBufferName = 0;
    GLuint uVerticeName = 0;
    glGenBuffers(uBufferSize, &uBufferName);
    glGenVertexArrays(uBufferSize, &uVerticeName);
    glBindVertexArray(uVerticeName);
    glBindBuffer(GL_ARRAY_BUFFER, uBufferName);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * triangles.size(), triangles.data()->vertices.data(), GL_STATIC_DRAW);
    //loadShader
    GLuint program = ProgramWrapper::CreateProgramByCodePath("resource/Shader/easy.vertex", "resource/Shader/easy.fragment");
    JUMP_IF_FAIL(program > 0);

    glUseProgram(program);
    GLuint trianglesVerticesLocation = 0;
    GLint pointCompNum = 3;
    
    while (!glfwWindowShouldClose(pWindow))
    {
        glClearColor(0.0, 0.0, 0.0, 0.0);
        //set curRender some vertexinfo
        glEnableVertexAttribArray(trianglesVerticesLocation);
        //describe how to read vertex buffer object(vbo)
        glVertexAttribPointer(trianglesVerticesLocation, pointCompNum, GL_FLOAT, GL_FALSE, 0, 0);

        //third params meaning : vertex count
        glDrawArrays(GL_TRIANGLES, 0, triangles.size() * sizeof(Triangle::vertices));
        glDisableVertexAttribArray(trianglesVerticesLocation);

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