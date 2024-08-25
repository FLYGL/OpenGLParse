#pragma once
#include <framework/EasyDefine.hpp>
#include <glm/glm.hpp>

//camrea init loot at z aix for camera, up direction is y aix
//define: rotate Y first, then rotate X
//Y Degree is fron 0 to 360, counterclock
//X Degree is from -90 to 90, counterclock
class CameraComponent
{
public:
    CameraComponent();
    ~CameraComponent() = default;
public:
    void SetXDegree(glm::float32);
    void SetYDegree(glm::float32);
    void SetPosition(const glm::vec3&);

    void AddDeltaXDegree(glm::float32);
    void AddDeltaYDegree(glm::float32);
    void AddDeltaPosition(const glm::vec3&);
    //apply order :  xdegree, ydegree, position
    //left hand
    void GenrateViewMatrix(OUT glm::mat4&);

    void SetFrustum(glm::float32 fFovYDegree, glm::float32 fAspect, glm::float32 fNear, glm::float32 fFar);
    //without divided by w
    //left hand
    void GenratePerspectiveMatrix(OUT glm::mat4&);
private:
    //view params
    glm::float32 m_rotatedXDegree;
    glm::float32 m_rotatedYDegree;
    glm::vec3 m_cameraPosition;
    //frustum params
    glm::float32 m_fovyDegree;
    //aspect = x/y
    glm::float32 m_aspect;
    glm::float32 m_nearDistance;
    glm::float32 m_farDistance;
};