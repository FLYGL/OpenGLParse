#include <framework/EasyDefine.hpp>
#include <framework/FeatureEnvMgr.hpp>
#include <framework/GlobalInstanceManager.hpp>
#include <framework/WindowManager.hpp>
#include <glfw/glfw3.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "component/FirstPersonComponent.hpp"
#include "geometry/baseGeometry.hpp"
#include "GPUCodeWrapper.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"
ANONYMOUS_SCOPE_BEGIN
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
    //component
    FirstPersonComponent firstPersonView;
    //timestamp
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::milliseconds deltaTime;

    bool inited = false;
};

CubeDrawContext& gs_cubeDrawContext = InstanceManager::GetInstanceManager().RegisterIntance< CubeDrawContext>();

void InitCubeDrawContext()
{
    std::vector<Triangle> cubeTriangles = std::move(MakeCubeTriangles({ -1.0f, -1.0f, -1.0f }, { 1.0f, 1.0f, 1.0f }));
    GLuint cubeVertexName;
    GLuint cubeBufferName;

    //gen vao
    glGenVertexArrays(1, &cubeVertexName);
    JUMP_IF_FAIL(cubeVertexName > 0);

    //gen vbo
    glGenBuffers(1, &cubeBufferName);
    JUMP_IF_FAIL(cubeBufferName > 0);

    //bind vao,after binding, vao will capture most vertexattributeInfos;
    glBindVertexArray(cubeVertexName);
    GLuint uVertexAttriLocation = 0;
    //enable this attribute for vao
    glEnableVertexAttribArray(uVertexAttriLocation);

    //bind buffer to define bufferformat
    glBindBuffer(GL_ARRAY_BUFFER, cubeBufferName);
    GLuint uVertexPointComp = 3;
    //define buffer format and attributeIndex, this also record buffer, buffer format,attributeindex in vao
    glVertexAttribPointer(uVertexAttriLocation, uVertexPointComp, GL_FLOAT, GL_FALSE, 0, 0);

    //transferVertexAttributeData
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Triangle) * cubeTriangles.size()), cubeTriangles.data()->vertices.data(), GL_STATIC_DRAW);

    //vbo deal done
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    //vao deal done
    glBindVertexArray(0);

    gs_cubeDrawContext.triangles = std::move(cubeTriangles);
    gs_cubeDrawContext.vertexObjName = cubeVertexName;
    gs_cubeDrawContext.vertexObjBuffer = cubeBufferName;
    gs_cubeDrawContext.program = ProgramWrapper::CreateProgramByCodePath("resource/Shader/easy.vertex", "resource/Shader/easy.fragment");
    JUMP_IF_FAIL(gs_cubeDrawContext.program > 0);

    //init worldPositionMat4
    gs_cubeDrawContext.worldPositionMat4 = glm::identity<glm::mat4>();
    //init firstPersonView
    gs_cubeDrawContext.firstPersonView.Init();

    gs_cubeDrawContext.lastTime = std::chrono::high_resolution_clock::now();
    gs_cubeDrawContext.deltaTime = std::chrono::milliseconds(0);

    gs_cubeDrawContext.inited = true;
FAIL_STATE:
    DO_NOTHING();
}

void PreProcess()
{
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_DEPTH_TEST);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void BaseContextUpdate()
{
    //TimeUpdate
    auto curTime = std::chrono::high_resolution_clock::now();
    gs_cubeDrawContext.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - gs_cubeDrawContext.lastTime);
    gs_cubeDrawContext.lastTime = curTime;
}

void UpdateCamera()
{
    gs_cubeDrawContext.firstPersonView.UpdateByDeltaTime(gs_cubeDrawContext.deltaTime.count());
}

void UpdateCubeRotation()
{
    static float curYDegree = 0.0f;
    static float rotateYDegreePerSecond = 30.0f;

    auto& delatTime = gs_cubeDrawContext.deltaTime;
    curYDegree += static_cast<float>(delatTime.count()) * rotateYDegreePerSecond / 1000;
    gs_cubeDrawContext.localRotateMat4 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(curYDegree), glm::vec3{ 0, 1, 0 });
}

void DoCubeTransform()
{
    std::vector<Point> tranformedVertices;
    tranformedVertices.resize(gs_cubeDrawContext.triangles.size() * sizeof(Triangle::vertices));

    glm::mat4 transformMat = gs_cubeDrawContext.firstPersonView.GetPerspectiveMartix() * gs_cubeDrawContext.firstPersonView.GetViewMatrix() * gs_cubeDrawContext.worldPositionMat4 * gs_cubeDrawContext.localRotateMat4;
    size_t vertexIndex = 0;
    for (Triangle& triangle : gs_cubeDrawContext.triangles)
    {
        for (Point& point : triangle.vertices)
        {
            Point& tranformedPoint = tranformedVertices[vertexIndex++];
            glm::vec4 tranformedGlmPoint = transformMat * glm::vec4(point.x, point.y, point.z, 1);
            // perspective division
            tranformedGlmPoint /= tranformedGlmPoint.w;
            tranformedPoint.x = tranformedGlmPoint.x;
            tranformedPoint.y = tranformedGlmPoint.y;
            tranformedPoint.z = tranformedGlmPoint.z;
        }
    }
    glBindBuffer(GL_ARRAY_BUFFER, gs_cubeDrawContext.vertexObjBuffer);
    glBufferData(GL_ARRAY_BUFFER, static_cast<GLsizeiptr>(sizeof(Point) * tranformedVertices.size()), tranformedVertices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void DrawCube()
{
    glUseProgram(gs_cubeDrawContext.program);
    glBindVertexArray(gs_cubeDrawContext.vertexObjName);
    glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(gs_cubeDrawContext.triangles.size() * sizeof(Triangle::vertices)));
    glBindVertexArray(0);
}

void PostProcess()
{
    
}

void RefactorCube()
{
    if (!gs_cubeDrawContext.inited)
    {
        InitCubeDrawContext();
    }
    PreProcess();
    BaseContextUpdate();
    UpdateCamera();
    UpdateCubeRotation();
    DoCubeTransform();
    DrawCube();
    PostProcess();
}

RegisterUniqueFrameFeaturetest(RefactorCube);

ANONYMOUS_SCOPE_END