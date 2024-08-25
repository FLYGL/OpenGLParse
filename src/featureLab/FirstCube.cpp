#include "framework/EasyDefine.hpp"
#include "framework/FeatureEnvMgr.hpp"

#include <iostream>
#include <vector>
#include <chrono>

#include "component/CameraComponent.hpp"
#include "component/InputComponent.hpp"
#include "geometry/baseGeometry.hpp"
#include "GPUCodeWrapper.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
struct CubeDrawContext;
//TODO control implement
//1. move speed
//2. w a s d keydown
//3. cursor
void ControlInputCallback(int nKey, int nAction, void* pContext)
{
    CubeDrawContext* pDrawContext = reinterpret_cast<CubeDrawContext*>(pContext);
    std::cout << nKey << " " << nAction << std::endl;
}

//TODO check difference between vao and vbo
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

    //glm
    glm::mat<4, 4, GLfloat> worldPositionMat4;
    glm::mat<4, 4, GLfloat> localRotateMat4;
    glm::mat<4, 4, GLfloat> viewMat4;
    glm::mat<4, 4, GLfloat> perspectiveMat4;
    //component
    CameraComponent cubeCamera;
    InputComponent inputCamera;
    bool inited = false;

    CubeDrawContext() : 
        inputCamera{ ControlInputCallback , this}
    {}
};

CubeDrawContext gs_cubeDrawContext;

void InitCubeDrawContext()
{
    std::vector<Triangle> cubeTriangles = std::move(MakeCubeTriangles({-1.0f, -1.0f, -1.0f},{1.0f, 1.0f, 1.0f}));
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
    gs_cubeDrawContext.program = ProgramWrapper::CreateProgramByCodePath("resource/Shader/easy.vertex", "resource/Shader/easy.fragment");
    JUMP_IF_FAIL(gs_cubeDrawContext.program > 0);

    //init worldPositionMat4
    gs_cubeDrawContext.worldPositionMat4 = glm::identity<glm::mat4>();
    //init viewMat4 and perspectiveMat4
    gs_cubeDrawContext.cubeCamera.SetPosition({ 0, 0, -10 });
    gs_cubeDrawContext.cubeCamera.SetXDegree(45.0f);
    gs_cubeDrawContext.cubeCamera.GenrateViewMatrix(gs_cubeDrawContext.viewMat4);
    gs_cubeDrawContext.cubeCamera.SetFrustum(45.0f, 1.0f, 0.5f, 20.0f);
    gs_cubeDrawContext.cubeCamera.GenratePerspectiveMatrix(gs_cubeDrawContext.perspectiveMat4);

    gs_cubeDrawContext.inited = true;
FAIL_STATE:
    DO_NOTHING();
}

void PrepareSetOpenGLContextState()
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void UpdateRotation()
{
    static float curYDegree = 0.0f;
    static float rotateYDegreePerSecond = 30.0f;
    static auto s_lastTime = std::chrono::high_resolution_clock::now();

    auto curTime = std::chrono::high_resolution_clock::now();
    auto delatTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - s_lastTime);
    curYDegree += static_cast<float>(delatTime.count()) * rotateYDegreePerSecond / 1000;
    s_lastTime = curTime;

    gs_cubeDrawContext.localRotateMat4 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(curYDegree), glm::vec3{ 0, 1, 0 });
    gs_cubeDrawContext.cubeCamera.GenrateViewMatrix(gs_cubeDrawContext.viewMat4);
}

void DoTransform()
{
    std::vector<Point> tranformedVertices;
    tranformedVertices.resize(gs_cubeDrawContext.triangles.size() * sizeof(Triangle::vertices));

    glm::mat4 transformMat = gs_cubeDrawContext.perspectiveMat4 * gs_cubeDrawContext.viewMat4 * gs_cubeDrawContext.worldPositionMat4 * gs_cubeDrawContext.localRotateMat4;
    size_t vertexIndex = 0;
    for (Triangle& triangle : gs_cubeDrawContext.triangles)
    {
        for (Point& point : triangle.vertices)
        {
            Point& tranformedPoint = tranformedVertices[vertexIndex++];
            glm::vec4 tranformedGlmPoint = transformMat * glm::vec4(point.x, point.y, point.z, 1);
            tranformedGlmPoint = glm::normalize(tranformedGlmPoint);
            tranformedPoint.x = tranformedGlmPoint.x;
            tranformedPoint.y = tranformedGlmPoint.y;
            tranformedPoint.z = tranformedGlmPoint.z;
        }
    }
    glBindVertexArray(gs_cubeDrawContext.vertexObjName);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Point) * tranformedVertices.size()), tranformedVertices.data(), GL_STATIC_DRAW);
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
    PrepareSetOpenGLContextState();
    UpdateRotation();
    DoTransform();
    DrawCube();
}

RegisterUniqueFrameFeaturetest(FirstCube);