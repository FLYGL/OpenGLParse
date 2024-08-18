#include "CameraComponent.hpp"
#include <glm/gtc/matrix_transform.hpp>

CameraComponent::CameraComponent() : 
    m_rotatedXDegree{0.0f}, m_rotatedYDegree{0.0f}, m_cameraPosition{0.0f, 0.0f, 0.0f}
{}

void CameraComponent::SetXDegree(glm::float32 newXDegree)
{
    m_rotatedXDegree = newXDegree;
}

void CameraComponent::SetYDegree(glm::float32 newYDegree)
{
    m_rotatedYDegree = newYDegree;
}

void CameraComponent::SetPosition(const glm::vec3& newPosition)
{
    m_cameraPosition = newPosition;
}

void CameraComponent::AddDeltaXDegree(glm::float32 deltaXDegree)
{
    m_rotatedXDegree += deltaXDegree;
}

void CameraComponent::AddDeltaYDegree(glm::float32 deltaYDegree)
{
    m_rotatedYDegree += deltaYDegree;
}

void CameraComponent::AddDeltaPosition(const glm::vec3& deltaPosition)
{
    m_cameraPosition += deltaPosition;
}

void CameraComponent::GenrateViewMatrix(OUT glm::mat4& outMat4)
{
    //tranlate tranpose first
    outMat4 =  glm::translate(glm::identity<glm::mat4>(), -m_cameraPosition);
    //y tranpose
    outMat4 = glm::rotate(glm::identity<glm::mat4>(), -m_rotatedYDegree, glm::vec3{ 0, 1, 0 }) * outMat4;
    //x tranpose
    outMat4 = glm::rotate(glm::identity<glm::mat4>(), -m_rotatedXDegree, glm::vec3{1, 0, 0}) * outMat4;
}