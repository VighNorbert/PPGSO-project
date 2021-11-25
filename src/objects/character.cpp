#include "character.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Character::mesh_male_hoodie_sitting_muscle_car;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_jacket_driving_muscle_car;

std::unique_ptr<ppgso::Mesh> Character::mesh_male_police_driving_police_car;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_police_sitting_police_car;

std::unique_ptr<ppgso::Mesh> Character::mesh_female_business_suit_driving_van;

std::unique_ptr<ppgso::Shader> Character::shader;

std::unique_ptr<ppgso::Texture> Character::texture;

Character::Character(Object *parent, CharacterType characterType) {
    parentObject = parent;
    this->characterType = characterType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_male_hoodie_sitting_muscle_car) mesh_male_hoodie_sitting_muscle_car = std::make_unique<ppgso::Mesh>("objects/characters/male_hoodie_sitting_muscle_car.obj");
    if (!mesh_male_jacket_driving_muscle_car) mesh_male_jacket_driving_muscle_car = std::make_unique<ppgso::Mesh>("objects/characters/male_jacket_driving_muscle_car.obj");
    if (!mesh_male_police_driving_police_car) mesh_male_police_driving_police_car = std::make_unique<ppgso::Mesh>("objects/characters/male_police_driving_police_car.obj");
    if (!mesh_male_police_sitting_police_car) mesh_male_police_sitting_police_car = std::make_unique<ppgso::Mesh>("objects/characters/male_police_sitting_police_car.obj");
    if (!mesh_female_business_suit_driving_van) mesh_female_business_suit_driving_van = std::make_unique<ppgso::Mesh>("objects/characters/female_business_suit_driving_van.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool Character::update(Scene &scene, float dt, glm::mat4 parentModelMatrix) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Character::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightPosition", scene.lightPosition);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    shader->setUniform("DiffuseStrength", 1.f);
    shader->setUniform("AmbientStrength", .2f);
    shader->setUniform("SpecularStrength", .2f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    switch (this->characterType) {
        case CharacterType::MalePoliceSittingPoliceCar:
            mesh_male_police_sitting_police_car->render();
            break;
        case CharacterType::MalePoliceDrivingPoliceCar:
            mesh_male_police_driving_police_car->render();
            break;
        case CharacterType::MaleHoodieSittingMuscleCar:
            mesh_male_hoodie_sitting_muscle_car->render();
            break;
        case CharacterType::MaleJacketDrivingMuscleCar:
            mesh_male_jacket_driving_muscle_car->render();
            break;
        case FemaleBusinessSuitDrivingVan:
            mesh_female_business_suit_driving_van->render();
            break;
        case FemaleBusinessSuitStanding:
            break;
        case MaleBusinessShirtStanding:
            break;
        case MaleBusinessSuitStanding:
            break;
        case MaleHoodieShooting:
            break;
        case MaleHoodieSitting:
            break;
        case MaleHoodieStanding:
            break;
        case MaleJacketDriving:
            break;
        case MaleJacketStanding:
            break;
        case MalePoliceDriving:
            break;
        case MalePoliceSitting:
            break;
        case MalePoliceStanding:
            break;
    }
}