#include <glm/glm.hpp>

#include "light.h"

Light::Light(glm::vec3 color, float constant, float linear, float quadratic) {
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    this->color = color;
}

Light::Light(glm::vec3 color, glm::vec3 direction, float cutOffDeg, float outerCutOffDeg, float constant, float linear, float quadratic) {
    this->isSpotlight = true;
    this->localDirection = direction;
    this->cutOff = glm::cos(glm::radians(cutOffDeg));
    this->outerCutOff = glm::cos(glm::radians(outerCutOffDeg));

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    this->color = color;
}