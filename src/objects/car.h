#ifndef PPGSO_CAR_H
#define PPGSO_CAR_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum CarType
{
    MuscleCar = 0,
    MuscleCarGlass = 1,
//    PoliceCar = 2
};

class Car : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Mesh> mesh_glass;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

    CarType carType;

public:
    /*!
     * Create new object
     */
    explicit Car(CarType carType);

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

    // Speed and rotational momentum
    glm::vec3 speed;
    glm::vec3 rotMomentum;
};


#endif //PPGSO_CAR_H
