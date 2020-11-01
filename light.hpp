#ifndef LIGHT_H
#define LIGHT_H

#include "object-3d.hpp"

enum IlluminationType {
  Ideal, // Omnidirectional light.
  Spot   // Directional light.
};

class Light : public Object3D {

public:
  static int baseId;
  int id; // Starts with 0 and is used for GL_LIGHT0 + id.
  Vector4D diffuse;
  Vector4D ambient;
  Vector4D specular;
  IlluminationType LightType;

public:
  Light();
  void Render();  // Places light in scene and activates it.
  void Disable(); // Deactivate light.
  void SetLightType(IlluminationType LightType);
};

#endif
