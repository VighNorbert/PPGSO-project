#include "bank.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Bank::mesh;

std::unique_ptr<ppgso::Shader> Bank::shader;

std::unique_ptr<ppgso::Texture> Bank::texture;

Bank::Bank() {
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/buildings/bank.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool Bank::update(Scene &scene, float dt, glm::mat4 parentModelMatrix) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Bank::render(Scene &scene) {
    shader->use();

    // Set up light
    shader->setUniform("LightDirection", scene.lightDirection);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    mesh->render();
}