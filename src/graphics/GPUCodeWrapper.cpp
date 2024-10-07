#include "GPUCodeWrapper.hpp"
#include "framework/EasyDefine.hpp"
#include "framework/FileSystemWrapper.hpp"
#include <iostream>
//maybe threadlocal
static std::string gs_errLog;

struct ShaderContext
{
    GLuint  uShaderName;
    char*   pShaderCode;

    ShaderContext() : uShaderName{0}, pShaderCode{nullptr}
    {}

    void ReleaseSource();
};

void ShaderContext::ReleaseSource()
{
    if (uShaderName > 0)
    {
        glDeleteShader(uShaderName);
    }
    free(pShaderCode);
}

static ShaderContext CreateShaderContextByCodePath(GLenum eShaderType, const std::string& rShaderCodeAbPath)
{
    ShaderContext resultContext;
    bool bRetCode = false;
    GLuint uShaderName = 0;
    GLint nShaderStatus = 0;
    char* pShaderCode = nullptr;
    size_t uShaderCodeLength = 0;

    gs_errLog.resize(100);

    uShaderName = glCreateShader(eShaderType);
    bRetCode = LoadFileContent(rShaderCodeAbPath, reinterpret_cast<void**>(&pShaderCode), &uShaderCodeLength);
    JUMP_IF_FAIL(bRetCode);

    glShaderSource(uShaderName, 1, &pShaderCode, reinterpret_cast<GLint*>(&uShaderCodeLength));
    glCompileShader(uShaderName);
    glGetShaderiv(uShaderName, GL_COMPILE_STATUS, &nShaderStatus);
    if (!nShaderStatus)
    {
        GLsizei nValidLength = 0;
        glGetShaderInfoLog(uShaderName, static_cast<GLsizei>(gs_errLog.size()), &nValidLength, (GLchar*)gs_errLog.data());
        gs_errLog.resize(nValidLength);
        JUMP_IF_FAIL(false);
    }

    resultContext.uShaderName = uShaderName;
    resultContext.pShaderCode = pShaderCode;
    pShaderCode = nullptr;
FAIL_STATE:
    if (!bRetCode)
    {
        //ingore shader error, if uShaderName == 0
        glDeleteShader(uShaderName);
        free(pShaderCode);
        std::cout << gs_errLog << std::endl;
    }
    return resultContext;
}

GLuint ShaderWrapper::CreateShaderByCodePath(GLenum eShaderType, const std::string& rShaderCodeAbPath)
{
    ShaderContext shaderContext = CreateShaderContextByCodePath(eShaderType, rShaderCodeAbPath);
    free(shaderContext.pShaderCode);

    return shaderContext.uShaderName;
}

GLuint ProgramWrapper::CreateProgramByCodePath(const std::string& rVertexShaderCodeAbPath, const std::string& rFragShaderCodeAbPath)
{
    GLuint uResult = 0;
    bool bRetCode = false;
    GLuint program;
    GLint programStatus;

    GLuint easyVertexShader = ShaderWrapper::CreateShaderByCodePath(GL_VERTEX_SHADER, rVertexShaderCodeAbPath);
    GLuint easyFragShader = ShaderWrapper::CreateShaderByCodePath(GL_FRAGMENT_SHADER, rFragShaderCodeAbPath);
    JUMP_IF_FAIL(easyVertexShader > 0);
    JUMP_IF_FAIL(easyFragShader > 0);

    //gpu program
    program = glCreateProgram();
    
    glAttachShader(program, easyVertexShader);
    glAttachShader(program, easyFragShader);
    glLinkProgram(program);
    glGetProgramiv(program, GL_LINK_STATUS, &programStatus);
    if (!programStatus)
    {
        GLsizei nValidLength = 0;
        glGetProgramInfoLog(program, static_cast<GLsizei>(gs_errLog.size()), &nValidLength, (GLchar*)gs_errLog.data());
        gs_errLog.resize(nValidLength);
        JUMP_IF_FAIL(false);
    }

    uResult = program;
FAIL_STATE:
    if (uResult == 0)
    {
        std::cout << gs_errLog << std::endl;
    }
    return uResult;
}
