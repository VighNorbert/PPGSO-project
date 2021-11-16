#ifndef PPGSO_OBJECT_H
#define PPGSO_OBJECT_H

#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>

// Forward declare a scene
class Scene;

/*!
 *  Abstract scene object interface
 *  All objects in the scene should be able to update and render
 *  Generally we also want to keep position, rotation and scale for each object to generate a modelMatrix
 */
class Object {
public:
  // Define default constructors as this is an abstract class
  Object() = default;
  Object(const Object&) = default;
  Object(Object&&) = default;
  virtual ~Object() {};

  std::list< std::unique_ptr<Object> > childObjects;
  std::unique_ptr<Object> parentObject;

  /*!
   * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation of children
   *
   * @param scene - Reference to the Scene the object is rendered in
   * @param dt - Time delta for animation purposes
   * @return true to delete the object
   */
  bool updateChildren(Scene &scene, float dt, glm::mat4 parentModelMatrix);

  /*!
   * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation
   *
   * @param scene - Reference to the Scene the object is rendered in
   * @param dt - Time delta for animation purposes
   * @return true to delete the object
   */
  virtual bool update(Scene &scene, float time, glm::mat4 parentModelMatrix) = 0;

  /*!
   * Render the children objects in the scene
   * @param scene
   */
  void renderChildren(Scene &scene);

  /*!
   * Render the object in the scene
   * @param scene
   */
  virtual void render(Scene &scene) = 0;

  // Object properties
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};
  glm::mat4 modelMatrix{1};

protected:
  /*!
   * Generate modelMatrix from position, rotation and scale
   */
  void generateModelMatrix(glm::mat4 parentModelMatrix);
};


#endif //PPGSO_OBJECT_H