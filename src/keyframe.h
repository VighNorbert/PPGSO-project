#ifndef PPGSO_KEYFRAME_H
#define PPGSO_KEYFRAME_H

class Keyframe {
public:
    glm::mat4 matrix;
    float duration;

    Keyframe(glm::mat4 matrix, float duration);
};


#endif //PPGSO_KEYFRAME_H