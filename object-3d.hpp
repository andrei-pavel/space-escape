#ifndef OBJECT3D_H
#define OBJECT3D_H

#include "headers-and-defines.hpp"
#include "vector-3d.hpp"
#include "vector-4d.hpp"

#define MAXIMUM_LOD 50 // Maximum level of detail.

enum ObjectType { GlutCube, GlutSphere, Custom };

class Object3D {

public:                 // start with small letter
  Vector3D translation; // Position.
  Vector3D rotation;
  Vector3D scale;
  Vector3D color;
  Vector3D normal;

  int levelOfDetail; // for GlutSphere
  bool selected;

public: // start with big letter
  static Vector3D SelectedColor;
  static Vector3D ColorIncrement;
  ObjectType Type;
  bool Wireframe;
  bool Visible;
  bool Lighted;

  // Constructors.
  Object3D();           // No parameters.
  Object3D(ObjectType); // With type parameter.
  Object3D(Vector3D);   // With position parameter.
  Object3D(Vector3D, Vector3D,
           Vector3D); // With position, rotaton and scale parameters.

  void virtual Draw();

  // Setters for all non-public variables.
  void Select();
  void Deselect();
  void SetPosition(Vector3D);
  void SetRotation(Vector3D);
  void SetScale(Vector3D);
  void SetColor(Vector3D);
  void SetLevelOfDetail(int);

  // Getters for all modifiable, non-public variables.
  Vector3D GetRotation();
  Vector3D GetScale();
  Vector3D GetPosition();
  int GetLevelOfDetail();

protected:
  void virtual customDraw(); // Is being called only if Type is Custom.
  void defaultSettings();    // Set default settings.
};

#endif
