#include "GlobalInstanceManager.hpp"
InstanceManager& InstanceManager::GetInstanceManager()
{
    static InstanceManager s_instance;
    return s_instance;
}

InstanceManager::~InstanceManager()
{
    for (int i = static_cast<int>(m_instances.size()) - 1; i >= 0; --i)
    {
        InstanceEntry& rEntry = m_instances[i];
        rEntry.pFuncDeconstruct(rEntry.pInstance);
    }
    //for (InstanceEntry& rEntry : m_instances)
    //{
    //    rEntry.pFuncDeconstruct(rEntry.pInstance);
    //}
}