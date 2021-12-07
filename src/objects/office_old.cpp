#include "office_old.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> OfficeOld::mesh;

std::unique_ptr<ppgso::Shader> OfficeOld::shader;
std::unique_ptr<ppgso::Shader> OfficeOld::shader_shadow;

std::unique_ptr<ppgso::Texture> OfficeOld::texture;

OfficeOld::OfficeOld(Object* parent) {
    parentObject = parent;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/buildings/office_old.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool OfficeOld::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentPosition) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void OfficeOld::render(Scene &scene, GLuint depthMap) {
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

    mesh->render();
}

void OfficeOld::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    mesh->render();
}