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
    MuscleCarFrontLight = 3,
    MuscleCarBackLight = 4,
    PoliceCar = 5,
    PoliceCarGlass = 6,
    PoliceCarWheel = 7,
    PoliceCarFrontLight = 8,
    PoliceCarBackLight = 9,
    Van = 10,
    VanGlass = 11,
    VanWheel = 12,
    VanFrontLight = 13,
    VanBackLight = 14,
    Firetruck = 15,
    FiretruckWheel = 16
};

class Car : public Object {
private:

    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_wheel;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_front_light;
    static std::unique_ptr<ppgso::Mesh> mesh_muscle_car_back_light;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_wheel;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_front_light;
    static std::unique_ptr<ppgso::Mesh> mesh_police_car_back_light;
    static std::unique_ptr<ppgso::Mesh> mesh_van;
    static std::unique_ptr<ppgso::Mesh> mesh_van_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_van_wheel;
    static std::unique_ptr<ppgso::Mesh> mesh_van_front_light;
    static std::unique_ptr<ppgso::Mesh> mesh_van_back_light;
    static std::unique_ptr<ppgso::Mesh> mesh_firetruck;
    static std::unique_ptr<ppgso::Mesh> mesh_firetruck_wheel;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> color_shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> texture;
    static std::unique_ptr<ppgso::Texture> texture_van;
    static std::unique_ptr<ppgso::Texture> texture_firetruck;

    CarType carType;

    bool not_sitting = true;
    bool police_alive = true;

public:
    /*!
     * Create new object
     */
    Car(Object* parent, CarType carType, Scene& scene);

    /*!
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) override;

    /*!
     * @param scene Scene to render in
     */
    void render(Scene &scene, GLuint depthMap) override;

    bool isWheel();

    float getWheelDiameter();

    /*!
  * Render asteroid
  * @param scene Scene to render in
  */
    void renderForShadow(Scene &scene) override;

};


#endif //PPGSO_CAR_H
