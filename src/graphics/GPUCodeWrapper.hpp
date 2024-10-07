#include <GL/glew.h>
#include <string>

class ShaderWrapper
{
public:
    static GLuint CreateShaderByCodePath(GLenum eShaderType, const std::string& rShaderCodeAbPath);
};

class ProgramWrapper
{
public:
    static GLuint CreateProgramByCodePath(const std::string& rVertexShaderCodeAbPath, const std::string& rFragShaderCodeAbPath);
};