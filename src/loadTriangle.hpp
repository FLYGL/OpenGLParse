#include <string>
#include <vector>
#include <array>
#include <GL/glew.h>

struct Point
{
    GLfloat x = 0;
    GLfloat y = 0;
    GLfloat z = 0;
};

struct Triangle
{
    std::array<Point, 3> vertices;
};

std::vector<Triangle> loadTriangleVertices(const std::string& vertexPath);