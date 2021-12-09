#include "character.h"
#include "gun.h"
#include "money_bag.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Character::mesh_male_hoodie_sitting_muscle_car;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_jacket_driving_muscle_car;

std::unique_ptr<ppgso::Mesh> Character::mesh_male_police_driving_police_car;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_police_sitting_police_car;

std::unique_ptr<ppgso::Mesh> Character::mesh_female_business_suit_driving_van;

std::unique_ptr<ppgso::Mesh> Character::mesh_female_business_suit_standing;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_business_shirt_standing;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_business_suit_standing;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_business_suit_pushing_button;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_hoodie_shooting;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_hoodie_standing;
std::unique_ptr<ppgso::Mesh> Character::mesh_male_police_standing;

std::unique_ptr<ppgso::Shader> Character::shader;
std::unique_ptr<ppgso::Shader> Character::shader_shadow;

std::unique_ptr<ppgso::Texture> Character::texture;

Character::Character(Object *parent, CharacterType characterType) {
    parentObject = parent;
    this->characterType = characterType;

    if (this->characterType == CharacterType::MaleHoodieShooting) {
        auto pistol = std::make_unique<Gun>(this, GunType::Pistol);
        childObjects.push_back(move(pistol));

        auto money = std::make_unique<MoneyBag>(this);
        childObjects.push_back(move(money));
    }

    if (this->characterType == CharacterType::MaleHoodieSittingMuscleCar) {
        auto pistol = std::make_unique<Gun>(this, GunType::Pistol);
        pistol->rotation.x = - ppgso::PI/5;
        pistol->position = {-0.4,-0.5,0.65};
        childObjects.push_back(move(pistol));

        auto money = std::make_unique<MoneyBag>(this);
        money->position = {-0.7,0.3,0.3};
        childObjects.push_back(move(money));
    }

    if (this->characterType == CharacterType::MaleHoodieStanding) {
        auto pistol = std::make_unique<Gun>(this, GunType::Pistol);
        pistol->rotation.x = ppgso::PI/3;
        pistol->position = {-0.15,0.65,-1.5};
        childObjects.push_back(move(pistol));

        auto money = std::make_unique<MoneyBag>(this);
        childObjects.push_back(move(money));
    }

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_male_hoodie_sitting_muscle_car) mesh_male_hoodie_sitting_muscle_car = std::make_unique<ppgso::Mesh>("objects/characters/male_hoodie_sitting_muscle_car.obj");
    if (!mesh_male_jacket_driving_muscle_car) mesh_male_jacket_driving_muscle_car = std::make_unique<ppgso::Mesh>("objects/characters/male_jacket_driving_muscle_car.obj");
    if (!mesh_male_police_driving_police_car) mesh_male_police_driving_police_car = std::make_unique<ppgso::Mesh>("objects/characters/male_police_driving_police_car.obj");
    if (!mesh_male_police_sitting_police_car) mesh_male_police_sitting_police_car = std::make_unique<ppgso::Mesh>("objects/characters/male_police_sitting_police_car.obj");
    if (!mesh_female_business_suit_driving_van) mesh_female_business_suit_driving_van = std::make_unique<ppgso::Mesh>("objects/characters/female_business_suit_driving_van.obj");
    if (!mesh_female_business_suit_standing) mesh_female_business_suit_standing = std::make_unique<ppgso::Mesh>("objects/characters/female_business_suit_standing.obj");
    if (!mesh_male_business_shirt_standing) mesh_male_business_shirt_standing = std::make_unique<ppgso::Mesh>("objects/characters/male_business_shirt_standing.obj");
    if (!mesh_male_business_suit_standing) mesh_male_business_suit_standing = std::make_unique<ppgso::Mesh>("objects/characters/male_business_suit_standing.obj");
    if (!mesh_male_business_suit_pushing_button) mesh_male_business_suit_pushing_button = std::make_unique<ppgso::Mesh>("objects/characters/male_business_suit_pushing_button.obj");
    if (!mesh_male_hoodie_shooting) mesh_male_hoodie_shooting = std::make_unique<ppgso::Mesh>("objects/characters/male_hoodie_shooting.obj");
    if (!mesh_male_hoodie_standing) mesh_male_hoodie_standing = std::make_unique<ppgso::Mesh>("objects/characters/male_hoodie_standing.obj");
    if (!mesh_male_police_standing) mesh_male_police_standing = std::make_unique<ppgso::Mesh>("objects/characters/male_police_standing.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shader_shadow) shader_shadow = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool Character::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentPosition) {
    age += dt;

    if (keyframes.empty()) {
        generateModelMatrix(parentModelMatrix);
        return true;
    } else {
        return keyframesUpdate(scene);
    }
}

void Character::render(Scene &scene, GLuint depthMap) {
    shader->use();

    // Set up light
    scene.renderLight(shader);

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

    shader->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shader->setUniformInt("ShadowMap", (int)depthMap);

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
        case CharacterType::FemaleBusinessSuitDrivingVan:
            mesh_female_business_suit_driving_van->render();
            break;
        case CharacterType::FemaleBusinessSuitStanding:
            mesh_female_business_suit_standing->render();
            break;
        case CharacterType::MaleBusinessShirtStanding:
            mesh_male_business_shirt_standing->render();
            break;
        case CharacterType::MaleBusinessSuitStanding:
            mesh_male_business_suit_standing->render();
            break;
        case CharacterType::MaleHoodieShooting:
            mesh_male_hoodie_shooting->render();
            break;
        case MaleHoodieSitting:
            break;
        case CharacterType::MaleHoodieStanding:
            mesh_male_hoodie_standing->render();
            break;
        case MaleJacketDriving:
            break;
        case MaleJacketStanding:
            break;
        case MalePoliceDriving:
            break;
        case MalePoliceSitting:
            break;
        case CharacterType::MalePoliceStanding:
            mesh_male_police_standing->render();
            break;
        case CharacterType::MaleBusinessSuitPushingButton:
            mesh_male_business_suit_pushing_button->render();
            break;
    }
}

void Character::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

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
        case CharacterType::FemaleBusinessSuitDrivingVan:
            mesh_female_business_suit_driving_van->render();
            break;
        case CharacterType::FemaleBusinessSuitStanding:
            mesh_female_business_suit_standing->render();
            break;
        case CharacterType::MaleBusinessShirtStanding:
            mesh_male_business_shirt_standing->render();
            break;
        case CharacterType::MaleBusinessSuitStanding:
            mesh_male_business_suit_standing->render();
            break;
        case CharacterType::MaleHoodieShooting:
            mesh_male_hoodie_shooting->render();
            break;
        case MaleHoodieSitting:
            break;
        case CharacterType::MaleHoodieStanding:
            mesh_male_hoodie_standing->render();
            break;
        case MaleJacketDriving:
            break;
        case MaleJacketStanding:
            break;
        case MalePoliceDriving:
            break;
        case MalePoliceSitting:
            break;
        case CharacterType::MalePoliceStanding:
            mesh_male_police_standing->render();
            break;
        case CharacterType::MaleBusinessSuitPushingButton:
            mesh_male_business_suit_pushing_button->render();
            break;
    }
}
