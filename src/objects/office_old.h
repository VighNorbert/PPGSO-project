#ifndef PPGSO_OFFICE_OLD_H
#define PPGSO_OFFICE_OLD_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"
#include "road.h"


class OfficeOld : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create new object
     */
    OfficeOld(Object* parent);

    void checkCollisions(Scene &scene, float dt) override {};

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene, GLuint depthMap) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void renderForShadow(Scene &scene) override;

    static void generateOffice(Scene &scene, glm::vec3 position, float rotation) {
        auto office = std::make_unique<OfficeOld>(nullptr);
        office->position = position;
        office->rotation.z = rotation;

        for (int i = 0; i < 3; i ++) {
            for (int j = -5; j <= 5; j += 5) {
                auto road = std::make_unique<Road>(nullptr);
                road->roadType = RoadType::Sidewalk;
                road->position = {j - .5f, 0, -2.5f - i * 5.f};
                office->childObjects.push_back(move(road));
            }
        }
        scene.rootObjects.push_back(move(office));
    }
};


#endif //PPGSO_OFFICE_OLD_H
