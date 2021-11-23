#include "car.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_glass;

std::unique_ptr<ppgso::Shader> Car::shader;

std::unique_ptr<ppgso::Texture> Car::texture;

Car::Car(CarType carType) {
    this->carType = carType;

    if (this->carType == CarType::MuscleCar) {
        auto glass = std::make_unique<Car>(CarType::MuscleCarGlass);
        childObjects.push_back(move(glass));
    } else if (this->carType == CarType::PoliceCar) {
        auto glass = std::make_unique<Car>(CarType::PoliceCarGlass);
        childObjects.push_back(move(glass));
    }
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};


    if (!mesh_muscle_car) mesh_muscle_car = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car.obj");
    if (!mesh_muscle_car_glass) mesh_muscle_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_glass.obj");
    if (!mesh_police_car) mesh_police_car = std::make_unique<ppgso::Mesh>("objects/cars/police_car.obj");
    if (!mesh_police_car_glass) mesh_police_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/police_car_glass.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool Car::update(Scene &scene, float dt, glm::mat4 parentModelMatrix) {
    position += speed * dt;

    // Rotate the object
    rotation += rotMomentum * dt;

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

    shader->setUniform("Texture", *texture);
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
    }
}