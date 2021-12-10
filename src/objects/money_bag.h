#ifndef PPGSO_MONEY_BAG_H
#define PPGSO_MONEY_BAG_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

class MoneyBag : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create new object
     */
    MoneyBag(Object* parent);

    void checkCollisions(Scene &scene, float dt) override {};

    /*!
     * Update asteroid
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

#endif //PPGSO_MONEY_BAG_H
