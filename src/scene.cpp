#include "scene.h"

void Scene::update(float time) {
  camera->update(time);

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

void Scene::render() {
  // Simply render all rootObjects
  for ( auto& obj : rootObjects )
    obj->renderChildren(*this);
}

void Scene::renderLight(std::unique_ptr<ppgso::Shader> &shader) {
    shader->setUniform("LightsCount", lights.size());

    int i = 0;
    for (auto& light: lights) {
        std::string base = "lights[" + std::to_string(i) + "].";

        shader->setUniform(base + "position", light->position);
        shader->setUniform(base + "constant", light->constant);
        shader->setUniform(base + "linear", light->linear);
        shader->setUniform(base + "quadratic", light->quadratic);
        shader->setUniform(base + "color", light->color);
        shader->setUniform(base + "isSpotlight", light->isSpotlight);
        shader->setUniform(base + "direction", light->direction);
        shader->setUniform(base + "cutOff", light->cutOff);
        shader->setUniform(base + "outerCutOff", light->outerCutOff);

        i++;
    }

}