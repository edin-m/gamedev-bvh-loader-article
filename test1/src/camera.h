#ifndef CAMERA_H
#define CAMERA_H

#include "window.h"

class Camera
{
    glm::vec3 location;
    glm::vec3 lookat;
    glm::vec3 upvec;

    glm::mat4 projection;
    glm::mat4 view;

    float znear, zfar;
    float fovy, m_aspect;

    void updateMatrices()
    {
        view = glm::lookAt(location, lookat, upvec);
        projection = glm::perspective(fovy, m_aspect, znear, zfar);
    }

public:
    Camera(WindowX11& win)
    {
        location = glm::vec3(0, 4, 20);
        lookat   = glm::vec3(0, 2, 0);
        upvec    = glm::vec3(0, 1, 0);
        znear = 0.01f;
        zfar  = 10000.0f;
        fovy  = 45.0f;
        m_aspect = win.width / win.height;

        updateMatrices();
    }

    void setProjection(glm::mat4& mat) { projection = mat; }
    void setView(glm::mat4& mat) { view = mat; }

    glm::mat4 matrix() const { return projection * view; }

    float aspect() const { return m_aspect; }
    void  aspect(float ratio) { m_aspect = ratio; updateMatrices(); }
};

#endif // CAMERA_H
