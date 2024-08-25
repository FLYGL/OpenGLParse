#include "InputManager.hpp"
#include <framework/GlobalInstanceManager.hpp>

InputManager& InputManager::GetInstance()
{
    static InputManager& s_instance = InstanceManager::GetInstanceManager().RegisterIntance<InputManager>();
    return s_instance;
}

size_t InputManager::RegisterInputCallback(KeyInputCallback pKeyInputCallback, MousemoveCallback pMousemoveCallback, void* pContext)
{
    
    if (pKeyInputCallback)
    {
        GetInstance().m_registeredCallback.emplace_back(++GetInstance().m_uUniqueId, pKeyInputCallback, pMousemoveCallback, pContext);
        return GetInstance().m_uUniqueId;
    }
    return 0;
}

void InputManager::UnRegisterInputCallback(size_t uUniqueId)
{
    if (uUniqueId == 0)
    {
        return;
    }
    for (int i = static_cast<int>(GetInstance().m_registeredCallback.size()) - 1; i >= 0; ++i)
    {
        if (GetInstance().m_registeredCallback[i].uUniqueId == uUniqueId)
        {
            GetInstance().m_registeredCallback.erase(GetInstance().m_registeredCallback.begin() + i);
            break;
        }
    }
}

void InputManager::FireKeyInputCallback(int nKey, int nAction)
{
    for (InputCallbackInfo& rCallbackInfo : GetInstance().m_registeredCallback)
    {
        if(rCallbackInfo.pKeyInputCallback)
            rCallbackInfo.pKeyInputCallback(nKey, nAction, rCallbackInfo.pContext);
    }
}

void InputManager::FireMousemoveCallback(double xpos, double ypos)
{
    for (InputCallbackInfo& rCallbackInfo : GetInstance().m_registeredCallback)
    {
        if (rCallbackInfo.pMousemoveCallback)
            rCallbackInfo.pMousemoveCallback(xpos, ypos, rCallbackInfo.pContext);
    }
}