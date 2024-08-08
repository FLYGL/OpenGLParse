#include <string>
#include <vector>
#include <array>

struct Point
{
    float x = 0;
    float y = 0;
    float z = 0;
};

struct Triangle
{
    std::array<Point, 3> vertices;
};

std::vector<Triangle> loadTriangleVertices(const std::string& vertexPath);