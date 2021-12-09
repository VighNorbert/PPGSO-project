#ifndef PPGSO_OBJECT_H
#define PPGSO_OBJECT_H

#include <memory>
#include <list>
#include <map>

#include <glm/glm.hpp>
#include <shader.h>
#include "ppgso.h"
#include "keyframe.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

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
  Object* parentObject = nullptr;

  /*!
   * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation of children
   *
   * @param scene - Reference to the Scene the object is rendered in
   * @param dt - Time delta for animation purposes
   * @return true to delete the object
   */
  bool updateChildren(Scene &scene, float dt, glm::mat4 parentModelMatrix, glm::vec3 parentPosition);

  /*!
   * Update Object parameters, usually used to update the modelMatrix based on position, scale and rotation
   *
   * @param scene - Reference to the Scene the object is rendered in
   * @param dt - Time delta for animation purposes
   * @return true to delete the object
   */
  virtual bool update(Scene &scene, float time, glm::mat4 parentModelMatrix, glm::vec3 parentPosition) = 0;

  /*!
   * Render the children objects in the scene
   * @param scene
   */
  void renderChildren(Scene &scene, GLuint depthMap);

  /*!
   * Render the object in the scene
   * @param scene
   */
  virtual void render(Scene &scene, GLuint depthMap) = 0;

  /*!
   * Render the object in the scene
   * @param scene
   */
  void renderForShadowChildren(Scene &scene);

  /*!
   * Render the object in the scene
   * @param scene
   */
  virtual void renderForShadow(Scene &scene) = 0;

  // Object properties
  glm::vec3 globalRotation{0,0,0};
  glm::vec3 position{0,0,0};
  glm::vec3 rotation{0,0,0};
  glm::vec3 scale{1,1,1};
  glm::mat4 modelMatrix{1};

  // Speed and rotational momentum
  glm::vec3 speed{0, 0, 0};
  glm::vec3 rotMomentum{0, 0, 0};


  std::list<Keyframe> keyframes;
  float age = 0.f;

  static glm::mat4 getModelMatrix(glm::vec3 position, glm::vec3 rotation = {0, 0, 0}, glm::vec3 scale = {1, 1, 1}) {
    return glm::translate(glm::mat4(1.0f), position)
       * glm::orientate4(rotation)
       * glm::scale(glm::mat4(1.0f), scale);
  }

  static glm::mat4 interpolate(glm::mat4& _mat1, glm::mat4& _mat2, float _time)
  {
      glm::quat rot0 = glm::quat_cast(_mat1);
      glm::quat rot1= glm::quat_cast(_mat2);

      glm::quat finalRot = glm::slerp(rot0, rot1, _time);

      glm::mat4 finalMat = glm::mat4_cast(finalRot);

      finalMat[3] = _mat1[3] * (1 - _time) + _mat2[3] * _time;

      return finalMat;
  }

  bool keyframesUpdate(Scene &scene) {
      float t = 0.0f;
      glm::mat4 actual, next, last;
      glm::vec3 actualrot, nextrot, lastrot;
      glm::vec3 actualpos, nextpos, lastpos;
      float duration;
      for (auto iter= keyframes.begin(); iter != keyframes.end(); iter++) {
          if (iter->duration == -1) return false;
          else if (t + iter->duration > age) {
              actual = iter->matrix;
              actualrot = iter->rotation;
              actualpos = iter->position;
              duration = iter->duration;
              iter++;
              if (iter == keyframes.end()) {
                  modelMatrix = actual;
                  rotation = actualrot;
                  position = actualpos;
                  return true;
              }
              next = iter->matrix;
              nextrot = iter->rotation;
              nextpos = iter->position;
              float a = (age - t) / duration;
              modelMatrix = Object::interpolate(actual, next, a);
              rotation = glm::lerp(actualrot, nextrot, a);
              position = glm::lerp(actualpos, nextpos, a);
              return true;
          }
          last = iter->matrix;
          lastrot = iter->rotation;
          lastpos = iter->position;
          t += iter->duration;
      }
      modelMatrix = last;
      rotation = lastrot;
      position = lastpos;
      return true;
  }

protected:
  /*!
   * Generate modelMatrix from position, rotation and scale
   */
  void generateModelMatrix(glm::mat4 parentModelMatrix);
};


#endif //PPGSO_OBJECT_H