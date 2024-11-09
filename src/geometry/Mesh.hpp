#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assert.h>

#include <array>
#include <algorithm>

#include "context/RenderContext.hpp"
#include "MeshMemStorage.hpp"
//what is mesh
//1. mesh cpu storage
//2. mesh gpu upload
//3. save all cpu and gpu states

//MeshBase define base behavior
class MeshBase
{
public:
    MeshBase() = default;
    ~MeshBase() = default;
public:
    bool InitOpenGLObject();
    bool UnInitOpenGLObject();
    virtual bool MeshUploadGPUSync();
    virtual bool DrawVAO();
    virtual size_t GetVertexCount() = 0;
    virtual void ReleaseCPUStorage() = 0;
    virtual void* GetMeshCPUMemStorage() = 0;
    virtual size_t GetMeshCPUBufferSize() = 0;
    //TODO stack array
    virtual std::array<OpenGL_VAO_AttributeInfo,4> GetAttributeInfos() = 0;
protected:
    GLuint m_meshVAO = 0;
    GLuint m_meshVBO = 0;
    bool m_bUploaded = false;
};

template<typename MeshMemStorageType>
class Mesh : public MeshBase
{
    using PositionType = typename MeshMemStorageType::PositionType;
    using ColorType = typename MeshMemStorageType::ColorType;
    using NormalType = typename MeshMemStorageType::NormalType;
public:
    Mesh() = default;
    ~Mesh() = default;
public:
    bool MakeMesh(std::vector<PositionType>& rPositions, std::vector<ColorType>& rColors, std::vector<NormalType>& rNormals)
    {
        bool bResult = false;
        //TODO optimize memory
        unsigned char* pMeshBuffer = nullptr;
        unsigned char* pOffset = nullptr;
        MeshDataType eMeshDataType = 0;
        const size_t uBufferSize = sizeof(PositionType) * rPositions.size() + sizeof(ColorType) * rColors.size() + sizeof(NormalType) * rNormals.size();
        pMeshBuffer = new unsigned char[uBufferSize];
        JUMP_IF_FAIL(pMeshBuffer);

        pOffset = pMeshBuffer;
        memcpy(pOffset, rPositions.data(), sizeof(PositionType) * rPositions.size());
        assert(rPositions.size() > 0);
        eMeshDataType |= MeshDataType::POSITION;
        
        if(rColors.size() > 0)
        {
            pOffset += sizeof(PositionType) * rPositions.size();
            memcpy(pOffset, rColors.data(), sizeof(ColorType) * rColors.size());
            assert(rPositions.size() == rColors.size());
            eMeshDataType |= MeshDataType::COLOR;
        }

        if(rNormals.size() > 0)
        {
            pOffset += sizeof(ColorType) * rColors.size();
            memcpy(pOffset, rNormals.data(), sizeof(NormalType) * rNormals.size());
            assert(rPositions.size() == rNormals.size());
            eMeshDataType |= MeshDataType::NORMAL;
        }
        m_bUploaded = false;
        m_meshMemStorage.ReleaseStorage();
        m_meshMemStorage.SetMeshStorage(pMeshBuffer, uBufferSize, rPositions.size(), eMeshDataType);
        bResult = true;
    FAIL_STATE:
        return bResult;
    }
public:
    bool MeshUploadGPUSync() override
    {
        bool bResult = false;
        JUMP_IF_SUCCESS(m_bUploaded);
        JUMP_IF_FAIL(InitOpenGLObject());
        JUMP_IF_FAIL(MeshBase::MeshUploadGPUSync());
    SUCCESS_STATE:
        bResult = true;
    FAIL_STATE:
        return bResult;
    }

    size_t GetVertexCount() override
    {
        return m_meshMemStorage.GetVertexCount();
    }
    
    void ReleaseCPUStorage() override
    {
        return m_meshMemStorage.ReleaseStorage();
    }

    void* GetMeshCPUMemStorage() override
    {
        return m_meshMemStorage.GetMeshBuffer();
    }

    size_t GetMeshCPUBufferSize() override
    {
        return m_meshMemStorage.GetMeshBufferSize();
    }

    std::array<OpenGL_VAO_AttributeInfo,4> GetAttributeInfos() override
    {
        std::array<OpenGL_VAO_AttributeInfo,4> result;
        auto attrInfos = m_meshMemStorage.GetAttributeInfos();
        auto minLength = std::min(result.size(), attrInfos.size());
        auto maxLength = std::max(result.size(), attrInfos.size());
        for(auto i = 0; i < minLength; ++i)
        {
            result[i] = attrInfos[i];
        }
        for(auto i = minLength; i < maxLength; ++i)
        {
            result[i].uAttrLocation = static_cast<GLuint>(i);
            result[i].bEnable = false;
        }
        return result; 
    }
public:
    //RenderContext
    void SetMeshModelMatrix(const glm::mat<4, 4, GLfloat>& rMat4)
    {
        m_meshRenderContext.modelMat4 = rMat4;
    }

    auto& GetModelMatrix()
    {
        return m_meshRenderContext.modelMat4;
    }

    void SetMeshShaderProgram(std::shared_ptr<ShaderProgram> pShaderProgram)
    {
        m_meshRenderContext.pShaderProgram = pShaderProgram;
    }

    bool UseProgram()
    {
        return m_meshRenderContext.pShaderProgram->UseProgram();
    }

    //TODO : erase this function
    //auto deduce type
    auto GetProgram()
    {
        return m_meshRenderContext.pShaderProgram->GetProgram();
    }

private:
   MeshMemStorageType   m_meshMemStorage;
   MeshRenderContext    m_meshRenderContext;
};

typedef Mesh<MeshMemStorage<MeshStorageType::MIX>> BaseMesh;