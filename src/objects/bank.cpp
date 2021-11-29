#include "bank.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_walls;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_floor;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_doors;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_windows;

std::unique_ptr<ppgso::Shader> Bank::shader;
std::unique_ptr<ppgso::Shader> Bank::color_shader;

std::unique_ptr<ppgso::Texture> Bank::texture;

Bank::Bank(Object* parent, BankType bankType) {
    parentObject = parent;
    this->bankType = bankType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_bank) mesh_bank = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank.obj");
    if (!mesh_bank_inside_walls) mesh_bank_inside_walls = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_walls.obj");
    if (!mesh_bank_inside_floor) mesh_bank_inside_floor = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_floor.obj");
    if (!mesh_bank_inside_doors) mesh_bank_inside_doors = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_doors.obj");
    if (!mesh_bank_inside_windows) mesh_bank_inside_windows = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_windows.obj");

    if (!color_shader) color_shader = std::make_unique<ppgso::Shader>(color_vert_glsl, color_frag_glsl);
    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_01_A.bmp"));
}


bool Bank::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Bank::render(Scene &scene) {
    if (bankType == BankType::BankInsideWalls || bankType == BankType::BankInsideFloor) {
        color_shader->use();

        if (bankType == BankType::BankInsideWalls){
            color_shader->setUniform("OverallColor", {1.0f, 0.975f, 0.853f});
        } else {
            color_shader->setUniform("OverallColor", {0.5f, 0.975f, 0.853f});
        }

        // use camera
        color_shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
        color_shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

        // render mesh
        color_shader->setUniform("ModelMatrix", modelMatrix);
    } else {
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
    }

    switch (this->bankType) {
        case BankType::BankOutside:
            mesh_bank->render();
            break;
        case BankType::BankInsideWalls:
            mesh_bank_inside_walls->render();
            break;
        case BankType::BankInsideFloor:
            mesh_bank_inside_floor->render();
            break;
        case BankType::BankInsideDoors:
            mesh_bank_inside_doors->render();
            break;
        case BankType::BankInsideWindows:
            mesh_bank_inside_windows->render();
            break;
    }
}