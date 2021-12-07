#include <glm/glm.hpp>

#include "mainlight.h"

MainLight::MainLight(glm::vec3 color, float constant, float linear, float quadratic, float maxDist) {
    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    this->maxDist = maxDist;

    this->color = color;
}

MainLight::MainLight(glm::vec3 color, glm::vec3 direction, float cutOffDeg, float outerCutOffDeg, float constant, float linear, float quadratic, float maxDist) {
    this->isSpotlight = true;
    this->localDirection = direction;
    this->cutOff = glm::cos(glm::radians(cutOffDeg));
    this->outerCutOff = glm::cos(glm::radians(outerCutOffDeg));

    this->constant = constant;
    this->linear = linear;
    this->quadratic = quadratic;

    this->maxDist = maxDist;

    this->color = color;
}