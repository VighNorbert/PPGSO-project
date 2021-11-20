#include "texture.h"

// Static resources


ppgso::Texture TextureGenerator::random() {
    std::vector<std::string> textures = {
            "textures/PolygonCity_Texture_01_A.bmp",
            "textures/PolygonCity_Texture_01_B.bmp",
            "textures/PolygonCity_Texture_01_C.bmp",
            "textures/PolygonCity_Texture_02_A.bmp",
            "textures/PolygonCity_Texture_02_B.bmp",
            "textures/PolygonCity_Texture_02_C.bmp",
            "textures/PolygonCity_Texture_03_A.bmp",
            "textures/PolygonCity_Texture_03_B.bmp",
            "textures/PolygonCity_Texture_03_C.bmp",
            "textures/PolygonCity_Texture_04_A.bmp",
            "textures/PolygonCity_Texture_04_B.bmp",
            "textures/PolygonCity_Texture_04_C.bmp"
    };
    int p = rand() % textures.size();

    return ppgso::image::loadBMP(textures.at(p));
//    return reinterpret_cast<ppgso::Texture &>(textures.at(p,
}
