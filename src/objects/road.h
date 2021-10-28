#ifndef PPGSO_ROAD_H
#define PPGSO_ROAD_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

class Road : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;
    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Texture> texture;

    // Speed and rotational momentum
    glm::vec3 speed;
    glm::vec3 rotMomentum;


public:
    /*!
     * Create new object
     */
    Road();

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};


#endif //PPGSO_ROAD_H
