#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include "ObjectType.hpp"
#include "ObjectEntity.hpp"
#include "OriginWindow.hpp"

const int SIZE_X = 1024;
const int SIZE_Y = 1024;


int main() {
    // Create our window
    auto window = OriginWindow{SIZE_X, SIZE_Y};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
