#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <utility>
//#include <src/objects/road.h>
//#include <src/objects/character.h>
//#include <src/objects/car.h>
#include <src/objects/bank.h>
//#include <src/objects/apartment.h>
//#include <src/objects/office_old.h>
//#include <src/objects/furniture.h>
#include <src/objects/lightWrapper.h>

#include "camera.h"
#include "scene.h"

const GLuint SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;
    float fow = 60.0f;
    float ratio = 1.0f;

    // Store keyboard state
    std::map<int, int> keys;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.rootObjects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 200.0f);
        camera->position = {.0f, 10.0f, -10.0f};
        camera->tilt = 20.f;
        camera->rotation = 180.f;
//        camera->keyframes = {
//                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 2.f},
//                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 5.f},
//                {Camera::getViewMatrix(20.f, 225.f, {.0f, 10.0f, -9.0f}), 0.f}
//        };

        scene.camera = move(camera);

        scene.mainlight = std::make_unique<MainLight>();
        scene.mainlight->position = glm::vec3 {1000.f, 1000.f, -1000.f};

//        Road::generateCrossroad(scene, {-40.f, 0.f});
//        Road::generateRoad(scene, 0, 44, {-30.f, 0.f});
//        Road::generateRoad(scene, 2, 40, {-50.f, 0.f});
//        Road::generateRoad(scene, 1, 15, {-40.f, 10.f});
//        Road::generateRoad(scene, 3, 5, {-40.f, -10.f});
//
//        for (int i = -1; i>=-11; i-= 2) {
//            for (int j = 0; j<= 1; j++) {
//                auto road = std::make_unique<Road>(nullptr);
//                road->roadType = RoadType::Sidewalk;
//                road->position = {i * 2.5f, 0, 12.5f + j * 5.0f};
//                scene.rootObjects.push_back(move(road));
//            }
//        }
//
//        Road::generateCrossroad(scene, {200.f, 0.f});
//        Road::generateRoad(scene, 0, 5, {210.f, 0.f});
//        Road::generateRoad(scene, 1, 22, {200.f, 10.f});
//        Road::generateRoad(scene, 3, 40, {200.f, -10.f});
//
//        auto car = std::make_unique<Car>(nullptr, CarType::Van, scene);
//        car->position = {0, 0, -2.5f};
//        car->speed = {0, 0, 0};
//        car->rotation.z = - ppgso::PI / 2;
//        scene.rootObjects.push_back(move(car));
//
//        car = std::make_unique<Car>(nullptr, CarType::PoliceCar, scene);
//        car->position = {10, 0, -2.5f};
//        car->speed = {0, 0, 0};
//        car->rotation.z = - ppgso::PI / 2;
//        scene.rootObjects.push_back(move(car));
//
//        car = std::make_unique<Car>(nullptr, CarType::MuscleCar, scene);
//        car->position = {20, 0, -2.5f};
//        car->speed = {-1, 0, 0};
//        car->rotation.z = - ppgso::PI / 2;
//        scene.rootObjects.push_back(move(car));
//
//        car = std::make_unique<Car>(nullptr, CarType::PoliceCar, scene);
//        car->position = {-25, 0, 2.5};
//        car->speed = {1, 0, 0};
//        car->rotation.z = ppgso::PI / 2;
//        scene.rootObjects.push_back(move(car));
//
//        for (int i = 5; i<=185; i+= 5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {i -2.5, 0, 10};
//            apt->rotation.z = ppgso::PI;
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        auto apt_corner = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
//        apt_corner->position = {187.5, 0, 10};
//        apt_corner->rotation.z = ppgso::PI;
//        scene.rootObjects.push_back(move(apt_corner));
//
//        for (int i = 15; i<=115; i+= 5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {190, 0, i+2.5};
//            apt->rotation.z = ppgso::PI/2;
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        for (int i = -30; i<=170; i+= 5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {i + 2.5, 0, -10};
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        for (int i = 15; i<=80; i+= 5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {-50, 0, i+2.5};
//            apt->rotation.z = ppgso::PI/2;
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        for (int i = -60; i>=-250; i-= 5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {i+2.5, 0, 10};
//            apt->rotation.z = ppgso::PI;
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        auto apt_corner_bank = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
//        apt_corner_bank->position = {-52.5, 0, 10};
//        apt_corner_bank->rotation.z = ppgso::PI;
//        scene.rootObjects.push_back(move(apt_corner_bank));

        auto bank = std::make_unique<Bank>(nullptr, BankType::BankOutside, scene);
        bank->position = {-15, 0, 10};
        bank->rotation.z = ppgso::PI;
        scene.rootObjects.push_back(move(bank));

//        auto office_old_1 = std::make_unique<OfficeOld>(nullptr);
//        office_old_1->position = {183, 0, -10};
//        scene.rootObjects.push_back(move(office_old_1));
//
//        for (int i =-30; i>=-210; i-=5) {
//            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
//            apt->position = {190, 0, i+2.5};
//            apt->rotation.z = ppgso::PI/2;
//            scene.rootObjects.push_back(move(apt));
//        }
//
//        auto office_old_2 = std::make_unique<OfficeOld>(nullptr);
//        office_old_2->position = {217, 0, 10};
//        office_old_2->rotation.z = ppgso::PI;
//        scene.rootObjects.push_back(move(office_old_2));
    }

    /*
    void initSceneBank() {
        scene.rootObjects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 200.0f);
        camera->position = {8.5f, 3.5f, 3.5f};
        camera->tilt = 20.f;
        camera->rotation = -60.f;
//        camera->keyframes = {
//                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 2.f},
//                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 5.f},
//                {Camera::getViewMatrix(20.f, 225.f, {.0f, 10.0f, -9.0f}), 0.f}
//        };

        scene.camera = move(camera);

        auto sun = new Light();
        auto sunWrapper = std::make_unique<LightWrapper>(nullptr, glm::vec3{1000.f, 1000.f, 1000.f}, sun);
        scene.lights.push_back(sun);
        scene.rootObjects.push_back(move(sunWrapper));

        auto bank = std::make_unique<Bank>(nullptr, BankType::BankInside, scene);
        scene.rootObjects.push_back(move(bank));

        auto table = std::make_unique<Furniture>(nullptr, FurnitureType::Table);
        table->position = {0,0,4};
        scene.rootObjects.push_back(move(table));

        auto chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->position = {-0.5,0,3};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->position = {0.5,0,3};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI;
        chair->position = {0,0,5};
        scene.rootObjects.push_back(move(chair));

        auto character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessSuitPushingButton);
        character->rotation.z = ppgso::PI;
        character->position = {0, 0, 0.5};
        scene.rootObjects.push_back(move(character));

        table = std::make_unique<Furniture>(nullptr, FurnitureType::Table);
        table->rotation.z = ppgso::PI/2;
        table->position = {7,0,-1};
        scene.rootObjects.push_back(move(table));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI/2;
        chair->position = {6,0,-1.5};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI/2;
        chair->position = {6,0,-0.5};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI * 3/2;
        chair->position = {8,0,-1};
        scene.rootObjects.push_back(move(chair));

        character = std::make_unique<Character>(nullptr, CharacterType::FemaleBusinessSuitStanding);
        character->rotation.y = ppgso::PI/2;
        character->rotation.x = ppgso::PI/2;
        character->position = {8, 0.13, -3};
        scene.rootObjects.push_back(move(character));

        character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessShirtStanding);
        character->rotation.y = ppgso::PI/2;
        character->rotation.x = ppgso::PI/2;
        character->rotation.z = ppgso::PI * 3/2;
        character->position = {5, 0.13, -1};
        scene.rootObjects.push_back(move(character));

        table = std::make_unique<Furniture>(nullptr, FurnitureType::Table);
        table->rotation.z = ppgso::PI/2;
        table->position = {-7,0,-1};
        scene.rootObjects.push_back(move(table));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI * 3/2;
        chair->position = {-6,0,-1.5};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI * 3/2;
        chair->position = {-6,0,-0.5};
        scene.rootObjects.push_back(move(chair));

        chair = std::make_unique<Furniture>(nullptr, FurnitureType::Chair);
        chair->rotation.z = ppgso::PI/2;
        chair->position = {-8,0,-1};
        scene.rootObjects.push_back(move(chair));

        character = std::make_unique<Character>(nullptr, CharacterType::FemaleBusinessSuitStanding);
        character->rotation.y = ppgso::PI/2;
        character->rotation.x = ppgso::PI/2;
        character->rotation.z = ppgso::PI;
        character->position = {-5.5, 0.13, -3};
        scene.rootObjects.push_back(move(character));

        character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessShirtStanding);
        character->rotation.y = ppgso::PI/2;
        character->rotation.x = ppgso::PI/2;
        character->rotation.z = ppgso::PI;
        character->position = {-5, 0.13, -1};
        scene.rootObjects.push_back(move(character));

        character = std::make_unique<Character>(nullptr, CharacterType::MaleHoodieShooting);
        character->position = {0, 0, -4};
        scene.rootObjects.push_back(move(character));

    }
*/
    GLuint depthMapFBO;
    GLuint depthMap;
    int size_x, size_y;
public:
    /*!
     * Construct custom game window
     */
    SceneWindow(std::string WINDOW_NAME, int SIZE_X, int SIZE_Y) : Window{std::move(WINDOW_NAME), SIZE_X, SIZE_Y} {
        ratio = float(SIZE_X) / float(SIZE_Y);
        size_x = SIZE_X;
        size_y = SIZE_Y;
        //hideCursor();
        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glGenFramebuffers(1, &depthMapFBO);
        glGenTextures(1, &depthMap);
        glBindTexture(GL_TEXTURE_2D, depthMap);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, nullptr);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
        float borderColor[] = { 1.0, 1.0, 1.0, 1.0 };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);

        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
        glDrawBuffer(GL_NONE);
        glReadBuffer(GL_NONE);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        initScene();
//        initSceneBank();
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        static auto time = (float) glfwGetTime();

        // Compute time delta
        float dt = animate ? (float) glfwGetTime() - time : 0;

        time = (float) glfwGetTime();

        // Update and render all objects
        scene.update(dt);

        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.renderForShadow();

        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glViewport(0, 0, size_x, size_y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Set gray background
        glClearColor(.5f, .5f, .5f, 1.0f);

        scene.render(depthMap);
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Collect key state in a map
        keys[key] = action;
        if (keys[GLFW_KEY_A]) {
            scene.camera->moveX(1);
        }
        if (keys[GLFW_KEY_D]) {
            scene.camera->moveX(-1);
        }
        if (keys[GLFW_KEY_W]) {
            scene.camera->moveZ(1);
        }
        if (keys[GLFW_KEY_S]) {
            scene.camera->moveZ(-1);
        }
        if (keys[GLFW_KEY_UP]) {
            scene.camera->moveY(1);
        }
        if (keys[GLFW_KEY_DOWN]) {
            scene.camera->moveY(-1);
        }
        if (keys[GLFW_KEY_Q]) {
            scene.camera->rotate(-1);
        }
        if (keys[GLFW_KEY_E]) {
            scene.camera->rotate(1);
        }
    }
};


#endif //PPGSO_SCENEWINDOW_HPP
