#include "scene.h"

void Scene::update(float time) {
  camera->update(time);

  if (showFPS)
    std::cout << "FPS: " << 1 / time << std::endl;

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(rootObjects);

  while (i != std::end(rootObjects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    if (!obj->updateChildren(*this, time, glm::mat4{1.0f}, {0, 0, 0}))
      i = rootObjects.erase(i); // NOTE: no need to call destructors as we store shared pointers in the scene
    else
      ++i;
  }
}

void Scene::render(GLuint depthMap) {
  // Simply render all rootObjects
  for ( auto& obj : rootObjects )
    obj->renderChildren(*this, depthMap);
}

void Scene::renderLight(std::unique_ptr<ppgso::Shader> &shader, bool onlyMain) {
    int i = 0;
    if (mainlight != nullptr) {
        std::string base = "lights[" + std::to_string(i) + "].";
        shader->setUniform(base + "position", mainlight->position);
        shader->setUniform(base + "constant", mainlight->constant);
        shader->setUniform(base + "linear", mainlight->linear);
        shader->setUniform(base + "quadratic", mainlight->quadratic);
        shader->setUniform(base + "color", mainlight->color);
        shader->setUniform(base + "maxDist", mainlight->maxDist);
        shader->setUniform(base + "isSpotlight", mainlight->isSpotlight);
        shader->setUniform(base + "direction", mainlight->direction);
        shader->setUniform(base + "cutOff", mainlight->cutOff);
        shader->setUniform(base + "outerCutOff", mainlight->outerCutOff);
        i++;
    }
    if (!onlyMain) {
        for (auto &light: lights) {
            std::string base = "lights[" + std::to_string(i) + "].";

            shader->setUniform(base + "position", light->position);
            shader->setUniform(base + "constant", light->constant);
            shader->setUniform(base + "linear", light->linear);
            shader->setUniform(base + "quadratic", light->quadratic);
            shader->setUniform(base + "color", light->color);
            shader->setUniform(base + "maxDist", light->maxDist);
            shader->setUniform(base + "isSpotlight", light->isSpotlight);
            shader->setUniform(base + "direction", light->direction);
            shader->setUniform(base + "cutOff", light->cutOff);
            shader->setUniform(base + "outerCutOff", light->outerCutOff);
            i++;
        }
    }

    shader->setUniform("LightsCount", i);
}

void Scene::renderForShadow() {
    // Simply render all rootObjects
    for ( auto& obj : rootObjects )
        obj->renderForShadowChildren(*this);
}
