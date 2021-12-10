#ifndef PPGSO_LIGHTWRAPPER_H
#define PPGSO_LIGHTWRAPPER_H

#include "src/object.h"
#include "src/light.h"

class LightWrapper: public Object {
public:

    Light* light;

    LightWrapper(Object* parent, Light* light);
    LightWrapper(Object* parent, glm::vec3 position, Light* light);

    void checkCollisions(Scene &scene, float dt) override {};

    /*!
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene, GLuint depthMap) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void renderForShadow(Scene &scene) override;
};


#endif //PPGSO_LIGHTWRAPPER_H