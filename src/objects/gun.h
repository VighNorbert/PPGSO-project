#ifndef PPGSO_GUN_H
#define PPGSO_GUN_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum GunType
{
    Pistol = 0,
    Bullet = 1
};


class Gun : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_pistol;
    static std::unique_ptr<ppgso::Mesh> mesh_bullet;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> texture;

    GunType gunType;

public:
    /*!
     * Create new object
     */
    Gun(Object* parent, GunType gunType);

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

#endif //PPGSO_GUN_H
