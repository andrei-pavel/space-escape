#ifndef DRAW_OFF_CPP
#define DRAW_OFF_CPP

#include <fstream>
#include <iostream>
#include <vector>

#include "vector-3d.hpp"

// It should fail if commented lines appear before useful data.
// Reads only objects with 3 edges / face.
#pragma once
void drawOFF(char *filename) {
  std::ifstream input;
  input.open(filename);
  if (input.is_open()) {
    char line[3];
    input >> line;
    if (line == "OFF") {
      unsigned int nOfVertices, nOfFaces, nOfEdges;
      input >> nOfVertices >> nOfFaces >> nOfEdges;
      std::vector<Vector3D> vertices;
      float x, y, z;
      for (unsigned int i = 0; i < nOfVertices - 1; i += 1) {
        input >> x >> y >> z;
        vertices.push_back(Vector3D(x, y, z));
      }
      std::vector<unsigned int> faces;
      unsigned int value;
      for (unsigned int i = 0; i < nOfFaces; i += 1) {
        input >> nOfEdges;
        for (unsigned int j = 0; j < nOfEdges; j += 1) {
          input >> value;
          faces.push_back(value);
        }
      }
      glBegin(GL_TRIANGLES);
      for (unsigned int i = 0; i < nOfFaces; i += 1) {
        glVertex3f(vertices[faces[3 * i]].x, vertices[faces[3 * i]].y,
                   vertices[faces[3 * i]].z);
        glVertex3f(vertices[faces[3 * i + 1]].x, vertices[faces[3 * i + 1]].y,
                   vertices[faces[3 * i + 1]].z);
        glVertex3f(vertices[faces[3 * i + 2]].x, vertices[faces[3 * i + 2]].y,
                   vertices[faces[3 * i + 2]].z);
      }
      glEnd();
    } else {
      std::cerr << "ERROR: Not an OFF file." << std::endl;
      input.close();
    }
  } else
    std::cerr << "ERROR: Unable to read from file." << std::endl;
}

#endif
