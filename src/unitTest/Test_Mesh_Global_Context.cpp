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

#include <context/GlobalContext.hpp>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/epsilon.hpp"

ANONYMOUS_SCOPE_BEGIN

class CircleGlobalContext : public BaseGlobalContext
{
public:
    CircleGlobalContext() = default;
    ~CircleGlobalContext() = default;
public:
    bool Init()
    {
        bool bResult = false;
        bool bRetCode = false;
        //init firstPersonView
        bRetCode = firstPersonView.Init();
        JUMP_IF_FAIL(bRetCode);

        lastTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::milliseconds(0);

        bResult = true;
    FAIL_STATE:
        return bResult;
    }
    bool UnInit()
    {
        firstPersonView.UnInit();
        return true;
    }
public:
    bool Update() override
    {
        //TimeUpdate
        auto curTime = std::chrono::high_resolution_clock::now();
        deltaTime = std::chrono::duration_cast<std::chrono::milliseconds>(curTime - lastTime);
        lastTime = curTime;
        firstPersonView.UpdateByDeltaTime(deltaTime.count());
        return true;
    }
};

struct CircleDrawContext
{
    //mesh
    BaseMesh* pCircleMesh = nullptr;
    CircleGlobalContext globalContext;
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

    std::shared_ptr<ShaderProgram> pShaderProgram = ShaderProgram::MakeShaderProgram("resource/Shader/circle.vertex", "resource/Shader/circle.fragment");
    JUMP_IF_FAIL(pShaderProgram);

    gs_circleContext.pCircleMesh->SetMeshShaderProgram(pShaderProgram);
    //model init
    gs_circleContext.pCircleMesh->SetMeshModelMatrix(glm::identity<glm::mat4>());

    JUMP_IF_FAIL(gs_circleContext.globalContext.Init());
    gs_circleContext.inited = true;
FAIL_STATE:
    assert(gs_circleContext.inited);
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
    gs_circleContext.globalContext.Update();
}

void UpdateUniformInProgram()
{
    assert(gs_circleContext.pCircleMesh->UseProgram());

    GLint nProgram = gs_circleContext.pCircleMesh->GetProgram();
    glUniformMatrix4fv(glGetUniformLocation(nProgram, "modelMatrix"), 1, false, &gs_circleContext.pCircleMesh->GetModelMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(nProgram, "viewMatrix"), 1, false, &gs_circleContext.globalContext.firstPersonView.GetViewMatrix()[0][0]);
    glUniformMatrix4fv(glGetUniformLocation(nProgram, "perspectiveMatrix"), 1, false, &gs_circleContext.globalContext.firstPersonView.GetPerspectiveMartix()[0][0]);
    
    glUniform3f(glGetUniformLocation(nProgram, "circleCenter"), 0.0f, 0.0f, 0.0f);
    glUniform1f(glGetUniformLocation(nProgram, "circleRadius"), 1.0f);
    glUseProgram(0);
}

void DrawCircle()
{
    gs_circleContext.pCircleMesh->UseProgram();
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
    UpdateUniformInProgram();
    DrawCircle();
}

void Test_Mesh_Global_Context()
{
    FirstCircle();
}

RegisterUniqueFrameFeaturetest(Test_Mesh_Global_Context);
ANONYMOUS_SCOPE_END