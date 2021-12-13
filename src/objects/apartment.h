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

    void checkCollisions(Scene &scene, float dt) override {};

    /*!
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentRotation) override;

    /*!
     * @param scene Scene to render in
     */
    void render(Scene &scene, GLuint depthMap) override;

    /*!
     * @param scene Scene to render in
     */
    void renderForShadow(Scene &scene) override;

    static void generateApartments(Scene &scene, glm::vec3 firstPosition, int orientation, int count) {
        glm::vec3 positionShift = {5, 0, 0};
        float rotation = ppgso::PI;

        if (orientation == 0) {
            positionShift = {5, 0, 0};
            rotation = ppgso::PI;
        }
        else if (orientation == 1) {
            positionShift = {0, 0, 5};
            rotation = ppgso::PI / 2;
        }
        else if (orientation == 2) {
            positionShift = {-5, 0, 0};
            rotation = 0;
        }
        else if (orientation == 3) {
            positionShift = {0, 0, -5};
            rotation = - ppgso::PI / 2;
        }

        for (int i = 0; i < count; i++) {
            auto apt = std::make_unique<Apartment>(nullptr, ApartmentType::normal);
            apt->position = firstPosition + float(i) * positionShift;
            apt->rotation.z = rotation;
            scene.rootObjects.push_back(move(apt));
        }
    }

    static void generateApartmentCorner(Scene &scene, glm::vec3 position, int orientation) {
        float rotation = ppgso::PI;

        if (orientation == 0) {
            rotation = ppgso::PI;
        }
        else if (orientation == 1) {
            rotation = ppgso::PI / 2;
        }
        else if (orientation == 2) {
            rotation = 0;
        }
        else if (orientation == 3) {
            rotation = - ppgso::PI / 2;
        }

        auto apt_corner = std::make_unique<Apartment>(nullptr, ApartmentType::corner);
        apt_corner->position = position;
        apt_corner->rotation.z = rotation;
        scene.rootObjects.push_back(move(apt_corner));
    }
};


#endif //PPGSO_APARTMENT_H
