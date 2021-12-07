#include "road.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Road::mesh_road;
std::unique_ptr<ppgso::Mesh> Road::mesh_crossing;
std::unique_ptr<ppgso::Mesh> Road::mesh_crossroad;
std::unique_ptr<ppgso::Mesh> Road::mesh_sidewalk;
std::unique_ptr<ppgso::Mesh> Road::mesh_sidewalk_straight;
std::unique_ptr<ppgso::Mesh> Road::mesh_sidewalk_corner;
std::unique_ptr<ppgso::Mesh> Road::mesh_sidewalk_gutter;

std::unique_ptr<ppgso::Shader> Road::shader;
std::unique_ptr<ppgso::Shader> Road::shader_shadow;

std::unique_ptr<ppgso::Texture> Road::texture_side;
std::unique_ptr<ppgso::Texture> Road::texture_center;
std::unique_ptr<ppgso::Texture> Road::texture_main;

Road::Road(Object* parent) {
    parentObject = parent;

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};

    if (!mesh_road) mesh_road = std::make_unique<ppgso::Mesh>("objects/roads/road.obj");
    if (!mesh_crossing) mesh_crossing = std::make_unique<ppgso::Mesh>("objects/roads/road_crossing.obj");
    if (!mesh_crossroad) mesh_crossroad = std::make_unique<ppgso::Mesh>("objects/roads/road_crossroad.obj");
    if (!mesh_sidewalk) mesh_sidewalk = std::make_unique<ppgso::Mesh>("objects/roads/sidewalk.obj");
    if (!mesh_sidewalk_straight) mesh_sidewalk_straight = std::make_unique<ppgso::Mesh>("objects/roads/sidewalk_straight.obj");
    if (!mesh_sidewalk_corner) mesh_sidewalk_corner = std::make_unique<ppgso::Mesh>("objects/roads/sidewalk_corner.obj");
    if (!mesh_sidewalk_gutter) mesh_sidewalk_gutter = std::make_unique<ppgso::Mesh>("objects/roads/sidewalk_gutter.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!texture_side) texture_side = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Road_Side.bmp"));
    if (!texture_center) texture_center = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Road_Main.bmp"));
    if (!texture_main) texture_main = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture.bmp"));
}


bool Road::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    position += speed * dt;

    // Rotate the object
    rotation += rotMomentum * dt;

    generateModelMatrix(parentModelMatrix);

    return true;
}

void Road::render(Scene &scene, GLuint depthMap) {
    shader->use();

    // Set up light
//    shader->setUniform("LightPosition", scene.lightPosition);
    scene.renderLight(shader);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    shader->setUniform("DiffuseStrength", 1.f);
    shader->setUniform("AmbientStrength", .2f);
    shader->setUniform("SpecularStrength", .1f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    switch (roadType) {
        case StraightRoad:
            shader->setUniform("Texture", *texture_side);
            mesh_road->render();
            break;
        case RoadCrossing:
            shader->setUniform("Texture", *texture_side);
            mesh_crossing->render();
            break;
        case Crossroad:
            shader->setUniform("Texture", *texture_center);
            mesh_crossroad->render();
            break;
        case Sidewalk:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk->render();
            break;
        case Sidewalk_Straight:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_straight->render();
            break;
        case Sidewalk_Corner:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_corner->render();
            break;
        case Sidewalk_Gutter:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_gutter->render();
            break;
    }
}

void Road::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (roadType) {
        case StraightRoad:
            shader->setUniform("Texture", *texture_side);
            mesh_road->render();
            break;
        case RoadCrossing:
            shader->setUniform("Texture", *texture_side);
            mesh_crossing->render();
            break;
        case Crossroad:
            shader->setUniform("Texture", *texture_center);
            mesh_crossroad->render();
            break;
        case Sidewalk:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk->render();
            break;
        case Sidewalk_Straight:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_straight->render();
            break;
        case Sidewalk_Corner:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_corner->render();
            break;
        case Sidewalk_Gutter:
            shader->setUniform("Texture", *texture_main);
            mesh_sidewalk_gutter->render();
            break;
    }
}
