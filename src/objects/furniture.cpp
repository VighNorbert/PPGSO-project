#include "furniture.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Furniture::mesh_table;
std::unique_ptr<ppgso::Mesh> Furniture::mesh_chair;

std::unique_ptr<ppgso::Shader> Furniture::shader;
std::unique_ptr<ppgso::Shader> Furniture::shader_shadow;

std::unique_ptr<ppgso::Texture> Furniture::texture;

Furniture::Furniture(Object* parent, FurnitureType furnitureType) {
    parentObject = parent;
    this->furnitureType = furnitureType;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if (!mesh_table) mesh_table = std::make_unique<ppgso::Mesh>("objects/table.obj");
    if (!mesh_chair) mesh_chair = std::make_unique<ppgso::Mesh>("objects/chair.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
}


bool Furniture::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Furniture::render(Scene &scene, GLuint depthMap) {
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


    switch (this->furnitureType) {
        case FurnitureType::Chair:
            mesh_chair->render();
            break;
        case FurnitureType::Table:
            mesh_table->render();
            break;

    }
}

void Furniture::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (this->furnitureType) {
        case FurnitureType::Chair:
            mesh_chair->render();
            break;
        case FurnitureType::Table:
            mesh_table->render();
            break;

    }
}