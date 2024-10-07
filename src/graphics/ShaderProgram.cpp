#include <framework/EasyDefine.hpp>
#include "ShaderProgram.hpp"
#include "GPUCodeWrapper.hpp"

ShaderProgram::~ShaderProgram()
{
    Release();
}

bool ShaderProgram::Release()
{
    if(m_nProgram > 0)
        glDeleteProgram(m_nProgram);
    return true;
}

std::shared_ptr<ShaderProgram> ShaderProgram::MakeShaderProgram(const std::string& rVertexShaderCodeAbPath, const std::string& rFragShaderCodeAbPath)
{
    std::shared_ptr<ShaderProgram> pResultShaderProgram = nullptr;
    GLint nProgram = ProgramWrapper::CreateProgramByCodePath(rVertexShaderCodeAbPath, rFragShaderCodeAbPath);
    JUMP_IF_FAIL(nProgram);

    pResultShaderProgram = std::make_shared<ShaderProgram>(nProgram);
FAIL_STATE:
    return pResultShaderProgram;
}