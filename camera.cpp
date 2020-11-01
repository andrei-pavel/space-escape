#include "camera.hpp"

// Constructor with standard OpenGL values.
Camera::Camera() {
  Position = Vector3D(0, 0, 0);
  ForwardVector = Vector3D(0, 0, -1);
  RightVector = Vector3D(1, 0, 0);
  UpVector = Vector3D(0, 1, 0);
}

void Camera::RotateX(GLfloat Angle) {}

void Camera::RotateY(GLfloat Angle) {}

void Camera::RotateZ(GLfloat Angle) {}

void Camera::RotateXCenter(GLfloat Angle) {}

void Camera::RotateYCenter(GLfloat Angle) {
  float distance = 5;
  MoveForward(distance);
  RotateY(Angle);
  MoveBackward(distance);
}

void Camera::RotateZCenter(GLfloat Angle) {
  float distance = 5;
  MoveForward(distance);
  RotateZ(Angle);
  MoveBackward(distance);
}

// Placing observer in scene.
void Camera::Render() {
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  Vector3D ViewPoint =
      Position + ForwardVector; // Point at which camera points.

  gluLookAt(Position.x, Position.y, Position.z, ViewPoint.x, ViewPoint.y,
            ViewPoint.z, UpVector.x, UpVector.y, UpVector.z);
}

// Translations.
void Camera::MoveForward(GLfloat Distance) {
  Position = Position + ForwardVector * Distance;
}

void Camera::MoveBackward(GLfloat Distance) {
  Position = Position - ForwardVector * Distance;
}

void Camera::MoveRight(GLfloat Distance) {
  Position = Position + RightVector * Distance;
}

void Camera::MoveLeft(GLfloat Distance) {
  Position = Position - RightVector * Distance;
}

void Camera::MoveUpward(GLfloat Distance) {
  Position = Position + UpVector * Distance;
}

void Camera::MoveDownward(GLfloat Distance) {
  Position = Position - UpVector * Distance;
}

void Camera::SetPosition(Vector3D value) { Position = value; }

void Camera::SetForwardVector(Vector3D value) { ForwardVector = value; }

void Camera::SetRightVector(Vector3D value) { RightVector = value; }

void Camera::SetUpVector(Vector3D value) { UpVector = value; }
