#include "scene.h"

void Scene::update(float time) {
  camera->update(time);

  // Use iterator to update all objects so we can remove while iterating
  auto i = std::begin(rootObjects);

  while (i != std::end(rootObjects)) {
    // Update and remove from list if needed
    auto obj = i->get();
    if (!obj->updateChildren(*this, time, glm::mat4{1.0f}))
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