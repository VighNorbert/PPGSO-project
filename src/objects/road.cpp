#include "road.h"

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Road::mesh;
std::unique_ptr<ppgso::Texture> Road::texture;
std::unique_ptr<ppgso::Shader> Road::shader;

Road::Road() {
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};

    if (!shader) shader = std::make_unique<ppgso::Shader>(diffuse_vert_glsl, diffuse_frag_glsl);
    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("PolygonCity_Road_Side.bmp"));
    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("road.obj");
}


bool Road::update(Scene &scene, float dt) {
    position += speed * dt;

    // Rotate the object
    rotation += rotMomentum * dt;

    generateModelMatrix();

    return true;
}

void Road::render(Scene &scene) {
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