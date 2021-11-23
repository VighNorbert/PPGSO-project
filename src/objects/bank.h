#ifndef PPGSO_BANK_H
#define PPGSO_BANK_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"


class Bank : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

public:
    /*!
     * Create new object
     */
    Bank(Object* parent);

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;
};


#endif //PPGSO_BANK_H
