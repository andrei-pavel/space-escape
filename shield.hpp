#ifndef SHIELD_H
#define SHIELD_H

#include "object-3d.hpp"

class Shield : public Object3D {

public:
  GLint size;
  Vector4D diffuse;
  Vector4D ambient;
  Vector4D specular;
  float opacity;

public:
  Shield(); // Constructor

  void Draw();
  void SetSize(GLint size);
  void SetDiffuse(Vector4D color);
};

#endif
