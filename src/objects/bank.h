#ifndef PPGSO_BANK_H
#define PPGSO_BANK_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum BankType
{
    BankOutside = 0,
    BankInsideWalls = 1,
    BankInsideFloor = 2,
    BankInsideDoors = 3,
    BankInsideWindows = 4
};


class Bank : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_bank;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_walls;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_floor;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_doors;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_windows;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> color_shader;

    static std::unique_ptr<ppgso::Texture> texture;

    BankType bankType;

public:
    /*!
     * Create new object
     */
    Bank(Object* parent, BankType bankType);

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
    void render(Scene &scene) override;
};


#endif //PPGSO_BANK_H
