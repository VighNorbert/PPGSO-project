#ifndef _PPGSO_SCENE_H
#define _PPGSO_SCENE_H

#include <memory>
#include <map>
#include <list>

#include "object.h"
#include "camera.h"
#include "light.h"
#include "mainlight.h"

/*
 * Scene is an object that will aggregate all scene related data
 * Objects are stored in a list of objects
 * Keyboard and Mouse states are stored in a map and struct
 */
class Scene {
public:
    /*!
     * Update all objects in the scene
     * @param time
     */
    void update(float time);

    /*!
     * Render all objects in the scene
     */
    void render(GLuint depthMap);

    /*!
     * Render all objects in the scene
     */
    void renderForShadow();

    /*!
     * Destroy all objects in the scene
     */
    void close();

    /*!
     * @param scene
     * @param shader
     */
    void renderLight(std::unique_ptr<ppgso::Shader> &shader, bool onlyMain = true);

    // Camera object
    std::unique_ptr<Camera> camera;

    // All objects to be rendered in scene
    std::list< std::unique_ptr<Object> > rootObjects;

    std::list<Light *> lights = {};

    std::unique_ptr<MainLight> mainlight = nullptr;

    bool showBoundingBoxes = false;
    bool showFPS = false;
    float lastFPSOutputTime = 0.f;
    short scene_id;
};

#endif // _PPGSO_SCENE_H
