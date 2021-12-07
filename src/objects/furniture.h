#ifndef PPGSO_FURNITURE_H
#define PPGSO_FURNITURE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum FurnitureType
{
    Table = 0,
    Chair = 1
};


class Furniture : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_table;
    static std::unique_ptr<ppgso::Mesh> mesh_chair;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    static std::unique_ptr<ppgso::Texture> texture;

    FurnitureType furnitureType;

public:
    /*!
     * Create new object
     */
    Furniture(Object* parent, FurnitureType furnitureType);

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
};



#endif //PPGSO_FURNITURE_H
