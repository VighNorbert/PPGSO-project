#ifndef PPGSO_BANK_H
#define PPGSO_BANK_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum BankType
{
    BankOutside = 0,
    BankInside = 1,
    BankInsideGlass = 2,
    BankInsideAlarm = 3
};


class Bank : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_bank;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_glass;
    static std::unique_ptr<ppgso::Mesh> mesh_bank_inside_alarm;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

    BankType bankType;

public:
    /*!
     * Create new object
     */
    Bank(Object* parent, BankType bankType, Scene& scene);

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
