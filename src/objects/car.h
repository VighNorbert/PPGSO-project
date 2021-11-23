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
    MuscleCarWheel = 2,
    PoliceCar = 3,
    PoliceCarGlass = 4,
    PoliceCarWheel = 5,
    Van = 6,
    VanGlass = 7,
    VanWheel = 8
};

class Car : public Object {
private:

    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_wheel;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_wheel;
    static std::unique_ptr<ppgso::Mesh> mesh_van;
    static std::unique_ptr<ppgso::Mesh> mesh_van_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_van_wheel;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;
    static std::unique_ptr<ppgso::Texture> texture_van;

    CarType carType;

public:
    /*!
     * Create new object
     */
    Car(Object* parent, CarType carType);

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

    bool isWheel();

    float getWheelDiameter();

};


#endif //PPGSO_CAR_H
