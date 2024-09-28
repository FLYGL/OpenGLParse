#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assert.h>
//what is mesh,
//TODO MACRO OPTIMIZE

//usage: 1. upload to GPU 2. CPU Access MeshInfo
//data type: 1. position data per vertex 2. color data per vertex 3. normal data per vertex
//storage format : mix or separate

enum class MeshStorageType
{
    MIX,        //postion A, color A, normal A, position B, color B, normal B
    SEPARATE    //postion A, postion B, color A, color B, normal A, normal B
};

//better union/manual implement
struct MeshDataType
{
    static constexpr int POSITION   = 0b1;
    static constexpr int COLOR      = 0b10;
    static constexpr int NORMAL     = 0b100;
    MeshDataType(int __) : __ {__} {}
    MeshDataType() = default;
    bool operator!=(const MeshDataType& rOther) { return __ != rOther.__; };
    bool operator &(int value) { return __ & value; }
private:
    int __;
};

template <MeshStorageType, typename = glm::vec3, typename = glm::vec3, typename = glm::vec3>
class MeshMemStorage;

template <typename PostionType, typename ColorType, typename NormalType>
class MeshMemStorage<MeshStorageType::MIX, PostionType, ColorType, NormalType>
{
public:
    MeshMemStorage() = default;
    ~MeshMemStorage() = default;
public:
    void SetMeshStorage(unsigned char* pMeshBuffer, size_t uVertexCount, MeshDataType eMeshDataType)
    {
        assert(pMeshBuffer);
        assert(uVertexCount > 0);
        m_pMeshBuffer = pMeshBuffer;
        m_eDataTypes = eMeshDataType;
        m_uVertexCount = uVertexCount;
        m_uBytesPerVertex = 0;
        m_nPositionTypeOffset = -1;
        m_nColorTypeOffset = -1;
        m_nNormalTypeOffset = -1;
        if (eMeshDataType & MeshDataType::POSITION)
        {
            m_nPositionTypeOffset = m_uBytesPerVertex;
            m_uBytesPerVertex += sizeof(PostionType);
        }
        if (eMeshDataType & MeshDataType::COLOR)
        {
            m_nColorTypeOffset = m_uBytesPerVertex;
            m_uBytesPerVertex += sizeof(ColorType);
        }
        if (eMeshDataType & MeshDataType::NORMAL)
        {
            m_nNormalTypeOffset = m_uBytesPerVertex;
            m_uBytesPerVertex += sizeof(NormalType);
        }
        assert(m_uBytesPerVertex > 0);
    }
public:
    //Base Access Function
    size_t GetVertexCount() { return m_uVertexCount; }
    void SetPosition(size_t uIndex, PostionType pos)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        *reinterpret_cast<PostionType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nPositionTypeOffset) = pos;
    }
    PostionType GetPosition(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        return *reinterpret_cast<PostionType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nPositionTypeOffset);
    }
    void SetColor(size_t uIndex, ColorType color)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nColorTypeOffset >= 0);
        *reinterpret_cast<ColorType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nColorTypeOffset) = color;
    }
    ColorType GetColor(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nColorTypeOffset >= 0);
        return *reinterpret_cast<ColorType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nColorTypeOffset);
    }
    void SetNormal(size_t uIndex, NormalType normal)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nNormalTypeOffset >= 0);
        *reinterpret_cast<NormalType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nNormalTypeOffset) = normal;
    }
    NormalType GetNormal(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nNormalTypeOffset >= 0);
        return *reinterpret_cast<NormalType*>(m_pMeshBuffer + m_uBytesPerVertex * uIndex + m_nNormalTypeOffset);
    }

    MeshStorageType GetMeshStorageType()    { return MeshStorageType::MIX; }
    MeshDataType    GetMeshDataType()       { return m_eDataTypes; }
private:
    unsigned char* m_pMeshBuffer;
    size_t          m_uVertexCount;
    MeshDataType    m_eDataTypes;
private:
    //helper memeber
    size_t          m_uBytesPerVertex;
    //offset in one vertex data
    int             m_nPositionTypeOffset;
    int             m_nColorTypeOffset;
    int             m_nNormalTypeOffset;
};

template <typename PostionType, typename ColorType, typename NormalType>
class MeshMemStorage<MeshStorageType::SEPARATE, PostionType, ColorType, NormalType>
{
public:
    MeshMemStorage() = default;
    ~MeshMemStorage() = default;
public:
    void SetMeshStorage(unsigned char* pMeshBuffer, size_t uVertexCount, MeshDataType eMeshDataType)
    {
        assert(pMeshBuffer);
        assert(uVertexCount > 0);
        size_t uBytes = 0;
        m_pMeshBuffer = pMeshBuffer;
        m_eDataTypes = eMeshDataType;
        m_uVertexCount = uVertexCount;
        m_nPositionTypeOffset = -1;
        m_nColorTypeOffset = -1;
        m_nNormalTypeOffset = -1;
        if (eMeshDataType & MeshDataType::POSITION)
        {
            m_nPositionTypeOffset = uBytes;
            uBytes += sizeof(PostionType) * uVertexCount;
        }
        if (eMeshDataType & MeshDataType::COLOR)
        {
            m_nColorTypeOffset = uBytes;
            uBytes += sizeof(ColorType) * uVertexCount;
        }
        if (eMeshDataType & MeshDataType::NORMAL)
        {
            m_nNormalTypeOffset = uBytes;
            uBytes += sizeof(NormalType) * uVertexCount;
        }
        assert(uBytes > 0);
    }
public:
    //Base Access Function
    size_t GetVertexCount() { return m_uVertexCount; }
    void SetPosition(size_t uIndex, PostionType pos)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        *reinterpret_cast<PostionType*>(m_pMeshBuffer + sizeof(PostionType) * uIndex + m_nPositionTypeOffset) = pos;
    }
    PostionType GetPosition(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        return *reinterpret_cast<PostionType*>(m_pMeshBuffer + sizeof(PostionType) * uIndex + m_nPositionTypeOffset);
    }
    void SetColor(size_t uIndex, ColorType color)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nColorTypeOffset >= 0);
        *reinterpret_cast<ColorType*>(m_pMeshBuffer + sizeof(ColorType) * uIndex + m_nColorTypeOffset) = color;
    }
    ColorType GetColor(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nColorTypeOffset >= 0);
        return *reinterpret_cast<ColorType*>(m_pMeshBuffer + sizeof(ColorType) * uIndex + m_nColorTypeOffset);
    }
    void SetNormal(size_t uIndex, NormalType normal)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nNormalTypeOffset >= 0);
        *reinterpret_cast<NormalType*>(m_pMeshBuffer + sizeof(NormalType) * uIndex + m_nNormalTypeOffset) = normal;
    }
    NormalType GetNormal(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nNormalTypeOffset >= 0);
        return *reinterpret_cast<NormalType*>(m_pMeshBuffer + sizeof(NormalType) * uIndex + m_nNormalTypeOffset);
    }

    MeshStorageType GetMeshStorageType()    { return MeshStorageType::SEPARATE; }
    MeshDataType    GetMeshDataType()       { return m_eDataTypes; }
private:
    unsigned char* m_pMeshBuffer;
    size_t          m_uVertexCount;
    MeshDataType    m_eDataTypes;
private:
    //start offset of continuous data
    int             m_nPositionTypeOffset;
    int             m_nColorTypeOffset;
    int             m_nNormalTypeOffset;
};