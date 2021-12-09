#ifndef PPGSO_BOUNDINGCIRCLE_H
#define PPGSO_BOUNDINGCIRCLE_H

#include "src/scene.h"
#include "src/object.h"

class BoundingCircle : public Object {
private:

    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

public:

    BoundingCircle(glm::vec3 position, float radius);

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

    float radius;
};


#endif //PPGSO_BOUNDINGCIRCLE_H