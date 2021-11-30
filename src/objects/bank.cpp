#include "bank.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside;
std::unique_ptr<ppgso::Mesh> Bank::mesh_bank_inside_glass;

std::unique_ptr<ppgso::Shader> Bank::shader;

std::unique_ptr<ppgso::Texture> Bank::texture;

Bank::Bank(Object* parent, BankType bankType) {
    parentObject = parent;
    this->bankType = bankType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_bank) mesh_bank = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank.obj");
    if (!mesh_bank_inside) mesh_bank_inside = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside.obj");
    if (!mesh_bank_inside_glass) mesh_bank_inside_glass = std::make_unique<ppgso::Mesh>("objects/buildings/bank/bank_inside_glass.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_01_A.bmp"));
}


bool Bank::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Bank::render(Scene &scene) {
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


    switch (this->bankType) {
        case BankType::BankOutside:
            mesh_bank->render();
            break;
        case BankType::BankInside:
            mesh_bank_inside->render();
            break;
        case BankType::BankInsideGlass:
            mesh_bank_inside_glass->render();
            break;
    }
}