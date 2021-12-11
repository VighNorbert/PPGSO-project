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
    double shootFireAge = 0.0;
    float dispersion;
    bool willHit = false;
    double timeBetweenShoots;
    int shootsNumber = 1;
    float correctDispersion;

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

    void shoot(Object* parent, float bodyWidth, float age, float probability, int shootsFired) {
        if (shootFireAge == 0.0){
            correctDispersion = bodyWidth/2 * 10 / 3.07f;
            shootFireAge = age;
            dispersion = (bodyWidth/2 / probability) * 10 / 3.07f;
            timeBetweenShoots = 4.9/(1.0 * shootsFired);
        }

        if (age >= shootFireAge && shootsNumber <= shootsFired) {
            auto bullet = std::make_unique<Gun>(parent, GunType::Bullet);
            bullet->speed.z = 10;
            bullet->speed.x = glm::linearRand(-dispersion, dispersion) + 0.65f;
            bullet->speed.y = glm::linearRand(-0.9f, .0f);
            bullet->scale = {2,2,2};
            bullet->position = {-0.2, 1.43, 1};

            if (shootsNumber == shootsFired) {
                bullet->speed.x = glm::linearRand(-correctDispersion, correctDispersion) + 0.65f;
                bullet->speed.y = 0.f;
            }

            if (bullet->speed.x >= - correctDispersion + 0.65f && bullet->speed.x <= correctDispersion + 0.65f) {
                bullet->willHit = true;
            }

            childObjects.push_back(move(bullet));

            shootFireAge += timeBetweenShoots;
            shootsNumber += 1;
        }
    }
};

#endif //PPGSO_GUN_H
