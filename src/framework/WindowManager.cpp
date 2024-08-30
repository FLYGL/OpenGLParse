#include <framework/GlobalInstanceManager.hpp>
#include "WindowManager.hpp"

WindowManager& WindowManager::GetInstance()
{
    static WindowManager& s_windowMgr = InstanceManager::GetInstanceManager().RegisterIntance<WindowManager>();
    return s_windowMgr;
}

void WindowManager::FrameBufferResize(int nWidth, int nHeight)
{
    WindowState& rCurWinState = GetInstance().m_windowState;
    rCurWinState.nFrameBufferWidth = nWidth;
    rCurWinState.nFrameBufferHeight = nHeight;
    for (WindowEventCallback& rCallback : WindowManager::GetInstance().m_windows)
    {
        rCallback.m_pFrameBufferResizeCallback(nWidth, nHeight);
    }
}

void WindowManager::WindowResize(int nWidth, int nHeight)
{
    WindowState& rCurWinState = GetInstance().m_windowState;
    rCurWinState.nWindowWidth = nWidth;
    rCurWinState.nWindowHeight = nHeight;
    for (WindowEventCallback& rCallback : WindowManager::GetInstance().m_windows)
    {
        rCallback.m_pWindowResizeCallback(nWidth, nHeight);
    }
}

void WindowManager::RegisterWindowCallback(ResizeCallback winResize, ResizeCallback frameResize)
{
    WindowManager::GetInstance().m_windows.emplace_back(winResize, frameResize);
}

void WindowManager::GetWindowState(OUT WindowState& rWinState)
{
    const WindowState& rCurWinState = GetInstance().m_windowState;
    rWinState.nWindowWidth = rCurWinState.nWindowWidth;
    rWinState.nWindowHeight = rCurWinState.nWindowHeight;
    rWinState.nFrameBufferWidth = rCurWinState.nFrameBufferWidth;
    rWinState.nFrameBufferHeight = rCurWinState.nFrameBufferHeight;
}

void WindowManager::SetWindowState(GLFWwindow* pGLFWWindow)
{
    GetInstance().m_pGLFWWindow = pGLFWWindow;
    WindowState& rCurWinState = GetInstance().m_windowState;
    glfwGetWindowSize(pGLFWWindow, &rCurWinState.nWindowWidth, &rCurWinState.nWindowHeight);
    glfwGetFramebufferSize(pGLFWWindow, &rCurWinState.nFrameBufferWidth, &rCurWinState.nFrameBufferHeight);
}

void WindowManager::SetMousePosition(double fXPos, double fYPos)
{
    GLFWwindow* pGLFWWindow = GetInstance().m_pGLFWWindow;
    glfwSetCursorPos(pGLFWWindow, fXPos, fYPos);
}

bool WindowManager::HiddenMouse()
{
    GLFWwindow* pGLFWWindow = GetInstance().m_pGLFWWindow;
    glfwSetInputMode(pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    return true;
}

bool WindowManager::ShowMouse()
{
    GLFWwindow* pGLFWWindow = GetInstance().m_pGLFWWindow;
    glfwSetInputMode(pGLFWWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    return true;
}

bool WindowManager::ResetCenterMouse()
{
    WindowManager& rWindowMgr = GetInstance();
    GLFWwindow* pGLFWWindow = rWindowMgr.m_pGLFWWindow;
    double centerWidth = static_cast<double>(rWindowMgr.m_windowState.nWindowWidth / 2);
    double centerHeight = static_cast<double>(rWindowMgr.m_windowState.nWindowHeight / 2);
    WindowManager::GetInstance().SetMousePosition(centerWidth, centerHeight);
    return true;
}