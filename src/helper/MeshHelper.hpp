#include <framework/EasyDefine.hpp>
#include <geometry/Mesh.hpp>
#include <vector>
#include <assert.h>

template<MeshStorageType MeshStorageTypeValue,typename PositionType, typename ColorType, typename NormalType>
class MakeMeshHelper
{
public:
    MakeMeshHelper() = default;
    ~MakeMeshHelper() = default;
public:
    void MakeMeshBegin()
    {
        assert(!m_bMaking);
        m_positions.clear();
        m_normals.clear();
        m_colors.clear();
        m_bMaking = true;
    }

    Mesh<MeshStorageTypeValue, PositionType, ColorType, NormalType> MakeMeshEnd()
    {
        Mesh<MeshStorageTypeValue, PositionType, ColorType, NormalType> resultMesh;
        resultMesh.MakeMesh(m_positions, m_colors, m_normals);
        m_bMaking = false;
        return resultMesh;
    }
public:
    void PushPosition(PositionType position)
    {
        m_positions.emplace_back(position);
    }
    void PushNormal(ColorType normal)
    {
        m_normals.emplace_back(normal);
    }
    void PushColor(NormalType color)
    {
        m_colors.emplace_back(color);
    }
private:
    std::vector<PositionType> m_positions;
    std::vector<ColorType> m_colors;
    std::vector<NormalType> m_normals;

    bool m_bMaking = false;
};