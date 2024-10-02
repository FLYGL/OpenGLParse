#include <GL/glew.h>
#include <glm/glm.hpp>
#include <graphics/OpenGL.hpp>
#include <array>
#include <assert.h>
//what is mesh,
//TODO MACRO OPTIMIZE

//usage: CPU Access MeshInfo
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
    MeshDataType& operator|=(int meshDataType)
    {
        __ |= meshDataType;
        return *this;
    }
private:
    int __;
};

template<typename GlmType>
GLenum constexpr GetGLenumFromGlmType()
{
    //glm::vec
    if constexpr(std::is_same_v<typename GlmType::value_type, GLfloat>)
    {
        return GL_FLOAT;
    }
}

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
    void*           GetMeshBuffer()         { return reinterpret_cast<void*>(m_pMeshBuffer); }


    std::array<OpenGL_VAO_AttributeInfo,3> GetAttributeInfos()
    {
        std::array<OpenGL_VAO_AttributeInfo,3> attributeInfos;
        if(m_nPositionTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_POSITION_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_POSITION_LOCATION;
            rAttrInfo.nAttrComponentSize = PostionType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<PostionType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = m_uBytesPerVertex;
            rAttrInfo.uFirstOffset = m_nPositionTypeOffset;
        }
        if(m_nColorTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_COLOR_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_COLOR_LOCATION;
            rAttrInfo.nAttrComponentSize = ColorType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<ColorType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = m_uBytesPerVertex;
            rAttrInfo.uFirstOffset = m_nColorTypeOffset;
        }
        if(m_nNormalTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_NORMAL_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_NORMAL_LOCATION;
            rAttrInfo.nAttrComponentSize = NormalType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<NormalType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = m_uBytesPerVertex;
            rAttrInfo.uFirstOffset = m_nNormalTypeOffset;
        }

        return attributeInfos;
    }
private:
    unsigned char*  m_pMeshBuffer;
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

template <typename _PostionType, typename _ColorType, typename _NormalType>
class MeshMemStorage<MeshStorageType::SEPARATE, _PostionType, _ColorType, _NormalType>
{
    using PositionType = _PostionType;
    using ColorType = _ColorType;
    using NormalType = _NormalType;
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
            uBytes += sizeof(PositionType) * uVertexCount;
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
    void SetPosition(size_t uIndex, PositionType pos)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        *reinterpret_cast<PositionType*>(m_pMeshBuffer + sizeof(PositionType) * uIndex + m_nPositionTypeOffset) = pos;
    }
    PositionType GetPosition(size_t uIndex)
    {
        assert(uIndex < m_uVertexCount);
        assert(m_nPositionTypeOffset >= 0);
        return *reinterpret_cast<PositionType*>(m_pMeshBuffer + sizeof(PositionType) * uIndex + m_nPositionTypeOffset);
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
    void*           GetMeshBuffer()         { return reinterpret_cast<void*>(m_pMeshBuffer); }
    std::array<OpenGL_VAO_AttributeInfo,3> GetAttributeInfos()
    {
        std::array<OpenGL_VAO_AttributeInfo,3> attributeInfos;
        if(m_nPositionTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_POSITION_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_POSITION_LOCATION;
            rAttrInfo.nAttrComponentSize = PositionType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<PositionType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = 0;
            rAttrInfo.uFirstOffset = m_nPositionTypeOffset;
        }
        if(m_nColorTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_COLOR_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_COLOR_LOCATION;
            rAttrInfo.nAttrComponentSize = ColorType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<ColorType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = 0;
            rAttrInfo.uFirstOffset = m_nColorTypeOffset;
        }
        if(m_nNormalTypeOffset >= 0)
        {
            OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[OPENGL_VAO_NORMAL_LOCATION];
            rAttrInfo.bEnable = true;
            rAttrInfo.uAttrLocation = OPENGL_VAO_NORMAL_LOCATION;
            rAttrInfo.nAttrComponentSize = NormalType::length_type();
            rAttrInfo.eAttrComponentType = GetGLenumFromGlmType<NormalType>();
            rAttrInfo.bNormalized = GL_FALSE;
            rAttrInfo.uStride = 0;
            rAttrInfo.uFirstOffset = m_nNormalTypeOffset;
        }

        return attributeInfos;
    }
private:
    unsigned char*  m_pMeshBuffer;
    size_t          m_uVertexCount;
    MeshDataType    m_eDataTypes;
private:
    //start offset of continuous data
    int             m_nPositionTypeOffset;
    int             m_nColorTypeOffset;
    int             m_nNormalTypeOffset;
};