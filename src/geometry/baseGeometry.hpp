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
    Triangle(Point a, Point b, Point c)
    {
        vertices[0] = a;
        vertices[1] = b;
        vertices[2] = c;
    }
};

std::vector<Triangle> loadTriangleVertices(const std::string& vertexPath);
std::vector<Triangle> MakeCubeTriangles(const Point& min, const Point& max);