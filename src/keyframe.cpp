#include <glm/glm.hpp>

#include "keyframe.h"

Keyframe::Keyframe(glm::mat4 matrix, float duration) {
    this->matrix = matrix;
    this->duration = duration;
}