#include <GL/glew.h>
#include <string>
#include <memory>

class ShaderProgram
{
public:
    ShaderProgram() : m_nProgram{0} {}
    ShaderProgram(GLint nProgram) : m_nProgram{nProgram} {}
    ~ShaderProgram();

    bool UseProgram()
    {
        if(m_nProgram > 0)
        {
            glUseProgram(m_nProgram);
            return true;
        }
        return false;
    }

    GLint GetProgram() { return m_nProgram; }

    bool Release();
public:
    static std::shared_ptr<ShaderProgram> MakeShaderProgram(const std::string& rVertexShaderCodeAbPath, const std::string& rFragShaderCodeAbPath);
private:
    GLint m_nProgram;
};