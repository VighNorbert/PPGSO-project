#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>

#include "lightWrapper.h"

LightWrapper::LightWrapper(Object *parent, Light *light) {
    parentObject = parent;
    this->light = light;
}

LightWrapper::LightWrapper(Object *parent, glm::vec3 position, Light *light) {
    parentObject = parent;
    this->position = position;
    this->light = light;
}

bool LightWrapper::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    light->position = parentModelMatrix * glm::vec4(position, 1.f);
    light->direction = glm::orientate4(globalRotation) * glm::vec4(light->localDirection, 1.f);
    return true;
}

void LightWrapper::render(Scene &scene) {}

