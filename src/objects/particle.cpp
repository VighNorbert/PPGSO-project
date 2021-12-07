#include "particle.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Particle::mesh;

std::unique_ptr<ppgso::Shader> Particle::shader;

std::unique_ptr<ppgso::Texture> Particle::fire_texture;
std::unique_ptr<ppgso::Texture> Particle::water_texture;

float FIRE_HOR = .3f;
float FIRE_VER = 1.f;
float FIRE_VER_SPEED = .2f;

float WATER_VER = .01f;
float WATER_HOR = .01f;


Particle::Particle(Object *parent, ParticleType particleType, glm::vec3 initialPosition) {
    parentObject = parent;
    this->particleType = particleType;

    if (!mesh) mesh = std::make_unique<ppgso::Mesh>("objects/sphere.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);

    if (!fire_texture) fire_texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/fire_texture.bmp"));
    if (!water_texture) water_texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/water_texture.bmp"));

    rotation = {0, 0, 0};

    if (particleType == ParticleType::Fire) {
        scale = {.3, .3, .3};
        speed = {glm::linearRand(-FIRE_HOR, FIRE_HOR), glm::linearRand(0.f, FIRE_VER), glm::linearRand(-FIRE_HOR, FIRE_HOR)};
        maxAge = 3.f;
    }
    if (particleType == ParticleType::Water) {
        scale = {.1, .1, .1};
        speed = {glm::linearRand(-WATER_HOR, WATER_HOR) , 2.0f + glm::linearRand(-WATER_VER, WATER_VER), 5.0f + glm::linearRand(-WATER_HOR, WATER_HOR)};
        maxAge = 5.f;
    }


    position = initialPosition + speed * glm::linearRand(0.f, .5f);

}

bool Particle::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    age += dt;

    if (age > maxAge) {
        return false;
    }

    if (particleType == ParticleType::Fire) {
        scale = glm::vec3{.3, .3, .3} / (1.0f + age * 1.5f ) ;
        speed.y += FIRE_VER_SPEED * dt;
    }
    if (particleType == ParticleType::Water) {
        speed.y += -1.f * dt - glm::linearRand(-WATER_HOR, WATER_HOR) * dt * 100;
        speed.x += glm::linearRand(-WATER_HOR, WATER_HOR) * dt * 100;
        speed.z += glm::linearRand(-WATER_HOR, WATER_HOR) * dt * 100;
    }

    position += speed * dt;


    generateModelMatrix(parentModelMatrix);

    return true;
}

void Particle::render(Scene &scene) {
    shader->use();

    // Set up light
    scene.renderLight(shader);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    shader->setUniform("DiffuseStrength", .2f);
    shader->setUniform("AmbientStrength", 1.f);
    shader->setUniform("SpecularStrength", .0f);

    // render mesh
    shader->setUniform("ModelMatrix", modelMatrix);

    if (particleType == ParticleType::Fire) {

        shader->setUniform("Texture", *fire_texture);
    } else {
        shader->setUniform("Texture", *water_texture);
    }
    shader->setUniform("TextureOffset", glm::vec2{age / (maxAge + .1f), 0});



    // Enable blending
    glEnable(GL_BLEND);
    // Additive blending
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);

    shader->setUniform("Transparency", 1.f);
    mesh->render();
//    if (particleType == ParticleType::Fire) {
        glDisable(GL_BLEND);
//    }
}
