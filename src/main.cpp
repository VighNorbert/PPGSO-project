#include <iostream>
#include <vector>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtx/matrix_transform_2d.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <ppgso/ppgso.h>

#include <shaders/color_vert_glsl.h>
#include <shaders/color_frag_glsl.h>
#include "SceneWindow.hpp"

const int SIZE_X = 1920;
const int SIZE_Y = 1080;


int main() {
    // Create our window
    auto window = SceneWindow{"Demoscéna - Mesto", SIZE_X, SIZE_Y};

    // Main execution loop
    while (window.pollEvents()) {}

    return EXIT_SUCCESS;
}
