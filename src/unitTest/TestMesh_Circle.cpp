#include <framework/EasyDefine.hpp>
#include <framework/FeatureEnvMgr.hpp>
#include <framework/GlobalInstanceManager.hpp>
#include <framework/WindowManager.hpp>
#include <GLFW/glfw3.h>
#include <geometry/baseGeometry.hpp>
#include <helper/MeshHelper.hpp>

#include <iostream>
#include <vector>
#include <chrono>

#include "component/FirstPersonComponent.hpp"
#include "graphics/GPUCodeWrapper.hpp"

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"

ANONYMOUS_SCOPE_BEGIN
struct CircleDrawContext
{
    //mesh
    BaseMesh* pCircleMesh = nullptr;
    GLuint program;
    //glm
    glm::mat<4, 4, GLfloat> modelMat4;
    //component
    FirstPersonComponent firstPersonView;
    //timestamp
    std::chrono::steady_clock::time_point lastTime;
    std::chrono::milliseconds deltaTime;

    bool inited = false;
};

CircleDrawContext& gs_circleContext = InstanceManager::GetInstanceManager().RegisterIntance<CircleDrawContext>();

void InitCircleDrawContext()
{
    std::vector<Triangle> circleTriangles = std::move(MakeCircleQuad());
    MakeMeshHelper makeMeshHelper;
    makeMeshHelper.MakeMeshBegin();

    for(Triangle& rTriangle : circleTriangles)
    {
        for(Point& rPoint : rTriangle.vertices)
        {   
            makeMeshHelper.PushPosition(glm::vec3(rPoint.x, rPoint.y, rPoint.z));
        }
    }

    gs_circleContext.pCircleMesh = makeMeshHelper.MakeMeshEnd();
    assert(gs_circleContext.pCircleMesh);
    assert(gs_circleContext.pCircleMesh->MeshUploadGPUSync());

    gs_circleContext.program = ProgramWrapper::CreateProgramByCodePath("resource/Shader/circle.vertex", "resource/Shader/circle.fragment");
    JUMP_IF_FAIL(gs_circleContext.program > 0);

    //init firstPersonView
    gs_circleContext.firstPersonView.Init();

    gs_circleContext.lastTime = std::chrono::high_resolution_clock::now();
    gs_circleContext.deltaTime = std::chrono::milliseconds(0);
    //model init
    gs_circleContext.modelMat4 = glm::identity<glm::mat4>();
    gs_circleContext.inited = true;
FAIL_STATE:
    DO_NOTHING();
}

void PreProcess()
{
    glEnable(GL_DEPTH_CLAMP);
    glEnable(GL_DEPTH_TEST);
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
}

void BaseContextUpdate()
{
    //TimeUpdate
    auto curTime = std::chrono::high_resolution_clock::now();
    gs_circleContext.deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - gs_circleContext.lastTime);
    gs_circleContext.lastTime = curTime;
}

void UpdateCamera()
{
    gs_circleContext.firstPersonView.UpdateByDeltaTime(gs_circleContext.deltaTime.count());
}

void UpdateUniformInProgram()
{
    glUseProgram(gs_circleContext.program);
    glUniformMatrix4fv(glGetUniformLocation(gs_circleContext.program, "modelMatrix"), 1, false, &gs_circleContext.modelMat4[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(gs_circleContext.program, "viewMatrix"), 1, false, &gs_circleContext.firstPersonView.GetViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(gs_circleContext.program, "perspectiveMatrix"), 1, false, &gs_circleContext.firstPersonView.GetPerspectiveMartix()[0][0]);
    
    glUniform3f(glGetUniformLocation(gs_circleContext.program, "circleCenter"), 0.0f, 0.0f, 0.0f);
    glUniform1f(glGetUniformLocation(gs_circleContext.program, "circleRadius"), 1.0f);
    glUseProgram(0);
}

void DrawCircle()
{
    glUseProgram(gs_circleContext.program);
    gs_circleContext.pCircleMesh->DrawVAO();
}

void FirstCircle()
{
    if (!gs_circleContext.inited)
    {
        InitCircleDrawContext();
    }
    PreProcess();
    BaseContextUpdate();
    UpdateCamera();
    UpdateUniformInProgram();
    DrawCircle();
}

void TestMesh_Circle()
{
    FirstCircle();
}

// RegisterUniqueFrameFeaturetest(TestMesh_Circle);
ANONYMOUS_SCOPE_END