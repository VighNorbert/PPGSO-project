#include "apartment.h"
#include "../texture.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Apartment::mesh_door_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_door_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_3;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_3;

std::unique_ptr<ppgso::Mesh> Apartment::mesh_door_corner_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_door_corner_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_corner_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_corner_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_corner_3;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_corner_1;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_corner_2;
std::unique_ptr<ppgso::Mesh> Apartment::mesh_roof_corner_3;

std::unique_ptr<ppgso::Shader> Apartment::shader;
std::unique_ptr<ppgso::Shader> Apartment::shader_shadow;

Apartment::Apartment(Object* parent, ApartmentType apartmentType) : texture(TextureGenerator::random()) {
    parentObject = parent;
    this->apartmentType = apartmentType;

    if (!mesh_door_1) mesh_door_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_door_1.obj");
    if (!mesh_door_2) mesh_door_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_door_2.obj");
    if (!mesh_1) mesh_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_1.obj");
    if (!mesh_2) mesh_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_2.obj");
    if (!mesh_3) mesh_3 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_3.obj");
    if (!mesh_roof_1) mesh_roof_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_roof_1.obj");
    if (!mesh_roof_2) mesh_roof_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_roof_2.obj");
    if (!mesh_roof_3) mesh_roof_3 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment/apartment_roof_3.obj");

    if (!mesh_door_corner_1) mesh_door_corner_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_door_corner_1.obj");
    if (!mesh_door_corner_2) mesh_door_corner_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_door_corner_2.obj");
    if (!mesh_corner_1) mesh_corner_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_corner_1.obj");
    if (!mesh_corner_2) mesh_corner_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_corner_2.obj");
    if (!mesh_corner_3) mesh_corner_3 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_corner_3.obj");
    if (!mesh_roof_corner_1) mesh_roof_corner_1 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_roof_corner_1.obj");
    if (!mesh_roof_corner_2) mesh_roof_corner_2 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_roof_corner_2.obj");
    if (!mesh_roof_corner_3) mesh_roof_corner_3 = std::make_unique<ppgso::Mesh>("objects/buildings/apartment_corner/apartment_roof_corner_3.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shader_shadow) shader_shadow = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};

    if(this->apartmentType == ApartmentType::normal){
        abType = static_cast<ApartmentBlockType>(rand() % 2);

        int height = rand() % 3 + 3;
        auto blockType = static_cast<ApartmentBlockType>(rand() % 3 + 2);
        auto roofType = static_cast<ApartmentBlockType>(rand() % 3 + 5);
        for (int i = 1; i < height; i++) {
            auto aptblock = std::make_unique<Apartment>(this, blockType, texture);
            aptblock->position = {0, i*3, 0};
            childObjects.push_back(move(aptblock));
        }
        auto aptblock = std::make_unique<Apartment>(this, roofType, texture);
        aptblock->position = {0, height*3, 0};
        childObjects.push_back(move(aptblock));
    }

    if(this->apartmentType == ApartmentType::corner){
        abType = static_cast<ApartmentBlockType>(rand() % 2 + 8);

        int height = rand() % 3 + 3;
        auto blockType = static_cast<ApartmentBlockType>(rand() % 3 + 10);

        auto roofType = static_cast<ApartmentBlockType>(rand() % 3 + 13);
        for (int i = 1; i < height; i++) {
            auto aptblock = std::make_unique<Apartment>(this, blockType, texture);
            aptblock->position = {0, i*3, 0};
            childObjects.push_back(move(aptblock));
        }
        auto aptblock = std::make_unique<Apartment>(this, roofType, texture);
        aptblock->position = {0, height*3, 0};
        childObjects.push_back(move(aptblock));
    }
}

Apartment::Apartment(Object* parent, ApartmentBlockType abt, const ppgso::Texture& texture) : texture(texture) {
    parentObject = parent;
    abType = abt;
    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
}


bool Apartment::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    generateModelMatrix(parentModelMatrix);

    return true;
}

void Apartment::render(Scene &scene, GLuint depthMap) {
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

    shader->setUniform("Texture", texture);

    shader->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shader->setUniformInt("ShadowMap", (int)depthMap);

    switch (abType) {
        case Door1:
            mesh_door_1->render();
            break;
        case Door2:
            mesh_door_2->render();
            break;
        case Block1:
            mesh_1->render();
            break;
        case Block2:
            mesh_2->render();
            break;
        case Block3:
            mesh_3->render();
            break;
        case Roof1:
            mesh_roof_1->render();
            break;
        case Roof2:
            mesh_roof_2->render();
            break;
        case Roof3:
            mesh_roof_3->render();
            break;
        case DoorCorner1:
            mesh_door_corner_1->render();
            break;
        case DoorCorner2:
            mesh_door_corner_2->render();
            break;
        case BlockCorner1:
            mesh_corner_1->render();
            break;
        case BlockCorner2:
            mesh_corner_2->render();
            break;
        case BlockCorner3:
            mesh_corner_3->render();
            break;
        case RoofCorner1:
            mesh_roof_corner_1->render();
            break;
        case RoofCorner2:
            mesh_roof_corner_2->render();
            break;
        case RoofCorner3:
            mesh_roof_corner_3->render();
            break;
    }
}

void Apartment::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (abType) {
        case Door1:
            mesh_door_1->render();
            break;
        case Door2:
            mesh_door_2->render();
            break;
        case Block1:
            mesh_1->render();
            break;
        case Block2:
            mesh_2->render();
            break;
        case Block3:
            mesh_3->render();
            break;
        case Roof1:
            mesh_roof_1->render();
            break;
        case Roof2:
            mesh_roof_2->render();
            break;
        case Roof3:
            mesh_roof_3->render();
            break;
        case DoorCorner1:
            mesh_door_corner_1->render();
            break;
        case DoorCorner2:
            mesh_door_corner_2->render();
            break;
        case BlockCorner1:
            mesh_corner_1->render();
            break;
        case BlockCorner2:
            mesh_corner_2->render();
            break;
        case BlockCorner3:
            mesh_corner_3->render();
            break;
        case RoofCorner1:
            mesh_roof_corner_1->render();
            break;
        case RoofCorner2:
            mesh_roof_corner_2->render();
            break;
        case RoofCorner3:
            mesh_roof_corner_3->render();
            break;
    }
}
