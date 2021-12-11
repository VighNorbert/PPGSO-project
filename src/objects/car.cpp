#include "car.h"
#include "character.h"
#include "particle.h"
#include "src/boundingcircle.h"

#include <shaders/phong_vert_glsl.h>
#include <shaders/phong_frag_glsl.h>
#include <shaders/shadow_vert_glsl.h>
#include <shaders/shadow_frag_glsl.h>
#include <src/objects/lightWrapper.h>

// Static resources
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_wheel;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_front_light;
std::unique_ptr<ppgso::Mesh> Car::mesh_muscle_car_back_light;

std::unique_ptr<ppgso::Mesh> Car::mesh_police_car;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_wheel;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_front_light;
std::unique_ptr<ppgso::Mesh> Car::mesh_police_car_back_light;

std::unique_ptr<ppgso::Mesh> Car::mesh_van;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_glass;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_wheel;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_front_light;
std::unique_ptr<ppgso::Mesh> Car::mesh_van_back_light;

std::unique_ptr<ppgso::Mesh> Car::mesh_firetruck;
std::unique_ptr<ppgso::Mesh> Car::mesh_firetruck_wheel;

std::unique_ptr<ppgso::Shader> Car::shader;
std::unique_ptr<ppgso::Shader> Car::shader_shadow;

std::unique_ptr<ppgso::Texture> Car::texture;
std::unique_ptr<ppgso::Texture> Car::texture_van;
std::unique_ptr<ppgso::Texture> Car::texture_firetruck;

Car::Car(Object* parent, CarType carType, Scene& scene) {
    parentObject = parent;
    this->carType = carType;

    if (this->carType == CarType::MuscleCar) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::MaleJacketDrivingMuscleCar);
        childObjects.push_back(move(char_driving));

        auto wheel_rf = std::make_unique<Car>(this, CarType::MuscleCarWheel, scene);
        wheel_rf->position = {-0.88974, 0.41045, 1.935};
        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::MuscleCarWheel, scene);
        wheel_rb->position = {-0.88974, 0.41045, -1.365};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::MuscleCarWheel, scene);
        wheel_lf->position = {0.88974, 0.41045, 1.935};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::MuscleCarWheel, scene);
        wheel_lb->position = {0.88974, 0.41045, -1.365};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        childObjects.push_back(std::make_unique<Car>(this, CarType::MuscleCarFrontLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::MuscleCarBackLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::MuscleCarGlass, scene));

        radius = 2.82f;
        weight = 1400.f;

        childObjects.push_back(std::make_unique<BoundingCircle>(radius));

        auto light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        auto lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {0.80443, 0.66418, 2.5}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

        light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {-0.80443, 0.66418, 2.5}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

    } else if (this->carType == CarType::PoliceCar) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::MalePoliceDrivingPoliceCar);
        childObjects.push_back(move(char_driving));

        auto char_sitting = std::make_unique<Character>(this, CharacterType::MalePoliceSittingPoliceCar);
        childObjects.push_back(move(char_sitting));

        auto wheel_rf = std::make_unique<Car>(this, CarType::PoliceCarWheel, scene);
        wheel_rf->position = {-0.83132, 0.37224, 1.6563};

        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::PoliceCarWheel, scene);
        wheel_rb->position = {-0.83132, 0.37224, -1.2337};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::PoliceCarWheel, scene);
        wheel_lf->position = {0.83132, 0.37224, 1.6563};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::PoliceCarWheel, scene);
        wheel_lb->position = {0.83132, 0.37224, -1.2337};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        auto light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        auto lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {0.56, 0.84, 2.23}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

        light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {-0.56, 0.84, 2.23}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

        childObjects.push_back(std::make_unique<Car>(this, CarType::PoliceCarFrontLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::PoliceCarBackLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::PoliceCarGlass, scene));

    } else if (this->carType == CarType::Van) {
        auto char_driving = std::make_unique<Character>(this, CharacterType::FemaleBusinessSuitDrivingVan);
        childObjects.push_back(move(char_driving));

        auto wheel_rf = std::make_unique<Car>(this, CarType::VanWheel, scene);
        wheel_rf->position = {-0.95778, 0.39926, 1.8444};
        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::VanWheel, scene);
        wheel_rb->position = {-0.95778, 0.39926, -1.4616};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::VanWheel, scene);
        wheel_lf->position = {0.95778, 0.39926, 1.8444};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::VanWheel, scene);
        wheel_lb->position = {0.95778, 0.39926, -1.4616};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));

        auto light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        auto lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {0.84, 0.83, 2.15}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

        light = new Light({1.0f, 0.975f, 0.853f}, {0, -0.13165, 1}, 10.f, 15.f, .5f, .1f, 0.05f, 35.f);
        lightWrapper = std::make_unique<LightWrapper>(this, glm::vec3 {-0.84, 0.83, 2.15}, light);
        scene.lights.push_back(light);
        childObjects.push_back(move(lightWrapper));

        radius = 2.51f;
        weight = 2400.f;

        childObjects.push_back(std::make_unique<BoundingCircle>(radius));

        childObjects.push_back(std::make_unique<Car>(this, CarType::VanFrontLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::VanBackLight, scene));
        childObjects.push_back(std::make_unique<Car>(this, CarType::VanGlass, scene));
    } else if (this->carType == CarType::Firetruck) {
        auto wheel_rf = std::make_unique<Car>(this, CarType::FiretruckWheel, scene);
        wheel_rf->position = {-0.90771, 0.400, 1.8499};
        childObjects.push_back(move(wheel_rf));

        auto wheel_rb = std::make_unique<Car>(this, CarType::FiretruckWheel, scene);
        wheel_rb->position = {-0.90771, 0.400, -1.4637};
        childObjects.push_back(move(wheel_rb));

        auto wheel_lf = std::make_unique<Car>(this, CarType::FiretruckWheel, scene);
        wheel_lf->position = {0.90771, 0.400, 1.8499};
        wheel_lf->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lf));

        auto wheel_lb = std::make_unique<Car>(this, CarType::FiretruckWheel, scene);
        wheel_lb->position = {0.90771, 0.400, -1.4637};
        wheel_lb->rotation.y = ppgso::PI;
        childObjects.push_back(move(wheel_lb));
    }

    position = {0, 0, 0};
    rotation = {0, 0, 0};
    scale = {1, 1, 1};
    speed = {0, 0, 0};
    rotMomentum = {0, 0, 0};


    if (!mesh_muscle_car) mesh_muscle_car = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car.obj");
    if (!mesh_muscle_car_glass) mesh_muscle_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_glass.obj");
    if (!mesh_muscle_car_wheel) mesh_muscle_car_wheel = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_wheel.obj");
    if (!mesh_muscle_car_front_light) mesh_muscle_car_front_light = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_frontlight.obj");
    if (!mesh_muscle_car_back_light) mesh_muscle_car_back_light = std::make_unique<ppgso::Mesh>("objects/cars/muscle_car_backlight.obj");
    if (!mesh_police_car) mesh_police_car = std::make_unique<ppgso::Mesh>("objects/cars/police_car.obj");
    if (!mesh_police_car_glass) mesh_police_car_glass = std::make_unique<ppgso::Mesh>("objects/cars/police_car_glass.obj");
    if (!mesh_police_car_wheel) mesh_police_car_wheel = std::make_unique<ppgso::Mesh>("objects/cars/police_car_wheel.obj");
    if (!mesh_police_car_front_light) mesh_police_car_front_light = std::make_unique<ppgso::Mesh>("objects/cars/police_car_frontlight.obj");
    if (!mesh_police_car_back_light) mesh_police_car_back_light = std::make_unique<ppgso::Mesh>("objects/cars/police_car_backlight.obj");
    if (!mesh_van) mesh_van = std::make_unique<ppgso::Mesh>("objects/cars/van.obj");
    if (!mesh_van_glass) mesh_van_glass = std::make_unique<ppgso::Mesh>("objects/cars/van_glass.obj");
    if (!mesh_van_wheel) mesh_van_wheel = std::make_unique<ppgso::Mesh>("objects/cars/van_wheel.obj");
    if (!mesh_van_front_light) mesh_van_front_light = std::make_unique<ppgso::Mesh>("objects/cars/van_frontlight.obj");
    if (!mesh_van_back_light) mesh_van_back_light = std::make_unique<ppgso::Mesh>("objects/cars/van_backlight.obj");
    if (!mesh_firetruck) mesh_firetruck = std::make_unique<ppgso::Mesh>("objects/cars/firetruck.obj");
    if (!mesh_firetruck_wheel) mesh_firetruck_wheel = std::make_unique<ppgso::Mesh>("objects/cars/firetruck_wheel.obj");

    if (!shader) shader = std::make_unique<ppgso::Shader>(phong_vert_glsl, phong_frag_glsl);
    if (!shader_shadow) shader_shadow = std::make_unique<ppgso::Shader>(shadow_vert_glsl, shadow_frag_glsl);

    if (!texture) texture = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_02_A.bmp"));
    if (!texture_van) texture_van = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/PolygonCity_Texture_01_A.bmp"));
    if (!texture_firetruck) texture_firetruck = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("textures/hasici.bmp"));
}

void Car::checkCollisions(Scene &scene, float dt) {
    this->collisionSpeedDelta = {0, 0, 0};
    if (keyframes.empty() || keyframesOver) {
        if (radius > 0.f) {
            for (auto &o: scene.rootObjects) {
                if (o.get() == this) continue;

                auto car = dynamic_cast<Car *>(o.get());
                if (!car) continue;
                if (o->radius == 0.f) continue;

                float dist = distance(this->position, o->position);
                float mindist = o->radius + this->radius;
                if (dist < mindist && length(o->speed) > .1f) { // collision detected
                    glm::vec3 oMomentum = o->speed * o->weight;
                    float factor = .1f + 3.f * (mindist - dist) / mindist;
                    this->collisionSpeedDelta += factor * (oMomentum / this->weight - this->speed);
                    if (this->carType == MuscleCar)
                        this->rotation.z += factor * length(speed) * .2f * dt;
                    else {
                        this->rotation.z -= factor * length(speed) * .2f * dt;
                    }
                    brakesApplied = true;
                    if (scene.showBoundingBoxes)
                        std::cout << "Debug: Collision detected" << std::endl;
                }
            }
        }
    }
}

bool Car::update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) {
    age += dt;

    if (isWheel() && parentObject != nullptr) {
        rotation.x += dt * glm::length(parentObject->speed) / getWheelDiameter();
        if (rotation.x > 2 * ppgso::PI)
            rotation.x -= float(int(rotation.x / (2 * ppgso::PI)) * 2 * ppgso::PI);
    }

    if (carType == MuscleCar) {
        if (age > 9 && not_sitting) {
            not_sitting = false;
            auto char_sitting = std::make_unique<Character>(this, CharacterType::MaleHoodieSittingMuscleCar);
            childObjects.push_front(move(char_sitting));
        }

        if (age < 65 && age > 34){
            int maxcount_per_sec = 100;
            for (int i = 1; i <= ceil(int(maxcount_per_sec * dt)); i++) {
                auto particle = std::make_unique<Particle>(this, ParticleType::Fire, glm::vec3{0.f, .5f, 2.f});
                childObjects.push_back(move(particle));
            }
        }

    }
    else if (carType == Firetruck && age < 67 && age > 45) {
        int maxcount_per_sec = 200;
        for (int i = 1; i <= ceil(int(maxcount_per_sec * dt)); i++) {
            auto particle = std::make_unique<Particle>(this, ParticleType::Water, glm::vec3{0.f, 2.f, 0.f});
            childObjects.push_back(move(particle));
        }
    }
    else if (carType == PoliceCar && age > 70 && police_alive) {
        police_alive = false;
        childObjects.pop_front();
        childObjects.pop_front();

        auto character = std::make_unique<Character>(nullptr, CharacterType::MalePoliceStanding);
        character->keyframes = {
                {2.5f, {185.f, 0.f, 4.f}, {0, 0, ppgso::PI/2}},
                {0.f, {193.f, 0.f, 4.f}, {0, 0, ppgso::PI/2}},
        };
        scene.rootObjects.push_back(move(character));

        character = std::make_unique<Character>(nullptr, CharacterType::MalePoliceStanding);
        character->keyframes = {
                {4.f, {185.f, 0.f, 1.f}, {0, 0, ppgso::PI/2}},
                {0.f, {195.f, 0.f, 1.f}, {0, 0, ppgso::PI/2}},
        };
        scene.rootObjects.push_back(move(character));
    }

    if (keyframes.empty() || keyframesOver) {
        if (length(collisionSpeedDelta) > 0.f) {
            speed += collisionSpeedDelta;
            brakingCounterSpeed = speed * .8f;
        }
        if (brakesApplied) {
            if (length(speed) > length(brakingCounterSpeed)) {
                speed -= brakingCounterSpeed * dt;
            } else {
                speed = {0.f, 0.f, 0.f};
                brakingCounterSpeed = {0, 0, 0};
                brakesApplied = false;
            }
        }
        position += speed * dt;

        generateModelMatrix(parentModelMatrix);
        return true;
    } else {
        if (keyframesUpdate(scene)) {
            return true;
        } else return false;
    }
}

void Car::render(Scene &scene, GLuint depthMap) {
    shader->use();

    // Set up light
    scene.renderLight(shader);

    // use camera
    shader->setUniform("ProjectionMatrix", scene.camera->projectionMatrix);
    shader->setUniform("ViewMatrix", scene.camera->viewMatrix);
    shader->setUniform("ViewPosition", scene.camera->position);

    if (carType == CarType::MuscleCarFrontLight || carType == CarType::MuscleCarBackLight
    || carType == CarType::PoliceCarFrontLight || carType == CarType::PoliceCarBackLight
    || carType == CarType::VanFrontLight || carType == CarType::VanBackLight) {
        shader->setUniform("DiffuseStrength", .2f);
        shader->setUniform("AmbientStrength", 2.0f);
        shader->setUniform("SpecularStrength", .3f);
    } else {
        shader->setUniform("DiffuseStrength", 1.f);
        shader->setUniform("AmbientStrength", .3f);
        shader->setUniform("SpecularStrength", 2.5f);

    }

    shader->setUniform("ModelMatrix", modelMatrix);

    if(carType == CarType::Van){
        shader->setUniform("Texture", *texture_van);
    } else if(carType == CarType::Firetruck) {
        shader->setUniform("Texture", *texture_firetruck);
    } else{
        shader->setUniform("Texture", *texture);
    }
    shader->setUniform("Transparency", 1.f);


    shader->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    glActiveTexture(GL_TEXTURE0 + depthMap);
    glBindTexture(GL_TEXTURE_2D, depthMap);
    shader->setUniformInt("ShadowMap", (int)depthMap);

    switch (this->carType) {
        case CarType::MuscleCar:
            mesh_muscle_car->render();
            break;
        case CarType::MuscleCarGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_muscle_car_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::MuscleCarWheel:
            mesh_muscle_car_wheel->render();
            break;
        case CarType::MuscleCarFrontLight:
            mesh_muscle_car_front_light->render();
            break;
        case CarType::MuscleCarBackLight:
            mesh_muscle_car_back_light->render();
            break;
        case CarType::PoliceCar:
            mesh_police_car->render();
            break;
        case CarType::PoliceCarGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_police_car_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::PoliceCarWheel:
            mesh_police_car_wheel->render();
            break;
        case CarType::PoliceCarFrontLight:
            mesh_police_car_front_light->render();
            break;
        case CarType::PoliceCarBackLight:
            mesh_police_car_back_light->render();
            break;
        case CarType::Van:
            mesh_van->render();
            break;
        case CarType::VanGlass:
            // Enable blending
            glEnable(GL_BLEND);
            // Additive blending
            glBlendFunc(GL_SRC_ALPHA, GL_ONE);

            mesh_van_glass->render();
            // Disable blending
            glDisable(GL_BLEND);
            break;
        case CarType::VanWheel:
            mesh_van_wheel->render();
            break;
        case CarType::VanFrontLight:
            mesh_van_front_light->render();
            break;
        case CarType::VanBackLight:
            mesh_van_back_light->render();
            break;
        case CarType::Firetruck:
            mesh_firetruck->render();
            break;
        case CarType::FiretruckWheel:
            mesh_firetruck_wheel->render();
            break;
    }
}

bool Car::isWheel() {
    return carType == CarType::MuscleCarWheel
        || carType == CarType::PoliceCarWheel
        || carType == CarType::VanWheel
        || carType == CarType::FiretruckWheel;
}

float Car::getWheelDiameter() {
    switch(carType) {
        case CarType::MuscleCarWheel:
            return 0.41045;
        case CarType::PoliceCarWheel:
            return 0.37224;
        case CarType::VanWheel:
            return 0.39926;
        case CarType::FiretruckWheel:
            return 0.400;
        default:
            return 0;
    }
}

void Car::renderForShadow(Scene &scene) {
    shader_shadow->use();

    shader_shadow->setUniform("LightProjectionMatrix", scene.mainlight->lightProjection);
    shader_shadow->setUniform("LightViewMatrix", scene.mainlight->getLightView(scene.camera->position));

    shader_shadow->setUniform("ModelMatrix", modelMatrix);

    switch (this->carType) {
        case CarType::MuscleCar:
            mesh_muscle_car->render();
            break;
        case CarType::MuscleCarWheel:
            mesh_muscle_car_wheel->render();
            break;
        case CarType::MuscleCarFrontLight:
            mesh_muscle_car_front_light->render();
            break;
        case CarType::MuscleCarBackLight:
            mesh_muscle_car_back_light->render();
            break;
        case CarType::PoliceCar:
            mesh_police_car->render();
            break;
        case CarType::PoliceCarWheel:
            mesh_police_car_wheel->render();
            break;
        case CarType::PoliceCarFrontLight:
            mesh_police_car_front_light->render();
            break;
        case CarType::PoliceCarBackLight:
            mesh_police_car_back_light->render();
            break;
        case CarType::Van:
            mesh_van->render();
            break;
        case CarType::VanWheel:
            mesh_van_wheel->render();
            break;
        case CarType::VanFrontLight:
            mesh_van_front_light->render();
            break;
        case CarType::VanBackLight:
            mesh_van_back_light->render();
            break;
        case CarType::Firetruck:
            mesh_firetruck->render();
            break;
        case CarType::FiretruckWheel:
            mesh_firetruck_wheel->render();
            break;
        default:
            break;
    }
}