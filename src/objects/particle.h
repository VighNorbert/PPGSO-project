#ifndef PPGSO_PARTICLE_H
#define PPGSO_PARTICLE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum ParticleType
{
    Fire = 0,
    Water = 1
};

class Particle : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> fire_texture;
    static std::unique_ptr<ppgso::Texture> water_texture;

    ParticleType particleType;

    float age = 0;
    float maxAge = 3.f;

    glm::vec3 speed = {0, 0, 0};

public:
    /*!
     * Create new object
     */
    explicit Particle(Object* parent, ParticleType apartmentType, glm::vec3 initialPosition);

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

    void renderForShadow(Scene &scene) override;
};


#endif //PPGSO_PARTICLE_H
