#ifndef PPGSO_LIGHT_H
#define PPGSO_LIGHT_H

#include "object.h"

class Light {
public:
    glm::vec3 position{0, 0, 0};

    bool isSpotlight = false;
    glm::vec3 localDirection;
    glm::vec3 direction;
    float cutOff = 30.f;
    float outerCutOff = 35.f;
    float maxDist = -1.f;

    float constant = 1.0f;
    float linear = 0.0f;
    float quadratic = 0.0f;

    glm::vec3 color{1, 1, 1};

    Light() = default;

    Light(glm::vec3 color, float constant, float linear, float quadratic, float maxDist);

    Light(glm::vec3 color, glm::vec3 direction, float cutOff, float outerCutOff, float constant, float linear, float quadratic, float maxDist);

};


#endif //PPGSO_LIGHT_H