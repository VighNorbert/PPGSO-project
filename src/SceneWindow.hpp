#ifndef PPGSO_SCENEWINDOW_HPP
#define PPGSO_SCENEWINDOW_HPP

#include <utility>
#include <src/objects/road.h>
#include <src/objects/character.h>
#include <src/objects/car.h>
#include <src/objects/bank.h>
#include <src/objects/apartment.h>
#include <src/objects/office_old.h>
#include <src/objects/furniture.h>
#include <src/objects/lightWrapper.h>

#include "camera.h"
#include "scene.h"

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

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
        scene.scene_id = 1;
        scene.rootObjects.clear();

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 200.0f);
        camera->position = {0.0f, 10.0f, -10.0f};
        camera->tilt = 20.f;
        camera->rotation = 180.f;
        camera->keyframes = {
                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 2.f},
                {Camera::getViewMatrix(15.f, 177.f, {-17.f, 5.0f, 12.0f}), 5.f},
                {Camera::getViewMatrix(15.f, 225.f, {.0f, 10.0f, -9.0f}), 2.f},
                {Camera::getViewMatrix(15.f, 225.f, {.0f, 10.0f, -9.0f}), 9.f},
                {Camera::getViewMatrix(15.f, 225.f, {80.f, 10.0f, -9.0f}), 5.f},
                {Camera::getViewMatrix(15.f, 210.f, {110.0f, 2.5f, -2.5f}), 3.f},
                {Camera::getViewMatrix(15.f, 210.f, {135.0f, 2.5f, -2.5f}), 3.f},
                {Camera::getViewMatrix(15.f, 225.f, {165.0f, 2.5f, -2.5f}), 3.f},
                {Camera::getViewMatrix(15.f, 225.f, {220.f, 10.0f, -12.f}), 6.f},
                {Camera::getViewMatrix(15.f, 225.f, {220.f, 10.0f, -12.f}), 2.f},
                {Camera::getViewMatrix(15.f, 300.f, {220.f, 10.0f, -12.f}), 5.f},
                {Camera::getViewMatrix(15.f, 300.f, {220.f, 10.0f, -12.f}), 2.f},
                {Camera::getViewMatrix(15.f, 240.f, {220.f, 10.0f, -12.f}), 0.f},
        };
        scene.camera = move(camera);

        scene.mainlight = std::make_unique<MainLight>();
        scene.mainlight->position = glm::vec3 {1000.f, 1000.f, -1000.f};

        // static objects
        {
            Road::generateCrossroad(scene, {-40.f, 0.f});
            Road::generateRoad(scene, 0, 44, {-30.f, 0.f});
            Road::generateRoad(scene, 2, 5, {-50.f, 0.f});
            Road::generateRoad(scene, 1, 5, {-40.f, 10.f});
            Road::generateRoad(scene, 3, 5, {-40.f, -10.f});

            for (int i = -1; i >= -11; i -= 2) {
                for (int j = 0; j <= 2; j++) {
                    auto road = std::make_unique<Road>(nullptr);
                    road->roadType = RoadType::Sidewalk;
                    road->position = {i * 2.5f, 0, 12.5f + j * 5.0f};
                    scene.rootObjects.push_back(move(road));
                }
            }

            Road::generateCrossroad(scene, {200.f, 0.f});
            Road::generateRoad(scene, 0, 5, {210.f, 0.f});
            Road::generateRoad(scene, 1, 10, {200.f, 10.f});
            Road::generateRoad(scene, 3, 20, {200.f, -10.f});

            for (int i = 5; i <= 185; i += 5) {
                auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
                apt->position = {i - 2.5, 0, 10};
                apt->rotation.z = ppgso::PI;
                scene.rootObjects.push_back(move(apt));
            }

            auto apt_corner = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
            apt_corner->position = {187.5, 0, 10};
            apt_corner->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(apt_corner));

            for (int i = 15; i <= 55; i += 5) {
                auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
                apt->position = {190, 0, i + 2.5};
                apt->rotation.z = ppgso::PI / 2;
                scene.rootObjects.push_back(move(apt));
            }


            for (int i = 15; i <= 30; i += 5) {
                auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
                apt->position = {-50, 0, i + 2.5};
                apt->rotation.z = ppgso::PI / 2;
                scene.rootObjects.push_back(move(apt));
            }

            for (int i = -60; i >= -75; i -= 5) {
                auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
                apt->position = {i + 2.5, 0, 10};
                apt->rotation.z = ppgso::PI;
                scene.rootObjects.push_back(move(apt));
            }

            auto apt_corner_bank = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
            apt_corner_bank->position = {-52.5, 0, 10};
            apt_corner_bank->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(apt_corner_bank));

            auto office_old_1 = std::make_unique<OfficeOld>(nullptr);
            office_old_1->position = {183, 0, -10};
            scene.rootObjects.push_back(move(office_old_1));

            for (int i = -30; i >= -115; i -= 5) {
                auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
                apt->position = {190, 0, i + 2.5};
                apt->rotation.z = ppgso::PI / 2;
                scene.rootObjects.push_back(move(apt));
            }

            auto office_old_2 = std::make_unique<OfficeOld>(nullptr);
            office_old_2->position = {217, 0, 10};
            office_old_2->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(office_old_2));

            auto bank = std::make_unique<Bank>(nullptr, BankType::BankOutside, scene);
            bank->position = {-15, 0, 10};
            bank->rotation.z = ppgso::PI;
            scene.rootObjects.push_back(move(bank));
        }

        // dynamic objects
        {
            auto character = std::make_unique<Character>(nullptr, CharacterType::MaleHoodieStanding);
            character->keyframes = {
                    {2.f, {-15.f, 1.7f, 20.f}, {0, 0, ppgso::PI}},
                    {4.f, {-15.f, 1.7f, 20.f}, {0, 0, ppgso::PI}},
                    {1.f, {-16.5f, 1.7f, 12.f}, {0, 0, ppgso::PI}},
                    {2.f, {-16.5f, 0.f, 10.f}, {0, 0, ppgso::PI}},
                    {-1.f, {-16.5f, 0.f, 5.f}, {0, 0, ppgso::PI}},
            };
            scene.rootObjects.push_back(move(character));

            auto car = std::make_unique<Car>(nullptr, CarType::MuscleCar, scene);
            car->keyframes = {
                    {7.f, {-50, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {2.f, {-16, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {20.f, {-16, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {-2.f, {156.8, 0, 2.5f}, {0, 0, ppgso::PI / 2}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::PoliceCar, scene);
            car->keyframes = {
                    {8.f, {-60, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {27.f, {-60, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {0.f, {185, 0, 2.5f}, {0, 0, ppgso::PI / 2}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::Van, scene);
            car->keyframes = {
                    {27.f, {202.5, 0, 50}, {0, 0, ppgso::PI}},
                    {3.75f, {202.5, 0, 50}, {0, 0, ppgso::PI}},
                    {-2.f, {202.5, 0, 25}, {0, 0, ppgso::PI}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::Firetruck, scene);
            car->keyframes = {
                    {37.f, {197.5, 0, -100}, {0, 0, 0}},
                    {7.f, {197.5, 0, -100}, {0, 0, 0}},
                    {0.f, {197.5, 0, -10}, {0, 0, 0}}
            };
            scene.rootObjects.push_back(move(car));
        }
    }


    void initSceneBank() {
        scene.scene_id = 0;
        scene.rootObjects.clear();

        scene.mainlight = std::make_unique<MainLight>();
        scene.mainlight->position = glm::vec3 {1000.f, 1000.f, -1000.f};

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 200.0f);
//        camera->position = {8.5f, 3.5f, 3.5f};
//        camera->tilt = 20.f;
//        camera->rotation = -60.f;
        camera->keyframes = {
                {Camera::getViewMatrix(0.f, 0.f, {0, 1.5, -2.5}), 2.f},
                {Camera::getViewMatrix(0.f, 0.f, {0, 1.5, -2.5}), 3.f},
                {Camera::getViewMatrix(20.f, -60.f, {8.5f, 3.5f, 3.5f}), 15.f},
                {Camera::getViewMatrix(20.f, -60.f, {8.5f, 3.5f, 3.5f}), 3.f},
                {Camera::getViewMatrix(40.f, -30.f, {2.5, 2.5, 2.5}), 5.f},
                {Camera::getViewMatrix(40.f, -30.f, {2.5, 2.5, 2.5}), 2.f},
                {Camera::getViewMatrix(0.f, 0.f, {0, 1.5, -2.5}), 0.f},
        };

        scene.camera = move(camera);

//        auto sun = new Light();
//        auto sunWrapper = std::make_unique<LightWrapper>(nullptr, glm::vec3{1000.f, 1000.f, 1000.f}, sun);
//        scene.lights.push_back(sun);
//        scene.rootObjects.push_back(move(sunWrapper));

//        auto character = std::make_unique<Character>(nullptr, CharacterType::MaleHoodieStanding);
//        character->rotation.z = ppgso::PI;
//        character->position = {0, 0, 0.5};
//        scene.rootObjects.push_back(move(character));

        // static objects
        {
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

            auto character = std::make_unique<Character>(nullptr, CharacterType::FemaleBusinessSuitStanding);
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
        }

        // dynamic objects
        {
            auto character = std::make_unique<Character>(nullptr, CharacterType::MaleBusinessSuitStanding);
            character->keyframes = {
                    {6.f, {3, 0.13, 4}, {ppgso::PI/2, 0, ppgso::PI}},
                    {2.f, {3, 0.13, 4}, {ppgso::PI/2, 0, ppgso::PI}},
                    {2.f, {3, 0, 4}, {0, 0, ppgso::PI}},
                    {0.f, {0, 0, 0.5}, {0, 0, ppgso::PI}},
                    {-1, {0, 0, 0.5}, {0, 0, ppgso::PI}}
            };
            scene.rootObjects.push_back(move(character));

            character = std::make_unique<Character>(nullptr, CharacterType::MaleHoodieShooting);
            character->position = {0, 0, -4};
            character->keyframes = {
                    {32.f, {0, 0, -4}, {0, 0, 0}},
                    {3.f, {0, 0, -4}, {0, 0, 0}},
                    {0, {0, 0, -6}, {0, 0, 0}}
            };
            scene.rootObjects.push_back(move(character));
        }
    }

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

//        initScene();
        initSceneBank();
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
        glClearColor(.695f, .822f, .987f, 1.0f);

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
        if (keys[GLFW_KEY_B]) {
            if (scene.showBoundingBoxes) {
                scene.showBoundingBoxes = false;
                std::cout << "Debug: Bounding boxes disabled" << std::endl;
            } else {
                scene.showBoundingBoxes = true;
                std::cout << "Debug: Bounding boxes enabled" << std::endl;
            }
        }
        if (keys[GLFW_KEY_F]) {
            if (scene.showFPS) {
                scene.showFPS = false;
                std::cout << "Debug: FPS disabled" << std::endl;
            } else {
                scene.showFPS = true;
                std::cout << "Debug: FPS enabled" << std::endl;
            }
        }
        if (keys[GLFW_KEY_C]) {
            if (scene.camera->debugEnabled) {
                scene.camera->debugEnabled = false;
                std::cout << "Debug: Camera position disabled" << std::endl;
            } else {
                scene.camera->debugEnabled = true;
                std::cout << "Debug: Camera position enabled" << std::endl;
                scene.camera->debug();
            }
        }
    }
};


#endif //PPGSO_SCENEWINDOW_HPP
