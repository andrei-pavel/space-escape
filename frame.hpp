#ifndef FRAME_H
#define FRAME_H

#include "object-3d.hpp"

class Frame : public Object3D {

private:
  Vector3D dimensions;
  Vector4D diffuse;
  Vector4D ambient;
  Vector4D specular;

public:
  Frame(); // Constructor.

  void Draw();
  void SetDimensions(Vector3D dimensions);
  void SetDiffuse(Vector4D color);
};

#endif
