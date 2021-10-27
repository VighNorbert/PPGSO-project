#include "Footman.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Footman::mesh;
std::unique_ptr<ppgso::Texture> Footman::texture;
std::unique_ptr<ppgso::Shader> Footman::shader;

Footman::Footman() {
    position = {0,0,0};
    rotation = {0,0,0};
    scale = {1,1,1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 1.f};

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("PolyartStandard.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("mesh.obj");
}


bool Footman::update(Scene &scene, float dt) {
    position += speed * dt;

    // Rotate the object
    rotation += rotMomentum * dt;

    generateModelMatrix();

    return true;
}

void Footman::render(Scene &scene) {
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