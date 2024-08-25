#include "InputManager.hpp"
#include <framework/GlobalInstanceManager.hpp>

InputManager& InputManager::GetInstance()
{
    static InputManager& s_instance = InstanceManager::GetInstanceManager().RegisterIntance<InputManager>();
    return s_instance;
}

size_t InputManager::RegisterInputCallback(InputCallback pCallback, void* pContext)
{
    
    if (pCallback)
    {
        GetInstance().m_registeredCallback.emplace_back(++GetInstance().m_uUniqueId, pCallback, pContext);
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

void InputManager::FireInputCallback(int nKey, int nAction)
{
    for (InputCallbackInfo& rCallbackInfo : GetInstance().m_registeredCallback)
    {
        rCallbackInfo.pCallback(nKey, nAction, rCallbackInfo.pContext);
    }
}