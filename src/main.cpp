#include "EasyDefine.hpp"
#include <iostream>
#include <GL/glew.h>
#include <glfw/glfw3.h>

#include "LoadTriangle.hpp"
#include "FileSystemWrapper.hpp"
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

GLuint LoadProgram(const std::string& rVertexAbPath, const std::string& rFragmentAbPath)
{
    GLuint uResult = 0;
    bool bRetCode = false;
    char* pVertexShaderCode = nullptr;
    size_t uVertexCodeSize = 0;
    GLint nVertexCodeSize = 0;
    char* pFragmentShaderCode = nullptr;
    size_t uFragmentCodeSize = 0;
    GLint nFragmentCodeSize = 0;
    std::string errorLog;
    GLsizei uValidLength = 0;
    errorLog.resize(100);

    bRetCode = LoadFileContent(rVertexAbPath, reinterpret_cast<void**>(&pVertexShaderCode), &uVertexCodeSize);
    JUMP_IF_FAIL(bRetCode);
    bRetCode = LoadFileContent(rFragmentAbPath, reinterpret_cast<void**>(&pFragmentShaderCode), &uFragmentCodeSize);
    JUMP_IF_FAIL(bRetCode);

    nVertexCodeSize = static_cast<GLint>(uVertexCodeSize);
    nFragmentCodeSize = static_cast<GLint>(uFragmentCodeSize);

    GLuint easyVertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLuint easyFragShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(easyVertexShader, 1, &pVertexShaderCode, &nVertexCodeSize);
    glShaderSource(easyFragShader, 1, &pFragmentShaderCode, &nFragmentCodeSize);
    glCompileShader(easyVertexShader);
    glCompileShader(easyFragShader);

    GLint vertexShaderStatus, fragShaderStatus;
    glGetShaderiv(easyVertexShader, GL_COMPILE_STATUS, &vertexShaderStatus);
    if (!vertexShaderStatus)
    {
        glGetShaderInfoLog(easyVertexShader, errorLog.size(), &uValidLength, (GLchar*)errorLog.data());
        JUMP_IF_FAIL(false);
    }
    glGetShaderiv(easyFragShader, GL_COMPILE_STATUS, &fragShaderStatus);
    if (!fragShaderStatus)
    {
        glGetShaderInfoLog(easyFragShader, errorLog.size(), &uValidLength, (GLchar*)errorLog.data());
        JUMP_IF_FAIL(false);
    }

    //gpu program
    GLuint program = glCreateProgram();
    GLint programStatus;
    glAttachShader(program, easyVertexShader);
    glAttachShader(program, easyFragShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        glGetProgramInfoLog(program, errorLog.size(), &uValidLength, (GLchar*)errorLog.data());
        JUMP_IF_FAIL(false);
    }

SUCCESS_STATE:
    uResult = program;
FAIL_STATE:
    free(pVertexShaderCode);
    free(pFragmentShaderCode);
    if (uResult == 0 && uValidLength > 0)
    {
        errorLog.resize(uValidLength);
        printf("LoadProgramError : %s\n", errorLog.data());
    }
    return uResult;
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
    GLuint program = LoadProgram("resource/Shader/easy.vertex", "resource/Shader/easy.fragment");
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