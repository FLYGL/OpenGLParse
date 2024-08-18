#include "InputComponent.hpp"

InputComponent::InputComponent(InputCallback pCallback, void* pContext) : m_uUniqueId {0}
{
    m_uUniqueId = InputManager::RegisterInputCallback(pCallback, pContext);
}

InputComponent::~InputComponent()
{
    InputManager::UnRegisterInputCallback(m_uUniqueId);
}