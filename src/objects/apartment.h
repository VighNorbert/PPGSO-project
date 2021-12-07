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
    Roof3 = 7,
    DoorCorner1 = 8,
    DoorCorner2 = 9,
    BlockCorner1 = 10,
    BlockCorner2 = 11,
    BlockCorner3 = 12,
    RoofCorner1 = 13,
    RoofCorner2 = 14,
    RoofCorner3 = 15
};

enum ApartmentType
{
    normal = 0,
    corner = 1
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

    static std::unique_ptr<ppgso::Mesh> mesh_door_corner_1;
    static std::unique_ptr<ppgso::Mesh> mesh_door_corner_2;
    static std::unique_ptr<ppgso::Mesh> mesh_corner_1;
    static std::unique_ptr<ppgso::Mesh> mesh_corner_2;
    static std::unique_ptr<ppgso::Mesh> mesh_corner_3;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_corner_1;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_corner_2;
    static std::unique_ptr<ppgso::Mesh> mesh_roof_corner_3;

    static std::unique_ptr<ppgso::Shader> shader;
    static std::unique_ptr<ppgso::Shader> shader_shadow;

    ApartmentType apartmentType;

public:
    ppgso::Texture texture;
    ApartmentBlockType abType;
    /*!
     * Create new object
     */
    explicit Apartment(Object* parent, ApartmentType apartmentType);

    explicit Apartment(Object* parent, ApartmentBlockType abt, const ppgso::Texture& texture);

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


#endif //PPGSO_APARTMENT_H
