#ifndef PPGSO_MAINLIGHT_H
#define PPGSO_MAINLIGHT_H

#include "object.h"
#include "light.h"
#include "glm/gtc/matrix_transform.hpp"
#include "camera.h"

class MainLight : Light {
public:
    MainLight() = default;

    MainLight(glm::vec3 color, float constant, float linear, float quadratic, float maxDist);

    MainLight(glm::vec3 color, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, float maxDist);

    float near_plane = 1.0f, far_plane = 1000.f;
    glm::mat4 lightProjection = glm::ortho(-50.0f, 50.0f, 50.0f, -50.0f, near_plane, far_plane);

    glm::mat4 getLightView(Camera camera) {
        return glm::lookAt(glm::vec3(50.f, 50.f, -50.f),
                    glm::vec3(0.0f, 0.0f, 0.0f),
                    glm::vec3(0.0f, 1.0f, 0.0f));
    }

};


#endif //PPGSO_MAINLIGHT_H