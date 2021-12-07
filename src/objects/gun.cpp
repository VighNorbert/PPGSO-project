#include "gun.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Gun::mesh_pistol;
std::unique_ptr<ppgso::Mesh> Gun::mesh_bullet;

std::unique_ptr<ppgso::Shader> Gun::shader;
std::unique_ptr<ppgso::Shader> Gun::shader_shadow;

std::unique_ptr<ppgso::Texture> Gun::texture;

Gun::Gun(Object* parent, GunType gunType) {
    parentObject = parent;
    this->gunType = gunType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_pistol) mesh_pistol = std::make_unique<ppgso::Mesh>("objects/handgun/handgun.obj");
    if (!mesh_bullet) mesh_bullet = std::make_unique<ppgso::Mesh>("objects/handgun/bullet.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/Handgun_C.bmp"));
}


bool Gun::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Gun::render(Scene &scene, GLuint depthMap) {
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


    switch (this->gunType) {
        case GunType::Bullet:
            mesh_bullet->render();
            break;
        case GunType::Pistol:
            mesh_pistol->render();
            break;

    }
}

void Gun::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (this->gunType) {
        case GunType::Bullet:
            mesh_bullet->render();
            break;
        case GunType::Pistol:
            mesh_pistol->render();
            break;

    }
}
