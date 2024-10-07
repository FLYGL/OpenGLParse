#include <framework/EasyDefine.hpp>
#include <framework/FeatureEnvMgr.hpp>
#include <framework/GlobalInstanceManager.hpp>
#include <framework/WindowManager.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include <vector>
#include <chrono>

#include "component/CameraComponent.hpp"
#include "component/InputComponent.hpp"
#include "geometry/baseGeometry.hpp"
#include "graphics/GPUCodeWrapper.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"

ANONYMOUS_SCOPE_BEGIN

void CubeKeyInputCallback(int nKey, int nAction, void* pContext);
void CubeMousemoveCallback(double fXPos, double fYPos, void* pContext);
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
    //record mousedeltapos
    double mouseXDelta;
    double mouseYDelta;
    bool isMousemove;
    //record keydown
    int inputKey;
    //move
    glm::float32 moveSpeed;
    glm::float32 rotateSpeed;
    //timestamp
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::milliseconds deltaTime;
    WindowState windowState;

    bool inited = false;
};

CubeDrawContext& gs_cubeDrawContext = InstanceManager::GetInstanceManager().RegisterIntance< CubeDrawContext>();

static void CubeKeyInputCallback(int nKey, int nAction, void* pContext)
{
    CubeDrawContext* pDrawContext = reinterpret_cast<CubeDrawContext*>(pContext);
    switch (nAction)
    {
        case GLFW_PRESS:
            pDrawContext->inputKey = nKey;
            break;
        case GLFW_RELEASE:
            pDrawContext->inputKey = GLFW_KEY_NONE;
        default:
            break;
    }
}

//upper-left
static void CubeMousemoveCallback(double fXPos, double fYPos, void* pContext)
{
    CubeDrawContext* pDrawContext = reinterpret_cast<CubeDrawContext*>(pContext);
    double centerWidth = static_cast<double>(pDrawContext->windowState.nWindowWidth / 2);
    double centerHeight = static_cast<double>(pDrawContext->windowState.nWindowHeight / 2);
    pDrawContext->mouseXDelta = fXPos - centerWidth;
    pDrawContext->mouseYDelta = fYPos - centerHeight;
    pDrawContext->isMousemove = true;
    WindowManager::GetInstance().SetMousePosition(centerWidth, centerHeight);
}

static void WindowResizeCallback(int nWidth, int nHeight)
{
    gs_cubeDrawContext.windowState.nWindowWidth = nWidth;
    gs_cubeDrawContext.windowState.nWindowHeight = nHeight;
}

static void FrameResizeCallback(int nWidth, int nHeight)
{
    gs_cubeDrawContext.windowState.nFrameBufferWidth = nWidth;
    gs_cubeDrawContext.windowState.nFrameBufferHeight = nHeight;
}

void InitCubeDrawContext()
{
    std::vector<Triangle> cubeTriangles = std::move(MakeCubeTriangles({-1.0f, -1.0f, -1.0f},{1.0f, 1.0f, 1.0f}));
    GLuint cubeVertexName;
    GLuint cubeBufferName;
    GLuint uVertexPointComp = 3;
    GLuint uVertexAttriLocation = 0;
    
    //gen vao
    glGenVertexArrays(1, &cubeVertexName);
    JUMP_IF_FAIL(cubeVertexName > 0);

    //gen vbo
    glGenBuffers(1, &cubeBufferName);
    JUMP_IF_FAIL(cubeBufferName > 0);

    //bind vao,after binding, vao will capture most vertexattributeInfos;
    glBindVertexArray(cubeVertexName);
    //enable this attribute for vao
    glEnableVertexAttribArray(uVertexAttriLocation);

    //bind buffer to define bufferformat
    glBindBuffer(GL_ARRAY_BUFFER, cubeBufferName);
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
    //init viewMat4 and perspectiveMat4
    gs_cubeDrawContext.cubeCamera.SetPosition({ 0, 0, -10 });
    gs_cubeDrawContext.cubeCamera.GenrateViewMatrix(gs_cubeDrawContext.viewMat4);
    gs_cubeDrawContext.cubeCamera.SetFrustum(45.0f, 1.0f, 0.5f, 20.0f);
    gs_cubeDrawContext.cubeCamera.GenratePerspectiveMatrix(gs_cubeDrawContext.perspectiveMat4);
    
    gs_cubeDrawContext.lastTime = std::chrono::high_resolution_clock::now();
    gs_cubeDrawContext.deltaTime = std::chrono::milliseconds(0);

    gs_cubeDrawContext.moveSpeed = 10.0f;
    gs_cubeDrawContext.rotateSpeed = 90.0f;

    //control,need reset every frame
    gs_cubeDrawContext.mouseXDelta = 0.0f;
    gs_cubeDrawContext.mouseYDelta = 0.0f;
    gs_cubeDrawContext.isMousemove = false;
    //don't to reset
    gs_cubeDrawContext.inputKey = GLFW_KEY_NONE;

    WindowManager::GetInstance().GetWindowState(gs_cubeDrawContext.windowState);
    WindowManager::GetInstance().RegisterWindowCallback(WindowResizeCallback, FrameResizeCallback);

    gs_cubeDrawContext.inputCamera.SetCallback(CubeKeyInputCallback , CubeMousemoveCallback, &gs_cubeDrawContext);

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
    //key control
    //in the world coordinate movement
    glm::vec4 moveDirection = glm::zero<glm::vec4>();
    glm::mat4 transposedViewMatrix = glm::transpose(gs_cubeDrawContext.viewMat4);
    switch (gs_cubeDrawContext.inputKey)
    {
        case GLFW_KEY_NONE:
            break;
        case GLFW_KEY_D:
        case GLFW_KEY_RIGHT:
            //move down the x
            moveDirection = transposedViewMatrix[0];
            break;
        case GLFW_KEY_SPACE:
            //move down the y
            moveDirection = transposedViewMatrix[1];
            break;
        case GLFW_KEY_W:
        case GLFW_KEY_UP:
            //move down the z
            moveDirection = transposedViewMatrix[2];
            break;
        case GLFW_KEY_A:
        case GLFW_KEY_LEFT:
            moveDirection = -transposedViewMatrix[0];
            break;
        case GLFW_KEY_LEFT_SHIFT:
            moveDirection = -transposedViewMatrix[1];
            break;
        case GLFW_KEY_S:
        case GLFW_KEY_DOWN:
            moveDirection = -transposedViewMatrix[2];
        default:
            break;
    }
    if (gs_cubeDrawContext.inputKey != GLFW_KEY_NONE)
    {
        moveDirection[3] = 0;
        moveDirection = glm::normalize(moveDirection);
        gs_cubeDrawContext.cubeCamera.AddDeltaPosition(moveDirection * gs_cubeDrawContext.moveSpeed * static_cast<glm::float32>(gs_cubeDrawContext.deltaTime.count()) / 1000.0f);
    }

    //mouse control
    //view rotate
    if (gs_cubeDrawContext.isMousemove)
    {
        glm::float32 deltaXDegree = gs_cubeDrawContext.rotateSpeed * static_cast<glm::float32>(gs_cubeDrawContext.deltaTime.count()) / 1000.0f;
        glm::float32 deltaYDegree = deltaXDegree;
        //fixed rotate even different delta
        
        //rotate Y
        if (glm::epsilonNotEqual<double>(gs_cubeDrawContext.mouseXDelta, 0.0, 0.01))
        {
            //counterclock
            deltaYDegree = gs_cubeDrawContext.mouseXDelta < 0 ? deltaYDegree : -deltaYDegree;
            gs_cubeDrawContext.cubeCamera.AddDeltaYDegree(deltaYDegree);
        }
        //rotate X
        if (glm::epsilonNotEqual<double>(gs_cubeDrawContext.mouseYDelta, 0.0, 0.01))
        {
            deltaXDegree = gs_cubeDrawContext.mouseYDelta < 0 ? deltaXDegree : -deltaXDegree;
            gs_cubeDrawContext.cubeCamera.AddDeltaXDegree(deltaXDegree);
        }

        gs_cubeDrawContext.mouseXDelta = 0.0f;
        gs_cubeDrawContext.mouseYDelta = 0.0f;
        gs_cubeDrawContext.isMousemove = false;
    }

    gs_cubeDrawContext.cubeCamera.GenrateViewMatrix(gs_cubeDrawContext.viewMat4);
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

    glm::mat4 transformMat = gs_cubeDrawContext.perspectiveMat4 * gs_cubeDrawContext.viewMat4 * gs_cubeDrawContext.worldPositionMat4 * gs_cubeDrawContext.localRotateMat4;
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

void FirstCube()
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
}

RegisterFrameFeaturetest(FirstCube);

ANONYMOUS_SCOPE_END