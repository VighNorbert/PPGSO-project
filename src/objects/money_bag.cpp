#include "money_bag.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> MoneyBag::mesh;


std::unique_ptr<ppgso::Shader> MoneyBag::shader;
std::unique_ptr<ppgso::Shader> MoneyBag::shader_shadow;

std::unique_ptr<ppgso::Texture> MoneyBag::texture;

MoneyBag::MoneyBag(Object* parent) {
    parentObject = parent;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/money_bag.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shader_shadow) shader_shadow = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool MoneyBag::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void MoneyBag::render(Scene &scene, GLuint depthMap) {
    shader->use();

    // Set up light
    scene.renderLight(shader, false);

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

    mesh->render();
}

void MoneyBag::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    mesh->render();
}