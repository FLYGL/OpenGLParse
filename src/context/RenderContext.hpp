#include <glm/glm.hpp>
#include <GL/glew.h>
#include <graphics/ShaderProgram.hpp>

struct BaseRenderContext
{

};

struct MeshRenderContext : public BaseRenderContext
{
    glm::mat<4, 4, GLfloat> modelMat4;
    std::shared_ptr<ShaderProgram> pShaderProgram;
};
