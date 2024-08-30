#include <framework/EasyDefine.hpp>
#include <framework/WindowManager.hpp>
#include <GL/glew.h>
#include <glm/glm.hpp>

#include "CameraComponent.hpp"
#include "InputComponent.hpp"


class FirstPersonComponent
{
public:
    FirstPersonComponent() = default;
    ~FirstPersonComponent() = default;
public:
    bool Init();
    bool UnInit();

    void UpdateByDeltaTime(long long ullDeltaTime);
    glm::mat<4, 4, GLfloat>& GetViewMatrix() { return m_viewMat4; }
    glm::mat<4, 4, GLfloat>& GetPerspectiveMartix() { return m_perspectiveMat4; }
private:
    static void FirstPersonKeyInputCallback(int nKey, int nAction, void* pContext);
    static void FirstPersonMousemoveCallback(double fXPos, double fYPos, void* pContext);
    static void WindowResizeCallback(int nWidth, int nHeight);
private:
    //baseComponent
    InputComponent m_input;
    CameraComponent m_camera;

    //cache
    glm::mat<4, 4, GLfloat> m_viewMat4;
    glm::mat<4, 4, GLfloat> m_perspectiveMat4;

    //run data
    WindowState m_windowState;
    int m_nInputKey;
    double m_dMouseXDelta;
    double m_dMouseYDelta;
    bool   m_bIsMousemove;

    glm::float32 m_fMoveSpeed;
    glm::float32 m_fRotateSpeed;
};