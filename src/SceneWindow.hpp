#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <shaders/diffuse_vert_glsl.h>
#include <shaders/diffuse_frag_glsl.h>

#include <utility>
#include <src/objects/road.h>
#include <src/objects/road_crossing.h>

#include "camera.h"
#include "scene.h"
#include "src/objects/Footman.h"

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
        scene.objects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 100.0f);
        camera->position = {.0f, 1.0f, -5.0f};
        scene.camera = move(camera);

        // Add space background
        scene.objects.push_back(std::make_unique<Footman>());

        // Add generator to scene
//        auto generator = std::make_unique<Generator>();
//        generator->position.y = 10.0f;

        auto roadc = std::make_unique<RoadCrossing>();
        roadc->position.z = 0 * 5.0f;
        roadc->position.x = -2.5f;
        scene.objects.push_back(move(roadc));

        for (int x = 1; x < 5; x++)
        {
            auto road = std::make_unique<Road>();
            road->position.z = x * 5.0f;
            road->position.x = -2.5f;
            scene.objects.push_back(move(road));
        }
        roadc = std::make_unique<RoadCrossing>();
        roadc->position.z = 0 * 5.0f;
        roadc->position.x = 2.5f;
        roadc->rotation.z = ppgso::PI;
        scene.objects.push_back(move(roadc));
        for (int x = 1; x < 5; x++)
        {
            auto road = std::make_unique<Road>();
            road->position.z = x * 5.0f;
            road->position.x = 2.5f;
            road->rotation.z = ppgso::PI;
            scene.objects.push_back(move(road));
        }

        // Add player to the scene
//        auto player = std::make_unique<Player>();
//        player->position.y = -6;
//        scene.objects.push_back(move(player));
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
