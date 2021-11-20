#ifndef PPGSO_TEXTURE_H
#define PPGSO_TEXTURE_H

#include <memory>

#include <ppgso/ppgso.h>

#include "src/scene.h"
#include "src/object.h"

class TextureGenerator {
public:
    static ppgso::Texture random();
};


#endif //PPGSO_TEXTURE_H
