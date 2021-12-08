#ifndef PPGSO_KEYFRAME_H
#define PPGSO_KEYFRAME_H

class Keyframe {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 scale;
    glm::mat4 matrix;
    float duration;

    Keyframe(glm::mat4 matrix, float duration);

    Keyframe(float duration, glm::vec3 position, glm::vec3 rotation = {0, 0, 0}, glm::vec3 scale = {1, 1, 1});
};


#endif //PPGSO_KEYFRAME_H