#include "car.h"
#include "character.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_wheel;

std::unique_ptr<ppgso::Mesh> Car::mesh_police_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_wheel;

std::unique_ptr<ppgso::Mesh> Car::mesh_van;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_wheel;

std::unique_ptr<ppgso::Shader> Car::shader;

std::unique_ptr<ppgso::Texture> Car::texture;
std::unique_ptr<ppgso::Texture> Car::texture_van;

Car::Car(Object* parent, CarType carType) {
    parentObject = parent;
    this->carType = carType;

    if (this->carType == CarType::MuscleCar) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::MaleJacketDrivingMuscleCar);
        childObjects.push_back(move(char_driving));

        auto char_sitting = std::make_unique<Character>(this, CharacterType::MaleHoodieSittingMuscleCar);
        childObjects.push_back(move(char_sitting));

        auto wheel_rf = std::make_unique<Car>(this, CarType::MuscleCarWheel);
        wheel_rf->position = {-0.88974, 0.41045, 1.935};
        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::MuscleCarWheel);
        wheel_rb->position = {-0.88974, 0.41045, -1.365};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::MuscleCarWheel);
        wheel_lf->position = {0.88974, 0.41045, 1.935};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::MuscleCarWheel);
        wheel_lb->position = {0.88974, 0.41045, -1.365};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        auto glass = std::make_unique<Car>(this, CarType::MuscleCarGlass);
        childObjects.push_back(move(glass));

    } else if (this->carType == CarType::PoliceCar) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::MalePoliceDrivingPoliceCar);
        childObjects.push_back(move(char_driving));

        auto char_sitting = std::make_unique<Character>(this, CharacterType::MalePoliceSittingPoliceCar);
        childObjects.push_back(move(char_sitting));

        auto wheel_rf = std::make_unique<Car>(this, CarType::PoliceCarWheel);
        wheel_rf->position = {-0.83132, 0.37224, 1.6563};

        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::PoliceCarWheel);
        wheel_rb->position = {-0.83132, 0.37224, -1.2337};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::PoliceCarWheel);
        wheel_lf->position = {0.83132, 0.37224, 1.6563};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::PoliceCarWheel);
        wheel_lb->position = {0.83132, 0.37224, -1.2337};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        auto glass = std::make_unique<Car>(this, CarType::PoliceCarGlass);
        childObjects.push_back(move(glass));

    } else if (this->carType == CarType::Van) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::FemaleBusinessSuitDrivingVan);
        childObjects.push_back(move(char_driving));

        auto wheel_rf = std::make_unique<Car>(this, CarType::VanWheel);
        wheel_rf->position = {-0.95778, 0.39926, 1.8444};

        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::VanWheel);
        wheel_rb->position = {-0.95778, 0.39926, -1.4616};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::VanWheel);
        wheel_lf->position = {0.95778, 0.39926, 1.8444};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::VanWheel);

        wheel_lb->position = {0.95778, 0.39926, -1.4616};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        auto glass = std::make_unique<Car>(this, CarType::VanGlass);
        childObjects.push_back(move(glass));
    }

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};


    if (!mesh_muscle_car) mesh_muscle_car = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car.obj");
    if (!mesh_muscle_car_glass) mesh_muscle_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_glass.obj");
    if (!mesh_muscle_car_wheel) mesh_muscle_car_wheel = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_wheel.obj");
    if (!mesh_police_car) mesh_police_car = std::make_unique<ppgso::Mesh>("objects/cars/police_car.obj");
    if (!mesh_police_car_glass) mesh_police_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/police_car_glass.obj");
    if (!mesh_police_car_wheel) mesh_police_car_wheel = std::make_unique<ppgso::Mesh>("objects/cars/police_car_wheel.obj");
    if (!mesh_van) mesh_van = std::make_unique<ppgso::Mesh>("objects/cars/van.obj");
    if (!mesh_van_glass) mesh_van_glass = std::make_unique<ppgso::Mesh>("objects/cars/van_glass.obj");
    if (!mesh_van_wheel) mesh_van_wheel = std::make_unique<ppgso::Mesh>("objects/cars/van_wheel.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
    if (!texture_van) texture_van = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_01_A.bmp"));
}


bool Car::update(Scene &scene, float dt, glm::mat4 parentModelMatrix) {
    position += speed * dt;

    if (isWheel() && parentObject != nullptr) {
        rotation.x += dt * glm::length(parentObject->speed) / getWheelDiameter();
        if (rotation.x > 2 * ppgso::PI)
            rotation.x -= float(int(rotation.x / (2 * ppgso::PI)) * 2 * ppgso::PI);
    }

    generateModelMatrix(parentModelMatrix);

    return true;
}

void Car::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightPosition", scene.lightPosition);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    shader->setUniform("DiffuseStrength", 1.f);
    shader->setUniform("AmbientStrength", .3f);
    shader->setUniform("SpecularStrength", 2.5f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    if(carType == CarType::Van){
        shader->setUniform("Texture", *texture_van);
    } else{
        shader->setUniform("Texture", *texture);
    }

    shader->setUniform("Transparency", 1.f);

    switch (this->carType) {
        case CarType::MuscleCar:
            mesh_muscle_car->render();
            break;
        case CarType::MuscleCarGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_muscle_car_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::MuscleCarWheel:
            mesh_muscle_car_wheel->render();
            break;
        case CarType::PoliceCar:
            mesh_police_car->render();
            break;
        case CarType::PoliceCarGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_police_car_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::PoliceCarWheel:
            mesh_police_car_wheel->render();
            break;
        case CarType::Van:
            mesh_van->render();
            break;
        case CarType::VanGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_van_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::VanWheel:
            mesh_van_wheel->render();
            break;
    }
}

bool Car::isWheel() {
    return carType == CarType::MuscleCarWheel
        || carType == CarType::PoliceCarWheel
        || carType == CarType::VanWheel;
}

float Car::getWheelDiameter() {
    switch(carType) {
        case CarType::MuscleCarWheel:
            return 0.41045;
        case CarType::PoliceCarWheel:
            return 0.37224;
        case CarType::VanWheel:
            return 0.39926;
        default:
            return 0;
    }
}