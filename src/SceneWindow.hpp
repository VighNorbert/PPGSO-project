#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <utility>
#include <src/objects/road.h>
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

        Road::generateCrossroad(scene, {200.f, 0.f});
        Road::generateRoad(scene, 0, 44, {-30.f, 0.f});

//        auto car = std::make_unique<Car>(CarType::MuscleCar);
//        car->position = {2.5f, 0, 25};
//        car->speed = {0, 0, -5};
//        car->rotation.z = ppgso::PI;
//        scene.rootObjects.push_back(move(car));
//
//        car = std::make_unique<Car>(CarType::PoliceCar);
//        car->position = {-2.5f, 0, -10};
//        car->speed = {0, 0, 5};
//        scene.rootObjects.push_back(move(car));

        for (int i = 30; i<=100; i+= 5) {
            auto apt = std::make_unique<Apartment>();
            apt->position = {i, 0, 10};
            apt->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(apt));
        }

        auto bank = std::make_unique<Bank>();
        bank->position = {0, 0, 10};
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
