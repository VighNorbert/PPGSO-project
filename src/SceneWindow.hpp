#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <utility>
#include <src/objects/road.h>
#include <src/objects/character.h>
#include <src/objects/car.h>
#include <src/objects/bank.h>
#include <src/objects/apartment.h>

#include "camera.h"
#include "scene.h"

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
        scene.camera = move(camera);

        Road::generateCrossroad(scene, {-40.f, 0.f});
        Road::generateRoad(scene, 0, 44, {-30.f, 0.f});
        Road::generateRoad(scene, 2, 40, {-50.f, 0.f});
        Road::generateRoad(scene, 1, 22, {-40.f, 10.f});
        Road::generateRoad(scene, 3, 22, {-40.f, -10.f});

        Road::generateCrossroad(scene, {200.f, 0.f});
        Road::generateRoad(scene, 0, 40, {210.f, 0.f});
        Road::generateRoad(scene, 1, 22, {200.f, 10.f});
        Road::generateRoad(scene, 3, 22, {200.f, -10.f});

        auto car = std::make_unique<Car>(nullptr, CarType::Van);
        car->position = {0, 0, -2.5f};
        car->speed = {0, 0, 0};
        car->rotation.z = - ppgso::PI / 2;
        scene.rootObjects.push_back(move(car));

        car = std::make_unique<Car>(nullptr, CarType::PoliceCar);
        car->position = {10, 0, -2.5f};
        car->speed = {0, 0, 0};
        car->rotation.z = - ppgso::PI / 2;
        scene.rootObjects.push_back(move(car));

        car = std::make_unique<Car>(nullptr, CarType::MuscleCar);
        car->position = {20, 0, -2.5f};
        car->speed = {0, 0, 0};
        car->rotation.z = - ppgso::PI / 2;
        scene.rootObjects.push_back(move(car));
//
        car = std::make_unique<Car>(nullptr, CarType::PoliceCar);
        car->position = {-25, 0, 2.5};
        car->speed = {1, 0, 0};
        car->rotation.z = ppgso::PI / 2;
        scene.rootObjects.push_back(move(car));

        for (int i = 5; i<=150; i+= 5) {
            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
            apt->position = {i, 0, 10};
            apt->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(apt));
        }

        for (int i = -30; i<=150; i+= 5) {
            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
            apt->position = {i + 2.5, 0, -10};
            scene.rootObjects.push_back(move(apt));
        }

        for (int i = 15; i<=80; i+= 5) {
            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
            apt->position = {-50, 0, i+2.5};
            apt->rotation.z = ppgso::PI/2;
            scene.rootObjects.push_back(move(apt));
        }

        for (int i = -60; i>=-250; i-= 5) {
            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
            apt->position = {i +2.5, 0, 10};
            apt->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(apt));
        }

        auto apt_corner_bank = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
        apt_corner_bank->position = {-52.5, 0, 10};
        apt_corner_bank->rotation.z = ppgso::PI;
        scene.rootObjects.push_back(move(apt_corner_bank));

        auto bank = std::make_unique<Bank>(nullptr);
        bank->position = {-15, 0, 10};
        bank->rotation.z = ppgso::PI;
        scene.rootObjects.push_back(move(bank));
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow(std::string WINDOW_NAME, int SIZE_X, int SIZE_Y) : Window{std::move(WINDOW_NAME), SIZE_X, SIZE_Y} {
        ratio = float(SIZE_X) / float(SIZE_Y);
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

        initScene();
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

        // Set gray background
        glClearColor(.5f, .5f, .5f, 0);
        // Clear depth and color buffers
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Update and render all objects
        scene.update(dt);
        scene.render();
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
