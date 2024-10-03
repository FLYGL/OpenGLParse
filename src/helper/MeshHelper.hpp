#include <framework/EasyDefine.hpp>
#include <geometry/Mesh.hpp>
#include <vector>
#include <assert.h>

template<MeshStorageType MeshStorageTypeValue = MeshStorageType::MIX,
    typename PositionType = glm::vec3, typename ColorType = glm::vec3, typename NormalType = glm::vec3>
class MakeMeshHelper
{
    using MeshMemStorageValue = MeshMemStorage<MeshStorageTypeValue, PositionType, ColorType, NormalType>;
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

    Mesh<MeshMemStorageValue>* MakeMeshEnd()
    {
        bool bRetCode = false;
        Mesh<MeshMemStorageValue>* pResultMesh = nullptr;
        
        pResultMesh = new Mesh<MeshMemStorageValue>();
        JUMP_IF_FAIL(pResultMesh);

        bRetCode = pResultMesh->MakeMesh(m_positions, m_colors, m_normals);
        JUMP_IF_FAIL(bRetCode);
    FAIL_STATE:
        if(!bRetCode)
        {
            delete pResultMesh;
            pResultMesh = nullptr;
        }
        m_bMaking = false;
        return pResultMesh;
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