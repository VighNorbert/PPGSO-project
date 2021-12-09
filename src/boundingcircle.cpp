#include <glm/glm.hpp>

#include "boundingcircle.h"
#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

std::unique_ptr<ppgso::Mesh> BoundingCircle::mesh;
std::unique_ptr<ppgso::Shader> BoundingCircle::shader;
std::unique_ptr<ppgso::Texture> BoundingCircle::texture;

BoundingCircle::BoundingCircle(glm::vec3 position, float radius) {
    float diameter = radius * 2;
    this->position = position;
    this->radius = radius;
    this->scale = glm::vec3{diameter};

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/sphere.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/bb.bmp"));
}

bool BoundingCircle::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);
    return true;
}

void BoundingCircle::render(Scene &scene, GLuint depthMap) {
    if (!scene.showBoundingBoxes)
        return;

    shader->use();

    // Set up light
    scene.renderLight(shader);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    shader->setUniform("DiffuseStrength", 0.f);
    shader->setUniform("AmbientStrength", 1.f);
    shader->setUniform("SpecularStrength", 0.f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    shader->setUniform("Texture", *texture);

    shader->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader->setUniform("Transparency", .5f);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    mesh->render();

    glDisable(GL_BLEND);
}

void BoundingCircle::renderForShadow(Scene &scene) {}