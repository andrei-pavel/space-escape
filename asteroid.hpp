#ifndef ASTEROID_H
#define ASTEROID_H

#include <cstdlib>
#include <ctime>

#include "object-3d.hpp"

class Asteroid : public Object3D {

public:
  GLfloat size;
  Vector4D diffuse;
  Vector4D ambient;
  Vector4D specular;

public:
  Asteroid(); // Constructor

  unsigned long mix(unsigned long a, unsigned long b, unsigned long c);

  void Draw();
  void SetSize(GLfloat size);
  void SetDiffuse(Vector4D color);
};

#endif
