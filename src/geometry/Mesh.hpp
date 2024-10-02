#include <GL/glew.h>
#include <glm/glm.hpp>
#include <assert.h>

#include <array>

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
    virtual void* GetMeshCPUMemStorage() = 0;
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
    using PositionType = typename MeshMemStorageType::PostionType;
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
        if(rPositions.size() > 0)
            eMeshDataType |= MeshDataType::POSITION;
        
        pOffset += sizeof(PositionType) * rPositions.size();
        memcpy(pOffset, rColors.data(), sizeof(ColorType) * rColors.size());
        if(rColors.size() > 0)
            eMeshDataType |= MeshDataType::COLOR;

        pOffset += sizeof(ColorType) * rColors.size();
        memcpy(pOffset, rNormals.data(), sizeof(NormalType) * rNormals.size());
        if(rNormals.size() > 0)
            eMeshDataType |= MeshDataType::NORMAL;

        m_meshMemStorage.SetMeshStorage(pMeshBuffer, rPositions.size(), eMeshDataType);
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
    void* GetMeshCPUMemStorage() override
    {
        return m_meshMemStorage.GetMeshBuffer();
    }
    std::array<OpenGL_VAO_AttributeInfo,4> GetAttributeInfos() override
    {
        std::array<OpenGL_VAO_AttributeInfo,4> result;
        auto attrInfos = m_meshMemStorage.GetAttributeInfos();
        for(uint32_t i = 0; i < result.size() && i < attrInfos.size(); ++i)
        {
            result[i] = attrInfos[i];
        }
        return result; 
    }
private:
   MeshMemStorageType m_meshMemStorage;
};