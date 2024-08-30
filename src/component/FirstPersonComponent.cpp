#include <glm/gtc/epsilon.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "FirstPersonComponent.hpp"

void FirstPersonComponent::FirstPersonKeyInputCallback(int nKey, int nAction, void* pContext)
{
    FirstPersonComponent* pFirstPersonContext = reinterpret_cast<FirstPersonComponent*>(pContext);
    switch (nAction)
    {
    case GLFW_PRESS:
        pFirstPersonContext->m_nInputKey = nKey;
        break;
    case GLFW_RELEASE:
        pFirstPersonContext->m_nInputKey = GLFW_KEY_NONE;
    default:
        break;
    }
}

void FirstPersonComponent::FirstPersonMousemoveCallback(double fXPos, double fYPos, void* pContext)
{
    FirstPersonComponent* pFirstPersonContext = reinterpret_cast<FirstPersonComponent*>(pContext);
    double centerWidth = static_cast<double>(pFirstPersonContext->m_windowState.nWindowWidth / 2);
    double centerHeight = static_cast<double>(pFirstPersonContext->m_windowState.nWindowHeight / 2);
    pFirstPersonContext->m_dMouseXDelta = fXPos - centerWidth;
    pFirstPersonContext->m_dMouseYDelta = fYPos - centerHeight;
    pFirstPersonContext->m_bIsMousemove = true;
}


void FirstPersonComponent::WindowResizeCallback(int nWidth, int nHeight)
{
    //TODO
}

bool FirstPersonComponent::Init()
{
    m_input.SetCallback(FirstPersonKeyInputCallback, FirstPersonMousemoveCallback, reinterpret_cast<void*>(this));
    WindowManager::GetInstance().GetWindowState(m_windowState);
    WindowManager::GetInstance().RegisterWindowCallback(WindowResizeCallback, nullptr);
    m_camera.SetPosition({ 0, 0, -10 });
    m_camera.GenrateViewMatrix(m_viewMat4);
    m_camera.SetFrustum(45.0f, 1.0f, 0.5f, 20.0f);
    m_camera.GenratePerspectiveMatrix(m_perspectiveMat4);

    m_fMoveSpeed = 15.0f;
    m_fRotateSpeed = 90.0f;

    m_dMouseXDelta = 0.0f;
    m_dMouseYDelta = 0.0f;
    m_bIsMousemove = false;
    m_bReleaseMouse = !WindowManager::GetInstance().HiddenMouse();

    m_nInputKey = GLFW_KEY_NONE;

    return true;
}

bool FirstPersonComponent::UnInit()
{
    return true;
}


void FirstPersonComponent::UpdateByDeltaTime(long long ullDeltaTime)
{
    //in the world coordinate movement
    glm::vec4 moveDirection = glm::zero<glm::vec4>();
    glm::mat4 transposedViewMatrix = glm::transpose(m_viewMat4);
    switch (m_nInputKey)
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
        break;
    case GLFW_KEY_F11:
    {
        if (m_bReleaseMouse)
        {
            WindowManager::GetInstance().HiddenMouse();
        }
        else
        {
            WindowManager::GetInstance().ShowMouse();
            WindowManager::GetInstance().ResetCenterMouse();
        }
        m_bReleaseMouse = !m_bReleaseMouse;
        m_nInputKey = GLFW_KEY_NONE;
        break;
    }
    default:
        break;
    }
    if (m_nInputKey != GLFW_KEY_NONE)
    {
        moveDirection[3] = 0;
        moveDirection = glm::normalize(moveDirection);
        m_camera.AddDeltaPosition(moveDirection * m_fMoveSpeed * static_cast<glm::float32>(ullDeltaTime) / 1000.0f);
    }

    //mouse control
    //view rotate
    if (m_bIsMousemove && !m_bReleaseMouse)
    {
        glm::float32 deltaXDegree = m_fRotateSpeed * static_cast<glm::float32>(ullDeltaTime) / 1000.0f;
        glm::float32 deltaYDegree = deltaXDegree;
        //fixed rotate even different delta

        //rotate Y
        if (glm::epsilonNotEqual<double>(m_dMouseXDelta, 0.0, 0.1))
        {
            //counterclock
            deltaYDegree = m_dMouseXDelta < 0 ? deltaYDegree : -deltaYDegree;
            m_camera.AddDeltaYDegree(deltaYDegree);
        }
        //rotate X
        if (glm::epsilonNotEqual<double>(m_dMouseYDelta, 0.0, 0.1))
        {
            deltaXDegree = m_dMouseYDelta < 0 ? deltaXDegree : -deltaXDegree;
            m_camera.AddDeltaXDegree(deltaXDegree);
        }

        m_dMouseXDelta = 0.0f;
        m_dMouseYDelta = 0.0f;
        m_bIsMousemove = false;
    }

    m_camera.GenrateViewMatrix(m_viewMat4);
    if (!m_bReleaseMouse)
    {
        WindowManager::GetInstance().ResetCenterMouse();
    }
}