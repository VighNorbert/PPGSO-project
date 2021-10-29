#ifndef PPGSO_ROAD_H
#define PPGSO_ROAD_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

enum RoadType
{
    StraightRoad = 0,
    RoadCrossing = 1,
    Crossroad = 2,
    Sidewalk = 3,
    Sidewalk_Straight = 4,
    Sidewalk_Corner = 5,
    Sidewalk_Gutter = 6
};

class Road : public Object {
private:
    // Static resources (Shared between instances)
    static std::unique_ptr<ppgso::Mesh> mesh_road;
    static std::unique_ptr<ppgso::Mesh> mesh_crossing;
    static std::unique_ptr<ppgso::Mesh> mesh_crossroad;
    static std::unique_ptr<ppgso::Mesh> mesh_sidewalk;
    static std::unique_ptr<ppgso::Mesh> mesh_sidewalk_straight;
    static std::unique_ptr<ppgso::Mesh> mesh_sidewalk_corner;
    static std::unique_ptr<ppgso::Mesh> mesh_sidewalk_gutter;

    static std::unique_ptr<ppgso::Shader> shader;

    static std::unique_ptr<ppgso::Texture> texture_side;
    static std::unique_ptr<ppgso::Texture> texture_center;
    static std::unique_ptr<ppgso::Texture> texture_main;

    // Speed and rotational momentum
    glm::vec3 speed;
    glm::vec3 rotMomentum;


public:
    RoadType roadType = RoadType::StraightRoad;

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

    /**
     * Creates a complete road with sidewalk and everything that goes with it
     * @param scene - Scene, to which the road should be added
     * @param direction - 0: positive x, 1: positive z, 2: negative x, 3: negative z
     * @param length - Length of the road which should be created
     * @param starting_coords - Coords where the road should start
     */
    static void generateRoad(Scene &scene, int direction, int length, glm::float2 starting_coords) {
        float roadRotation = float(direction % 2 - 1) * ppgso::PI * .5f;
        float x0 = starting_coords[0] + 2.5f - (direction == 2 ? float(length * 5) : 0.f);
        float z0 = starting_coords[1] + 2.5f - (direction == 3 ? float(length * 5) : 0.f);

        for (int l = 0; l < length; l++)
        {
            // one side of the road
            auto road = std::make_unique<Road>();
            if (direction % 2 == 0) { //x direction
                road->position.x = x0 + float(l * 5);
                road->position.z = z0 - 5.f;
            } else {
                road->position.z = z0 + float(l * 5);
                road->position.x = x0 - 5.f;
            }
            road->rotation.z = roadRotation;
            scene.objects.push_back(move(road));

            // corresponding sidewalk
            road = std::make_unique<Road>();
            road->roadType = (l % 4 == 2) ? Sidewalk_Gutter : Sidewalk_Straight;
            if (direction % 2 == 0) { //x direction
                road->position.x = x0 + float(l * 5);
                road->position.z = z0 - 10.f;
            } else {
                road->position.z = z0 + float(l * 5);
                road->position.x = x0 - 10.f;
            }
            road->rotation.z = roadRotation + ppgso::PI * 0.5f;
            scene.objects.push_back(move(road));

            // second side of the road
            road = std::make_unique<Road>();
            if (direction % 2 == 0) { //x direction
                road->position.x = x0 + float(l * 5);
                road->position.z = z0;
            } else {
                road->position.z = z0 + float(l * 5);
                road->position.x = x0;
            }
            road->rotation.z = roadRotation + ppgso::PI;
            scene.objects.push_back(move(road));


            // corresponding sidewalk
            road = std::make_unique<Road>();
            road->roadType = (l % 4 == 0) ? Sidewalk_Gutter : Sidewalk_Straight;
            if (direction % 2 == 0) { //x direction
                road->position.x = x0 + float(l * 5);
                road->position.z = z0 + 5.f;
            } else {
                road->position.z = z0 + float(l * 5);
                road->position.x = x0 + 5.f;
            }
            road->rotation.z = roadRotation + ppgso::PI * 1.5f;
            scene.objects.push_back(move(road));
        }
    }

    /**
     * Creates a complete crossroad on the given coordinates
     * @param scene - Scene, to which the road should be added
     * @param coords - Coords where the center of the crossroad should be
     */
    static void generateCrossroad(Scene &scene, glm::float2 coords) {
        for (int x = -1; x < 2; x+=2) {
            for (int z = -1; z < 2; z+=2) {
                auto road = std::make_unique<Road>();
                road->roadType = Crossroad;
                road->position.x = coords[0] + float(x) * 2.5f;
                road->position.z = coords[1] + float(z) * 2.5f;
                scene.objects.push_back(move(road));

                road = std::make_unique<Road>();
                road->roadType = RoadCrossing;
                road->position.x = coords[0] + float(x) * 7.5f;
                road->position.z = coords[1] + float(z) * 2.5f;
                road->rotation.z = float(z) * ppgso::PI * 0.5f;
                scene.objects.push_back(move(road));

                road = std::make_unique<Road>();
                road->roadType = RoadCrossing;
                road->position.x = coords[0] + float(x) * 2.5f;
                road->rotation.z = float(x == 1) * ppgso::PI;
                road->position.z = coords[1] + float(z) * 7.5f;
                scene.objects.push_back(move(road));

                road = std::make_unique<Road>();
                road->roadType = Sidewalk_Corner;
                road->position.x = coords[0] + float(x) * 7.5f;
                road->position.z = coords[1] + float(z) * 7.5f;
                road->rotation.z = (1.25f - float(z) * (x == 1 ? .25f : .75f)) * ppgso::PI;
                scene.objects.push_back(move(road));
            }
        }
    }
};


#endif //PPGSO_ROAD_H
