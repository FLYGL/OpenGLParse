#include "InputComponent.hpp"

InputComponent::InputComponent(KeyInputCallback pKeyInputCallback, MousemoveCallback pMousemoveCallback, void* pContext) : m_uUniqueId {0}
{
    m_uUniqueId = InputManager::RegisterInputCallback(pKeyInputCallback, pMousemoveCallback, pContext);
}

void InputComponent::SetCallback(KeyInputCallback pKeyInputCallback, MousemoveCallback pMousemoveCallback, void* pContext)
{
    m_uUniqueId = InputManager::RegisterInputCallback(pKeyInputCallback, pMousemoveCallback, pContext);
}

InputComponent::~InputComponent()
{
    InputManager::UnRegisterInputCallback(m_uUniqueId);
}