#ifndef SHIP_H
#define SHIP_H

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "object-3d.hpp"

class Ship : public Object3D {

public:
  Vector3D dimensions;
  Vector4D diffuse;
  Vector4D ambient;
  Vector4D specular;

  // OFF specific.
  unsigned int nOfVertices, nOfFaces, nOfEdges;
  std::vector<Vector3D> vertices;
  std::vector<unsigned int> faces;
  float smallestX, greatestX, smallestY, greatestY, smallestZ, greatestZ;

public:
  Ship(); // Constructor

  void Draw();
  void SetDimensions(Vector3D dimensions);
  void SetDiffuse(Vector4D color);

  // OFF specific.
  void readOFF(char *filename);
  Vector3D normal(Vector3D vertex1, Vector3D vertex2, Vector3D vertex3);
  void drawOFF();
};

#endif
