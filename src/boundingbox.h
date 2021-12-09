#ifndef PPGSO_BOUNDINGBOX_H
#define PPGSO_BOUNDINGBOX_H

#include "src/scene.h"
#include "src/object.h"

class BoundingBox : public Object {
private:

    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

public:
    glm::vec3 originalRotation{0.f, 0.f, 0.f};


    BoundingBox(glm::vec3 position, glm::vec3 size, glm::vec3 rotation = {0.f, 0.f, 0.f});

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


#endif //PPGSO_BOUNDINGBOX_H