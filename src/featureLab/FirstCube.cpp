#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"

#include <iostream>
#include <vector>

#include "geometry/baseGeometry.hpp"
#include "GPUCodeWrapper.hpp"

struct CubeDrawContext
{
    //mesh data
    std::vector<Triangle> triangles;
    //vertex object name
    GLuint vertexObjName;
    //vbo
    GLuint vertexObjBuffer;

    //cube shader program
    GLuint program;

    bool inited = false;
};

CubeDrawContext gs_cubeDrawContext;

void InitCubeDrawContext()
{
    std::vector<Triangle> cubeTriangles = std::move(MakeCubeTriangles({-1, -1, -1},{1, 1, 1}));
    GLuint cubeVertexName;
    GLuint cubeBufferName;
    
    glGenVertexArrays(1, &cubeVertexName);
    JUMP_IF_FAIL(cubeVertexName > 0);

    glGenBuffers(1, &cubeBufferName);
    JUMP_IF_FAIL(cubeBufferName > 0);

    glBindVertexArray(cubeVertexName);
    glBindBuffer(GL_ARRAY_BUFFER, cubeBufferName);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Triangle) * cubeTriangles.size()), cubeTriangles.data()->vertices.data(), GL_STATIC_DRAW);

    gs_cubeDrawContext.triangles = std::move(cubeTriangles);
    gs_cubeDrawContext.vertexObjName = cubeVertexName;
    gs_cubeDrawContext.vertexObjBuffer = cubeBufferName;
    gs_cubeDrawContext.program = ProgramWrapper::CreateProgramByCodePath("resource/Shader/cube.vertex", "resource/Shader/easy.fragment");
    JUMP_IF_FAIL(gs_cubeDrawContext.program > 0);

    gs_cubeDrawContext.inited = true;
FAIL_STATE:
    DO_NOTHING();
}

void DrawCube()
{
    GLuint uVertexAttriLocation = 0;
    GLuint uVertexPointComp = 3;
    glUseProgram(gs_cubeDrawContext.program);
    glEnableVertexAttribArray(uVertexAttriLocation);
    glBindVertexArray(gs_cubeDrawContext.vertexObjName);
    glVertexAttribPointer(uVertexAttriLocation, uVertexPointComp, GL_FLOAT, GL_FALSE, 0, 0);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(gs_cubeDrawContext.triangles.size() * sizeof(Triangle::vertices)));
    glDisableVertexAttribArray(uVertexAttriLocation);
}

void FirstCube()
{
    if (!gs_cubeDrawContext.inited)
    {
        InitCubeDrawContext();
    }
    DrawCube();
    std::cout << "hello cube" << std::endl;
}

RegisterUniqueFrameFeaturetest(FirstCube);