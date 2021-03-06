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

#include <shaders/blur_vert_glsl.h>
#include <shaders/blur_frag_glsl.h>
#include <shaders/bloom_frag_glsl.h>

#include "camera.h"
#include "scene.h"

const GLuint SHADOW_WIDTH = 2048, SHADOW_HEIGHT = 2048;

static float sceneTime;

class SceneWindow : public ppgso::Window {
private:
    Scene scene;
    bool animate = false;
    const float fow = 60.0f;
    float ratio = 1.0f;
    float loadTime = -1.f;
    const float overlayLength = 5.f;
    float playbackspeed = 1.f;

    GLuint hdrFBO = 0;
    GLuint colorBuffers[2];
    GLuint depthMapFBO;
    GLuint depthMap;
    GLuint rboDepth;

    GLuint pingpongFBO[2];
    GLuint pingpongBuffers[2];
    int size_x, size_y;

    GLuint attachments[2] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1 };

    bool bloom = true;
    float exposure = 1.0f;

    std::unique_ptr<ppgso::Shader> blurShader;
    std::unique_ptr<ppgso::Shader> bloomShader;

    std::unique_ptr<ppgso::Texture> overlay;

    // Store keyboard state
    std::map<int, int> keys;

    /*!
     * Reset and initialize the game scene
     * Creating unique smart pointers to objects that are stored in the scene object list
     */
    void initSceneOutside() {
        scene.scene_id = 1;
        scene.rootObjects.clear();

        scene.mainlight = std::make_unique<MainLight>();
        scene.mainlight->position = glm::vec3 {1000.f, 1000.f, -1000.f};

        // Create a camera
        auto camera = std::make_unique<Camera>(fow, ratio, 0.1f, 200.0f);
        camera->keyframes = {
                {2.f, {-15.f, 3.5f, 19.0f}, {15.f, 180.f, 0.f}},
                {2.f, {-15.f, 3.5f, 19.0f}, {15.f, 180.f, 0.f}, true, true},
                {4.f, {-17.f, 4.0f, 12.0f}, {15.f, 170.f, 0.f}, true, true},
                {1.f, {.0f, 10.0f, -9.0f}, {15.f, 225.f, 0.f}},
                {4.f, {.0f, 10.0f, -9.0f}, {15.f, 225.f, 0.f}, true, false},
                {7.f, {17.3f, 10.0f, -9.0f}, {15.f, 225.f, 0.f}},
                {5.f, {80.f, 10.0f, -9.0f}, {15.f, 225.f, 0.f}},
                {3.f, {110.0f, 2.5f, -2.5f}, {15.f, 210.f, 0.f}},
                {3.f, {135.0f, 2.5f, -2.5f}, {15.f, 210.f, 0.f}},
                {6.f, {165.0f, 2.5f, -2.5f}, {15.f, 225.f, 0.f}, false, true},
                {3.f, {220.f, 10.0f, -12.f}, {15.f, 240.f, 0.f}},
                {2.f, {220.f, 10.0f, -12.f}, {15.f, 240.f, 0.f}, true, true},
                {6.f, {210.f, 5.0f, -8.f}, {25.f, 240.f, 0.f}},
                {2.f, {210.f, 5.0f, -8.f}, {25.f, 240.f, 0.f}, true, true},
                {2.f, {210.5f, 6.0f, -11.5f}, {15.f, 300.f, 0.f}},
                {2.f, {210.5f, 6.0f, -11.5f}, {15.f, 300.f, 0.f}, true, true},
                {5.f, {209.f, 5.0f, -13.5f}, {15.f, 232.5f, 0.f}},
                {2.f, {209.f, 5.0f, -13.5f}, {15.f, 232.5f, 0.f}, true, true},
                {10.f, {206.5f, 2.f, -8.5f}, {15.f, 225.f, 0.f}},
                {2.f, {206.5f, 2.f, -8.5f}, {15.f, 225.f, 0.f}, true, true},
                {3.f, {191.f, 2.5f, -2.f}, {15.f, 225.f, 0.f}},
                {2.f, {191.f, 2.5f, -2.f}, {15.f, 225.f, 0.f}, true, true},
                {0.f, {206.5f, 2.f, -8.5f}, {15.f, 225.f, 0.f}},
        };
        scene.camera = move(camera);

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

            Apartment::generateApartments(scene, {2.5f, 0, 10}, 0, 37);
            Apartment::generateApartmentCorner(scene, {187.5, 0, 10}, 0);
            Apartment::generateApartments(scene, {190.f, 0.f, 17.5f}, 1, 9);
            Apartment::generateApartments(scene, {-50.f, 0.f, 17.5f}, 1, 4);
            Apartment::generateApartments(scene, {-72.5f, 0.f, 10.f}, 0, 4);
            Apartment::generateApartmentCorner(scene, {-52.5, 0, 10}, 0);
            Apartment::generateApartments(scene, {190.f, 0.f, -107.5f}, 1, 17);

            OfficeOld::generateOffice(scene, {183, 0, -10}, 0);
            OfficeOld::generateOffice(scene, {217, 0, 10}, ppgso::PI);

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
                    {4.f, {-15.f, 1.7f, 20.f}, {0, 0, ppgso::PI}, true, false},
                    {1.f, {-16.5f, 1.7f, 12.f}, {0, 0, ppgso::PI}},
                    {2.f, {-16.5f, 0.f, 10.f}, {0, 0, ppgso::PI}, false, true},
                    {-1.f, {-16.5f, 0.f, 5.f}, {0, 0, ppgso::PI}},
            };
            scene.rootObjects.push_back(move(character));

            auto car = std::make_unique<Car>(nullptr, CarType::MuscleCar, scene);
            car->keyframes = {
                    {7.f, {-50, 0, 2.5f}, {0, 0, ppgso::PI / 2}, false, true},
                    {2.f, {-16, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {4.f, {-16, 0, 2.5f}, {0, 0, ppgso::PI / 2}, true, false},
                    {18.f, {1.3, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {-2.f, {156.8, 0, 2.5f}, {0, 0, ppgso::PI / 2}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::Van, scene);
            car->keyframes = {
                    {29.f, {202.5, 0, 50}, {0, 0, ppgso::PI}},
                    {3.75f, {202.5, 0, 50}, {0, 0, ppgso::PI}},
                    {-2.f, {202.5, 0, 25}, {0, 0, ppgso::PI}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::Firetruck, scene);
            car->keyframes = {
                    {45.f, {197.5, 0, -100}, {0, 0, 0}},
                    {8.f, {197.5, 0, -100}, {0, 0, 0}, false, true},
                    {0.f, {197.5, 0, -15}, {0, 0, 0}}
            };
            scene.rootObjects.push_back(move(car));

            car = std::make_unique<Car>(nullptr, CarType::PoliceCar, scene);
            car->keyframes = {
                    {6.f, {-78, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {4.f, {-78, 0, 2.5f}, {0, 0, ppgso::PI / 2}, true, false},
                    {25.f, {-60, 0, 2.5f}, {0, 0, ppgso::PI / 2}},
                    {4.f, {167, 0, 2.5f}, {0, 0, ppgso::PI / 2}, false, true},
                    {0.f, {185, 0, 2.5f}, {0, 0, ppgso::PI / 2}}
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
        camera->keyframes = {
                {2.f, {0, 1.5, -2.5}, {0.f, 0.f, 0.f}},
                {3.f, {0, 1.5, -2.5}, {0.f, 0.f, 0.f}, true, true},
                {3.f, {8.5f, 3.5f, 3.5f}, {20.f, -60.f, 0.f}},
                {2.5f, {8.5f, 3.5f, 3.5f}, {20.f, -60.f, 0.f}, true, true},
                {6.5f, {0, 1.7, -3.8}, {0.f, -180.f, 0.f}},
                {3.f, {0, 1.7, -3.8}, {0.f, -180.f, 0.f}, true, true},
                {4.f, {8.5f, 3.5f, 3.5f}, {20.f, -60.f, 0.f}},
                {2.f, {8.5f, 3.5f, 3.5f}, {20.f, -60.f, 0.f}, true, true},
                {6.f, {2.5, 2.5, 2.5}, {40.f, -30.f, 0.f}},
                {2.f, {2.5, 2.5, 2.5}, {40.f, -30.f, 0.f}, true, true},
                {0.f, {0, 1.5, -2.5}, {0.f, 0.f, 0.f}}
        };

        scene.camera = move(camera);

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
                    {2.f, {3, 0.13, 4}, {ppgso::PI/2, 0, ppgso::PI}, true, true},
                    {3.f, {3, 0, 4}, {0, 0, ppgso::PI}, true, true},
                    {0.f, {0, 0, 0.5}, {0, 0, ppgso::PI}},
                    {-1, {0, 0, 0.5}, {0, 0, ppgso::PI}}
            };
            scene.rootObjects.push_back(move(character));

            character = std::make_unique<Character>(nullptr, CharacterType::MaleHoodieShooting);
            character->position = {0, 0, -4};
            character->keyframes = {
                    {37.f, {0, 0, -4}, {0, 0, 0}},
                    {3.f, {0, 0, -4}, {0, 0, 0}, true, true},
                    {0, {0, 0, -6}, {0, 0, 0}}
            };
            scene.rootObjects.push_back(move(character));
        }
    }

public:
    /*!
     * Construct custom game window
     */
    SceneWindow(std::string WINDOW_NAME, int SIZE_X, int SIZE_Y) : Window{std::move(WINDOW_NAME), SIZE_X, SIZE_Y} {
        ratio = float(SIZE_X) / float(SIZE_Y);
        size_x = SIZE_X;
        size_y = SIZE_Y;

        if (!blurShader) blurShader = std::make_unique<ppgso::Shader>(blur_vert_glsl, blur_frag_glsl);
        if (!bloomShader) bloomShader = std::make_unique<ppgso::Shader>(blur_vert_glsl, bloom_frag_glsl);

        glfwSetInputMode(window, GLFW_STICKY_KEYS, 1);

        overlay = std::make_unique<ppgso::Texture>(ppgso::image::loadBMP("overlay.bmp"));

        // Initialize OpenGL state
        // Enable Z-buffer
        glEnable(GL_DEPTH_TEST);
        glDepthFunc(GL_LEQUAL);

        // Enable polygon culling
        glEnable(GL_CULL_FACE);
        glFrontFace(GL_CCW);
        glCullFace(GL_BACK);

        glGenFramebuffers(1, &hdrFBO);
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glGenTextures(2, colorBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindTexture(GL_TEXTURE_2D, colorBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SIZE_X, SIZE_Y, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            // attach texture to framebuffer
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + i, GL_TEXTURE_2D, colorBuffers[i], 0);
        }

        glGenRenderbuffers(1, &rboDepth);
        glBindRenderbuffer(GL_RENDERBUFFER, rboDepth);
        glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, SIZE_X, SIZE_Y);
        glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, rboDepth);
        glDrawBuffers(2, attachments);

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

        glGenFramebuffers(2, pingpongFBO);
        glGenTextures(2, pingpongBuffers);
        for (unsigned int i = 0; i < 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[i]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffers[i]);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA16F, SIZE_X, SIZE_Y, 0, GL_RGBA, GL_FLOAT, nullptr);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, pingpongBuffers[i], 0);
        }

        glBindFramebuffer(GL_FRAMEBUFFER, 0);

        initSceneBank();
    }

    GLuint quadVAO = 0;
    GLuint quadVBO;
    void renderQuad()
    {
        if (quadVAO == 0)
        {
            float quadVertices[] = {
                    // positions        // texture Coords
                    -1.0f,  1.0f, 0.0f, 0.0f, 1.0f,
                    -1.0f, -1.0f, 0.0f, 0.0f, 0.0f,
                    1.0f,  1.0f, 0.0f, 1.0f, 1.0f,
                    1.0f, -1.0f, 0.0f, 1.0f, 0.0f,
            };
            // setup plane VAO
            glGenVertexArrays(1, &quadVAO);
            glGenBuffers(1, &quadVBO);
            glBindVertexArray(quadVAO);
            glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
            glBufferData(GL_ARRAY_BUFFER, sizeof(quadVertices), &quadVertices, GL_STATIC_DRAW);
            glEnableVertexAttribArray(0);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
            glEnableVertexAttribArray(1);
            glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        }
        glBindVertexArray(quadVAO);
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glBindVertexArray(0);
    }

    /*!
     * Window update implementation that will be called automatically from pollEvents
     */
    void onIdle() override {
        // Track time
        if (loadTime == -1.f) {
            loadTime = (float) glfwGetTime();
        }

        if (sceneTime == -1.f)
            sceneTime = (float) glfwGetTime() - loadTime;

        // Compute time delta
        if (sceneTime > overlayLength) {
            animate = true;
        }

        float dt = animate ? (float) glfwGetTime() - loadTime - sceneTime : 0;
        dt *= playbackspeed;

        sceneTime = (float) glfwGetTime() - loadTime;

        if (sceneTime > 46.f && scene.scene_id == 0) {
            scene.close();
            initSceneOutside();
            dt = 0;
        }

        // Update and render all objects
        scene.update(dt);

        // depth map
        glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
        glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        scene.renderForShadow();

        // hdr colors with 2 layers (first = hdr, second = light objects)
        glBindFramebuffer(GL_FRAMEBUFFER, hdrFBO);
        glViewport(0, 0, size_x, size_y);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClearColor(.695f, .822f, .987f, 1.0f);
        scene.render(depthMap);

        // blurring in 5 iterations
        bool horizontal = true, first_iteration = true;
        unsigned int amount = 5;
        blurShader->use();
        for (unsigned int i = 0; i < amount * 2; i++)
        {
            glBindFramebuffer(GL_FRAMEBUFFER, pingpongFBO[horizontal]);
            glActiveTexture(GL_TEXTURE0 + (first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]));
            blurShader->setUniformInt("image", first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]);
            blurShader->setUniformInt("horizontal", horizontal);
            glBindTexture(GL_TEXTURE_2D, first_iteration ? colorBuffers[1] : pingpongBuffers[!horizontal]);  // bind texture of other framebuffer (or scene if first iteration)
            renderQuad();
            horizontal = !horizontal;
            if (first_iteration)
                first_iteration = false;
        }

        // output the combination
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        bloomShader->use();
        bloomShader->setUniform("exposure", exposure);
        if (sceneTime < overlayLength) {
            bloomShader->setUniformInt("bloom", false);
            bloomShader->setUniform("scene", *overlay);
        } else {
            bloomShader->setUniformInt("bloom", bloom);
            glActiveTexture(GL_TEXTURE0 + colorBuffers[0]);
            glBindTexture(GL_TEXTURE_2D, colorBuffers[0]);
            bloomShader->setUniformInt("scene", colorBuffers[0]);
            glActiveTexture(GL_TEXTURE0 + pingpongBuffers[!horizontal]);
            glBindTexture(GL_TEXTURE_2D, pingpongBuffers[!horizontal]);
            bloomShader->setUniformInt("bloomBlur", pingpongBuffers[!horizontal]);
        }
        renderQuad();
    }

    void onKey(int key, int scanCode, int action, int mods) override {
        // Collect key state in a map
        keys[key] = action;

        // camera movement
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

        // camera rotation
        if (keys[GLFW_KEY_Q]) {
            scene.camera->rotate(-1);
        }
        if (keys[GLFW_KEY_E]) {
            scene.camera->rotate(1);
        }

        // debug
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
        if (keys[GLFW_KEY_L]) {
            if (bloom) {
                bloom = false;
                std::cout << "Debug: Bloom disabled" << std::endl;
            } else {
                bloom = true;
                std::cout << "Debug: Bloom enabled" << std::endl;
            }
        }
        if (keys[GLFW_KEY_K]) {
            if (scene.camera->useKeyframes) {
                scene.camera->useKeyframes = false;
                std::cout << "Debug: Camera keyframes disabled" << std::endl;
            } else {
                scene.camera->useKeyframes = true;
                std::cout << "Debug: Camera keyframes enabled" << std::endl;
            }
        }

        // scene selection
        if (keys[GLFW_KEY_1] || keys[GLFW_KEY_KP_1]) {
            std::cout << "Debug: Switching to scene 1..." << std::endl;
            scene.close();
            initSceneBank();
            loadTime = (float) glfwGetTime() - overlayLength;
            sceneTime = (float) glfwGetTime() - loadTime;
            std::cout << "Debug: Switched to scene 1" << std::endl;
        }
        if (keys[GLFW_KEY_2] || keys[GLFW_KEY_KP_2]) {
            std::cout << "Debug: Switching to scene 2..." << std::endl;
            scene.close();
            initSceneOutside();
            loadTime = (float) glfwGetTime() - overlayLength;
            sceneTime = (float) glfwGetTime() - loadTime;
            std::cout << "Debug: Switched to scene 2" << std::endl;
        }

        // playback speed
        if (keys[GLFW_KEY_SPACE]) {
            if (playbackspeed > 0.f) {
                std::cout << "Debug: Playback paused" << std::endl;
                playbackspeed = 0.f;
            } else {
                std::cout << "Debug: Normal playback" << std::endl;
                playbackspeed = 1.f;
            }
        }
        if (keys[GLFW_KEY_9] || keys[GLFW_KEY_KP_9]) {
            std::cout << "Debug: Fast forward (x5) enabled" << std::endl;
            playbackspeed = 5.f;
        }
    }
};


#endif //PPGSO_SCENEWINDOW_HPP
