#include <GL/glew.h>
#include <string>

struct ShaderContext
{
    GLuint  uShaderName;
    char*   pShaderCode;

    ShaderContext() : uShaderName{0}, pShaderCode{nullptr}
    {}

    void ReleaseSource();
};

class ShaderWrapper
{
public:
    static GLuint CreateShaderByCodePath(GLenum eShaderType, const std::string& rShaderCodeAbPath);
private:
    static ShaderContext CreateShaderContextByCodePath(GLenum eShaderType, const std::string& rShaderCodeAbPath);
};

class ProgramWrapper
{
public:
    static GLuint CreateProgramByCodePath(const std::string& rVertexShaderCodeAbPath, const std::string& rFragShaderCodeAbPath);
};