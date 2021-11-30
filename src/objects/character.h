#ifndef PPGSO_CHARACTER_H
#define PPGSO_CHARACTER_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum CharacterType
{
    FemaleBusinessSuitDrivingVan = 0,
    FemaleBusinessSuitStanding = 1,
    MaleBusinessShirtStanding = 2,
    MaleBusinessSuitStanding = 3,
    MaleHoodieShooting = 4,
    MaleHoodieSitting = 5,
    MaleHoodieSittingMuscleCar = 6,
    MaleHoodieStanding = 7,
    MaleJacketDriving = 8,
    MaleJacketDrivingMuscleCar = 9,
    MaleJacketStanding = 10,
    MalePoliceDriving = 11,
    MalePoliceDrivingPoliceCar = 12,
    MalePoliceSitting = 13,
    MalePoliceSittingPoliceCar = 14,
    MalePoliceStanding = 15,
    MaleBusinessSuitPushingButton = 16
};

class Character : public Object {
private:

    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_female_business_suit_driving_van;
    static std::unique_ptr<ppgso::Mesh> mesh_female_business_suit_standing;
    static std::unique_ptr<ppgso::Mesh> mesh_male_business_shirt_standing;
    static std::unique_ptr<ppgso::Mesh> mesh_male_business_suit_standing;
    static std::unique_ptr<ppgso::Mesh> mesh_male_business_suit_pushing_button;
    static std::unique_ptr<ppgso::Mesh> mesh_male_hoodie_shooting;
    static std::unique_ptr<ppgso::Mesh> mesh_male_hoodie_sitting;
    static std::unique_ptr<ppgso::Mesh> mesh_male_hoodie_sitting_muscle_car;
    static std::unique_ptr<ppgso::Mesh> mesh_male_hoodie_standing;
    static std::unique_ptr<ppgso::Mesh> mesh_male_jacket_driving;
    static std::unique_ptr<ppgso::Mesh> mesh_male_jacket_driving_muscle_car;
    static std::unique_ptr<ppgso::Mesh> mesh_male_jacket_standing;
    static std::unique_ptr<ppgso::Mesh> mesh_male_police_driving;
    static std::unique_ptr<ppgso::Mesh> mesh_male_police_driving_police_car;
    static std::unique_ptr<ppgso::Mesh> mesh_male_police_sitting;
    static std::unique_ptr<ppgso::Mesh> mesh_male_police_sitting_police_car;
    static std::unique_ptr<ppgso::Mesh> mesh_male_police_standing;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture;

    CharacterType characterType;

public:
    /*!
     * Create new object
     */
    Character(Object* parent, CharacterType characterType);

    /*!
     * Update asteroid
     * @param scene Scene to interact with
     * @param dt Time delta for animation purposes
     * @return
     */
    bool update(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentPosition) override;

    /*!
     * Render asteroid
     * @param scene Scene to render in
     */
    void render(Scene &scene) override;

};


#endif //PPGSO_CHARACTER_H





