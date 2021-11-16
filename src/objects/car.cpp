#include "car.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh;
std::unique_ptr<ppgso::Mesh> Car::mesh_glass;

std::unique_ptr<ppgso::Shader> Car::shader;

std::unique_ptr<ppgso::Texture> Car::texture;

Car::Car(CarType carType) {
    this->carType = carType;

    if (this->carType == CarType::MuscleCar) {
        auto glass = std::make_unique<Car>(CarType::MuscleCarGlass);
        childObjects.push_back(move(glass));
    }
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};


    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("muscle_car.obj");
    if (!mesh_glass) mesh_glass = std::make_unique<ppgso::Mesh>("muscle_car_glass.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("PolygonCity_Texture_02_A.bmp"));
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
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    switch (this->carType) {
        case CarType::MuscleCar:
            mesh->render();
            break;
        case CarType::MuscleCarGlass:
            mesh_glass->render();
            break;
    }
}