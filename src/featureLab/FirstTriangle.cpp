#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"

#include <GL/glew.h>
#include <vector>
#include <assert.h>

#include "geometry/baseGeometry.hpp"
#include "graphics/GPUCodeWrapper.hpp"

struct FirstTriangleContext
{
    std::vector<Triangle> triangles;
    GLuint uProgram = 0;
    GLuint uVertexBufferName = 0;
    GLuint uVertexObjectName = 0;

    bool bInited = false;
};

FirstTriangleContext drawContext;

void InitDrawContext()
{
    GLsizei uVertexBufferSize = 1;
    // prepare vertices data
    drawContext.triangles = std::move(loadTriangleVertices(""));
    glGenBuffers(uVertexBufferSize, &drawContext.uVertexBufferName);
    glGenVertexArrays(uVertexBufferSize, &drawContext.uVertexObjectName);
    glBindVertexArray(drawContext.uVertexObjectName);
    glBindBuffer(GL_ARRAY_BUFFER, drawContext.uVertexBufferName);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle) * drawContext.triangles.size(), drawContext.triangles.data()->vertices.data(), GL_STATIC_DRAW);
    drawContext.uProgram = ProgramWrapper::CreateProgramByCodePath("resource/Shader/easy.vertex", "resource/Shader/easy.fragment");
    JUMP_IF_FAIL(drawContext.uProgram > 0);

    glUseProgram(drawContext.uProgram);
    drawContext.bInited = true;
FAIL_STATE:
    //TODO ClearResource
    DO_NOTHING();
    assert(drawContext.bInited);
}

void DrawFirstTriangle()
{
    GLuint uVertexAttriLocation = 0;
    GLuint uVertexPointComp = 3;
    //set curRender some vertexinfo
    glEnableVertexAttribArray(uVertexAttriLocation);
    //besure vbo, TODO check glBindVertexArray is ok when multi drawcall
    glBindVertexArray(drawContext.uVertexObjectName);
    //describe how to read vertex buffer object(vbo)
    glVertexAttribPointer(uVertexAttriLocation, uVertexPointComp, GL_FLOAT, GL_FALSE, 0, 0);

    //third params meaning : vertex count
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(drawContext.triangles.size() * sizeof(Triangle::vertices)));
    glDisableVertexAttribArray(uVertexAttriLocation);
}

void FirstTriangle()
{
    if (!drawContext.bInited)
    {
        InitDrawContext();
    }
    DrawFirstTriangle();
}

RegisterFrameFeaturetest(FirstTriangle);