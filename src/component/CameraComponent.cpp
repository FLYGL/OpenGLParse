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
    outMat4 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(-m_rotatedYDegree) , glm::vec3{0, 1, 0}) * outMat4;
    //x tranpose
    outMat4 = glm::rotate(glm::identity<glm::mat4>(), glm::radians(-m_rotatedXDegree), glm::vec3{1, 0, 0}) * outMat4;
}

void CameraComponent::SetFrustum(glm::float32 fFovYDegree, glm::float32 fAspect, glm::float32 fNear, glm::float32 fFar)
{
    m_fovyDegree = fFovYDegree;
    m_aspect = fAspect;
    m_nearDistance = fNear;
    m_farDistance = fFar;
}

void CameraComponent::GenratePerspectiveMatrix(OUT glm::mat4& outMat4)
{
    glm::float32 fLengthY = 2 * m_nearDistance * glm::tan(glm::radians(m_fovyDegree / 2));
    glm::float32 fLengthX = m_aspect * fLengthY;
    glm::float32 a11 = m_nearDistance / fLengthX;
    glm::float32 a22 = m_nearDistance / fLengthY;
    glm::float32 a33 = ( m_nearDistance + m_farDistance) / (m_farDistance - m_nearDistance);
    glm::float32 a34 = 2 * m_nearDistance * m_farDistance / (m_nearDistance - m_farDistance);
    
    outMat4[0][0] = a11;    outMat4[1][0] = 0.0f;   outMat4[2][0] = 0.0f;   outMat4[3][0] = 0.0f;
    outMat4[0][1] = 0.0f;   outMat4[1][1] = a22;    outMat4[2][1] = 0.0f;   outMat4[3][1] = 0.0f;
    outMat4[0][2] = 0.0f;   outMat4[1][2] = 0.0f;   outMat4[2][2] = a33;    outMat4[3][2] = a34;
    outMat4[0][3] = 0.0f;   outMat4[1][3] = 0.0f;   outMat4[2][3] = 1.0f;   outMat4[3][3] = 0.0f;
}