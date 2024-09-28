#include <GLFW/glfw3.h>
//what is mesh,

//usage: 1. upload to GPU 2. CPU Access MeshInfo
//data type: 1. position data per vertex 2. color data per vertex 3. normal data per vertex
//storage format : mix or separate

enum class MeshStorageType
{
    MIX,        //postion A, color A, normal A, position B, color B, normal B
    SEPARATE    //postion A, postion B, color A, color B, normal A, normal B
};

enum class MeshDataType : unsigned char
{
    POSITION = 0b1,
    COLOR = 0b10,
    NORMAL = 0b100
};

template <MeshStorageType, typename, typename, typename>
class MeshStorage;

template <MeshStorageType = MeshStorageType::MIX, typename PostionType = GLfloat, typename ColorType = GLfloat, typename NormalType = GLfloat>
class MeshStorage
{
public:
    MeshStorage() = default;
    ~MeshStorage() = default;
public:
    void SetMeshStorage();
public:
    //Base Access Function
    size_t GetVertexCount() { return m_uVertexCount; }
    PostionType SetPosition(size_t uIndex);
    PostionType GetPosition(size_t uIndex);
    ColorType SetColor(size_t uIndex);
    ColorType GetColor(size_t uIndex);
    NormalType SetNormal(size_t uIndex);
    NormalType GetNormal(size_t uIndex);
    
    MeshStorageType GetMeshStorageType() { return MeshStorageType::MIX; }
private:
    MeshDataType    m_eDataTypes;
    size_t          m_uVertexCount;
    unsigned char*  m_pMeshBuffer;
};