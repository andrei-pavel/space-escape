#ifndef CAMERA_H
#define CAMERA_H

#include <GL/freeglut.h>

#include "vector-3d.hpp"

class Camera {

public:
  Vector3D ForwardVector;
  Vector3D RightVector;
  Vector3D UpVector;
  Vector3D Position;

public:
  Camera();

  void Render(); // Place observer in scene.

  // Rotation towards coordonate axis.
  void RotateX(GLfloat Angle);
  void RotateY(GLfloat Angle);
  void RotateZ(GLfloat Angle);

  // Rotation towards a center.
  void RotateXCenter(GLfloat Angle);
  void RotateYCenter(GLfloat Angle);
  void RotateZCenter(GLfloat Angle);

  // Translation
  void MoveForward(GLfloat Distance);
  void MoveBackward(GLfloat Distance);
  void MoveUpward(GLfloat Distance);
  void MoveDownward(GLfloat Distance);
  void MoveRight(GLfloat Distance);
  void MoveLeft(GLfloat Distance);

  void SetPosition(Vector3D value);
  void SetForwardVector(Vector3D value);
  void SetRightVector(Vector3D value);
  void SetUpVector(Vector3D value);
};

#endif
