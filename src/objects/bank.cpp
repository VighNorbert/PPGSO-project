#include "bank.h"
#include "lightWrapper.h"
#include "character.h"
#include "particle.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_glass;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_alarm_bottom;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_alarm_top;

std::unique_ptr<ppgso::Shader> Bank::shader;
std::unique_ptr<ppgso::Shader> Bank::shader_shadow;

std::unique_ptr<ppgso::Texture> Bank::texture;

Bank::Bank(Object* parent, BankType bankType, Scene& scene) {
    parentObject = parent;
    this->bankType = bankType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_bank) mesh_bank = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank.obj");
    if (!mesh_bank_inside) mesh_bank_inside = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside.obj");
    if (!mesh_bank_inside_glass) mesh_bank_inside_glass = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_glass.obj");
    if (!mesh_bank_inside_alarm_bottom) mesh_bank_inside_alarm_bottom = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_alarm_bottom.obj");
    if (!mesh_bank_inside_alarm_top) mesh_bank_inside_alarm_top = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_alarm_top.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shader_shadow) shader_shadow = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_01_A.bmp"));

    if (bankType == BankType::BankInside) {

        auto light = new Light({1, 1, 1}, 1.0f, 0.04f, 0.0032f, 50.f);
        auto sunWrapper = std::make_unique<LightWrapper>(this, glm::vec3{0.f, 3.f, 0.f}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(sunWrapper));

        auto glass = std::make_unique<Bank>(this, BankType::BankInsideGlass, scene);
        childObjects.push_back(move(glass));

        auto alarm_base = std::make_unique<Bank>(this, BankType::BankInsideAlarmBottom, scene);
        childObjects.push_back(move(alarm_base));

        auto alarm_top = std::make_unique<Bank>(this, BankType::BankInsideAlarmTop, scene);
        childObjects.push_back(move(alarm_top));

    }
}


bool Bank::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    age += dt;

    if (scene.scene_id == 0) {
        if (age > 11 && !alarm_pushed) {
            alarm_pushed = true;

            auto alarm = new Light({1, 0, 0}, 1.0f, .75f, .5f, 50.f);
            auto alarmWrapper = std::make_unique<LightWrapper>(this, glm::vec3{0.f, 1.f, 0.f}, alarm);
            scene.lights.push_back(alarm);
            childObjects.push_back(move(alarmWrapper));

            auto character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessSuitPushingButton);
            character->keyframes = {
                    {5.f, {0, 0, 0.5}, {0, 0, ppgso::PI}},
                    {1.f, {0, 0, 0.5}, {0, 0, ppgso::PI}},
                    {-1, {0, 0.13, 0.5}, {ppgso::PI/2, ppgso::PI/2, ppgso::PI}},
            };
            scene.rootObjects.push_back(move(character));
        }else if (age > 17 && !male_dead) {
            male_dead = true;
            auto character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessSuitStanding);
            character->rotation.x = ppgso::PI/2;
            character->rotation.y = ppgso::PI/2;
            character->rotation.z = ppgso::PI;
            character->position = {0, 0.13, 0.5};
            scene.rootObjects.push_back(move(character));
        }

        if (age > 17 && age < 40){
            int maxcount_per_sec = 50;
            for (int i = 1; i <= ceil(maxcount_per_sec * dt); i++) {
                auto particle = std::make_unique<Particle>(this, ParticleType::Blood, glm::vec3{1, 0, 0.5});
                childObjects.push_back(move(particle));
            }
        }
    }

    generateModelMatrix(parentModelMatrix);

    return true;
}

void Bank::render(Scene &scene, GLuint depthMap) {
    shader->use();

    // Set up light
    scene.renderLight(shader, !(bankType == BankInside || bankType == BankInsideAlarmBottom ||  bankType == BankInsideAlarmTop));

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

    glActiveTexture(GL_TEXTURE0 + depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shader->setUniformInt("ShadowMap", (int)depthMap);

    switch (this->bankType) {
        case BankType::BankOutside:
            mesh_bank->render();
            break;
        case BankType::BankInside:
            mesh_bank_inside->render();
            break;
        case BankType::BankInsideGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_bank_inside_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case BankType::BankInsideAlarmTop:
            shader->setUniform("SpecularStrength", .7f);
            shader->setUniform("Transparency", .9f);
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            mesh_bank_inside_alarm_top->render();
            glDisable(GL_BLEND);
            break;
        case BankType::BankInsideAlarmBottom:
            mesh_bank_inside_alarm_bottom->render();
            break;
    }
}

void Bank::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (this->bankType) {
        case BankType::BankOutside:
            mesh_bank->render();
            break;
        case BankType::BankInside:
            mesh_bank_inside->render();
            break;
        case BankType::BankInsideAlarmBottom:
            mesh_bank_inside_alarm_bottom->render();
            break;
        default:
            break;
    }
}