#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <utility>
#include <src/objects/road.h>
#include <src/objects/car.h>

#include "camera.h"
#include "scene.h"

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = true;
    float fow = 60.0f;
    float ratio = 1.0f;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initScene() {
        scene.rootObjects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 100.0f);
        camera->position = {.0f, 5.0f, -10.0f};
        camera->tilt = 20.f;
        camera->rotation = 180.f;
        scene.camera = move(camera);

        Road::generateCrossroad(scene, {0.f, 0.f});
        Road::generateRoad(scene, 0, 5, {10.f, 0.f});
        Road::generateRoad(scene, 1, 25, {0.f, 10.f});
        Road::generateRoad(scene, 2, 5, {-10.f, 0.f});
        Road::generateRoad(scene, 3, 5, {0.f, -10.f});

        auto car = std::make_unique<Car>(CarType::MuscleCar);
        car->position = {2.5f, 0, 25};
        car->speed = {0, 0, -5};
        car->rotation.z = ppgso::PI;
        scene.rootObjects.push_back(move(car));

        car = std::make_unique<Car>(CarType::PoliceCar);
        car->position = {-2.5f, 0, -10};
        car->speed = {0, 0, 5};
        scene.rootObjects.push_back(move(car));

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
};


#endif //PPGSO_SCENEWINDOW_HPP
