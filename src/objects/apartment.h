#ifndef PPGSO_APARTMENT_H
#define PPGSO_APARTMENT_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum ApartmentBlockType
{
    Door1 = 0,
    Door2 = 1,
    Block1 = 2,
    Block2 = 3,
    Block3 = 4,
    Roof1 = 5,
    Roof2 = 6,
    Roof3 = 7
};

class Apartment : public Object {
private:

// Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_door_1;
    static std::unique_ptr<ppgso::Mesh> mesh_door_2;
    static std::unique_ptr<ppgso::Mesh> mesh_1;
    static std::unique_ptr<ppgso::Mesh> mesh_2;
    static std::unique_ptr<ppgso::Mesh> mesh_3;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_1;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_2;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_3;

    static std::unique_ptr<ppgso::Shader> shader;


public:
    ppgso::Texture texture;
    ApartmentBlockType abType;
    /*!
     * Create new object
     */
    Apartment();

    explicit Apartment(ApartmentBlockType abt, const ppgso::Texture& texture);

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


#endif //PPGSO_APARTMENT_H
