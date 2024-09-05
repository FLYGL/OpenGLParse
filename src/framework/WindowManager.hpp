#pragma once
#include <framework/EasyDefine.hpp>
#include <GLFW/glfw3.h>
#include <vector>
typedef void(*ResizeCallback)(int nWidth, int nHeight);

struct WindowState
{
    WindowState() : nWindowWidth{ 0 }, nWindowHeight{ 0 }, nFrameBufferWidth{ 0 }, nFrameBufferHeight{ 0 }
    {}
    //window param
    int nWindowWidth;
    int nWindowHeight;
    int nFrameBufferWidth;
    int nFrameBufferHeight;
};

//TODO suport multiWindow
class WindowManager
{
    struct WindowEventCallback
    {
        WindowEventCallback(ResizeCallback windowResize, ResizeCallback frameResize) :
            m_pWindowResizeCallback{ windowResize }, m_pFrameBufferResizeCallback{ frameResize }
        {}
        ~WindowEventCallback() = default;
        ResizeCallback m_pWindowResizeCallback;
        ResizeCallback m_pFrameBufferResizeCallback;
    };
public:
    WindowManager() = default;
    ~WindowManager() = default;
public:
    static void WindowResize(int nWidth, int nHeight);
    static void FrameBufferResize(int nWidth, int nHeight);
    static void RegisterWindowCallback(ResizeCallback winResize, ResizeCallback frameResize);
    static void GetWindowState(OUT WindowState& rWindowState);
    static void SetWindowState(GLFWwindow* pGLFWWindow);
    static void SetMousePosition(double fXPos, double fYPos);
    static bool HiddenMouse();
    static bool ShowMouse();
    static bool ResetCenterMouse();
    static WindowManager& GetInstance();
private:
    std::vector<WindowEventCallback> m_windows;
    WindowState m_windowState;
    GLFWwindow* m_pGLFWWindow;
};