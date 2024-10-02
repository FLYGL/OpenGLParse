#include <GL/glew.h>

#define OPENGL_VAO_POSITION_LOCATION    0
#define OPENGL_VAO_COLOR_LOCATION       1
#define OPENGL_VAO_NORMAL_LOCATION      2
#define OPENGL_VAO_CUSTOM_LOCATION      3

struct OpenGL_VAO_AttributeInfo
{
    bool    bEnable = false;
    GLuint  uAttrLocation;
    GLint   nAttrComponentSize;
    GLenum  eAttrComponentType;
    GLboolean bNormalized;
    GLsizei uStride;
    GLuint  uFirstOffset;
};