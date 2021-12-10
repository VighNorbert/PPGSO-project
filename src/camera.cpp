#include <glm/glm.hpp>

#include "camera.h"
#include "scene.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

glm::mat4 interpolate(glm::mat4& _mat1, glm::mat4& _mat2, float _time)
{
    glm::quat rot0 = glm::quat_cast(_mat1);
    glm::quat rot1= glm::quat_cast(_mat2);

    glm::quat finalRot = glm::slerp(rot0, rot1, _time);

    glm::mat4 finalMat = glm::mat4_cast(finalRot);

    finalMat[3] = _mat1[3] * (1 - _time) + _mat2[3] * _time;

    return finalMat;
}

void Camera::update(float time) {
    if (keyframes.empty()) {
        viewMatrix = glm::mat4{1.0f}
                     * glm::rotate(glm::mat4{1.0f}, (ppgso::PI / 180.0f) * tilt, {1, 0, 0})
                     * glm::rotate(glm::mat4{1.0f}, (ppgso::PI / 180.0f) * rotation, {0, 1, 0})
                     * glm::translate(glm::mat4{1.0f}, -position);
    } else {
        age += time;
        float t = 0.0f;
        glm::mat4 actual, next, last;
        float duration;
        for (auto iter= keyframes.begin(); iter != keyframes.end(); iter++) {
            if (t + iter->duration > age) {
                actual = iter->matrix;
                duration = iter->duration;
                iter++;
                if (iter == keyframes.end()) {
                    viewMatrix = actual;
                    return;
                }
                next = iter->matrix;
                float a = (age - t) / duration;
                viewMatrix = interpolate(actual, next, a);
                return;
            }
            last = iter->matrix;
            t += iter->duration;
        }
        viewMatrix = last;
    }
}


void Camera::moveX(int dir) {
    position.x += dir * .5;
    debug();
}
void Camera::moveY(int dir) {
    position.y += dir * .5;
    debug();
}
void Camera::moveZ(int dir) {
    position.z += dir * .5;
    debug();
}
void Camera::rotate(int dir) {
    rotation += dir * 1.5;
    debug();
}

void Camera::debug() {
    if (debugEnabled) {
        std::cout << "Debug: Camera position " << position.x << " " << position.y << " " << position.z << " " << rotation << std::endl;
    }
}