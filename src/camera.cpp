#include <glm/glm.hpp>

#include "camera.h"


Camera::Camera(float fow, float ratio, float near, float far) {
    float fowInRad = (ppgso::PI/180.0f) * fow;

    projectionMatrix = glm::perspective(fowInRad, ratio, near, far);
}

void Camera::update() {
    viewMatrix = glm::mat4{1.0f}
        * glm::rotate(glm::mat4{1.0f}, (ppgso::PI/180.0f) * tilt, {1, 0, 0})
        * glm::rotate(glm::mat4{1.0f}, (ppgso::PI/180.0f) * rotation, {0, 1, 0})
        * glm::translate(glm::mat4{1.0f}, -position);
}


void Camera::moveX(int dir) {
    position.x += dir * .5;
}
void Camera::moveY(int dir) {
    position.y += dir * .5;
}
void Camera::moveZ(int dir) {
    position.z += dir * .5;
}
void Camera::rotate(int dir) {
    rotation += dir * 1.5;
}