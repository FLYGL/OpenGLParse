#include <framework/EasyDefine.hpp>
#include "Mesh.hpp"

#pragma warning(push)
bool MeshBase::InitOpenGLObject()
{
    bool bResult = false;

    glGenVertexArrays(1, &m_meshVAO);
    JUMP_IF_FAIL(m_meshVAO > 0);

    glGenBuffers(1, &m_meshVBO);
    JUMP_IF_FAIL(m_meshVBO > 0);

    bResult = true;
FAIL_STATE:
    if(!bResult)
    {
        UnInitOpenGLObject();
    }
    return bResult;
}

bool MeshBase::UnInitOpenGLObject()
{
    bool bResult = false;

    if(m_meshVBO > 0)
        glDeleteBuffers(1, &m_meshVBO);

    if(m_meshVAO > 0)
        glDeleteVertexArrays(1, &m_meshVAO);

    bResult = true;
//FAIL_STATE:
    return bResult;
}

bool MeshBase::MeshUploadGPUSync()
{
    bool bResult = false;
    void* pMeshCPUMem = GetMeshCPUMemStorage();
    std::array<OpenGL_VAO_AttributeInfo,4> attributeInfos = GetAttributeInfos();
    assert(m_meshVAO > 0);
    assert(m_meshVBO > 0);

    glBindVertexArray(m_meshVAO);
    glBindBuffer(GL_ARRAY_BUFFER, m_meshVBO);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(GetMeshCPUBufferSize()), pMeshCPUMem, GL_STATIC_DRAW);

    for(uint32_t i = 0; i < attributeInfos.size(); ++i)
    {
        OpenGL_VAO_AttributeInfo& rAttrInfo = attributeInfos[i];
        if(!rAttrInfo.bEnable)
        {
            glDisableVertexAttribArray(rAttrInfo.uAttrLocation);
            continue;
        }

        glEnableVertexAttribArray(rAttrInfo.uAttrLocation);
        glVertexAttribPointer(
            rAttrInfo.uAttrLocation,
            rAttrInfo.nAttrComponentSize,
            rAttrInfo.eAttrComponentType,
            rAttrInfo.bNormalized,
            rAttrInfo.uStride,
#pragma warning(disable : 4312)
            reinterpret_cast<const void*>(rAttrInfo.uFirstOffset)
        );
    }

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    m_bUploaded = true;
    bResult = true;
//FAIL_STATE:
    return bResult;
}

bool MeshBase::DrawVAO()
{
    glBindVertexArray(m_meshVAO);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(GetVertexCount()));
    glBindVertexArray(0);
    return true;
}
#pragma warning(pop)