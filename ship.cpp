#include "ship.hpp"

float max4f(float a, float b, float c, float d) {
  if (b <= a && c <= a && d <= a)
    return a;
  else if (a <= b && c <= b && d <= b)
    return b;
  else if (a <= c && b <= c && d <= c)
    return c;
  else if (a <= d && b <= d && c <= d)
    return d;
  else
    return a; // Never gonna happen, but gets rid of warning.
}

float min4f(float a, float b, float c, float d) {
  if (a <= b && a <= c && a <= d)
    return a;
  else if (b <= a && b <= c && b <= d)
    return b;
  else if (c <= a && c <= b && c <= d)
    return c;
  else if (d <= a && d <= b && d <= c)
    return d;
  else
    return a; // Never gonna happen, but gets rid of warning.
}

// It should fail if commented lines appear before useful data.
// Reads only objects with 3 edges / face.
void Ship::readOFF(char *filename) {
  std::ifstream input;
  input.open(filename);
  if (input.is_open()) {
    std::string line;
    input >> line;
    if (line == "OFF") {
      input >> nOfVertices >> nOfFaces >> nOfEdges;
      float x, y, z;
      for (unsigned int i = 0; i < nOfVertices; i += 1) {
        input >> x >> y >> z;
        vertices.push_back(Vector3D(x, y, z));
      }
      unsigned int value;
      for (unsigned int i = 0; i < nOfFaces; i += 1) {
        input >> nOfEdges;
        for (unsigned int j = 0; j < nOfEdges; j += 1) {
          input >> value;
          faces.push_back(value);
        }
      }

      for (unsigned int i = 0; i < nOfFaces; i += 1) {
        smallestX =
            min4f(smallestX, vertices[faces[3 * i]].x,
                  vertices[faces[3 * i + 1]].x, vertices[faces[3 * i + 2]].x);
        smallestY =
            min4f(smallestY, vertices[faces[3 * i]].y,
                  vertices[faces[3 * i + 1]].y, vertices[faces[3 * i + 2]].y);
        smallestZ =
            min4f(smallestZ, vertices[faces[3 * i]].z,
                  vertices[faces[3 * i + 1]].z, vertices[faces[3 * i + 2]].z);
        greatestX =
            max4f(greatestX, vertices[faces[3 * i]].x,
                  vertices[faces[3 * i + 1]].x, vertices[faces[3 * i + 2]].x);
        greatestY =
            max4f(greatestY, vertices[faces[3 * i]].y,
                  vertices[faces[3 * i + 1]].y, vertices[faces[3 * i + 2]].y);
        greatestZ =
            max4f(greatestZ, vertices[faces[3 * i]].z,
                  vertices[faces[3 * i + 1]].z, vertices[faces[3 * i + 2]].z);
      }
    } else {
      std::cerr << "ERROR: Not an OFF file." << std::endl;
      input.close();
    }
  } else
    std::cerr << "ERROR: Unable to read from file." << std::endl;
}

Vector3D Ship::normal(Vector3D vertex1, Vector3D vertex2, Vector3D vertex3) {
  Vector3D A, B, normal;

  A.x = vertex1.x - vertex2.x;
  A.y = vertex1.y - vertex2.y;
  A.z = vertex1.z - vertex2.z;

  B.x = vertex2.x - vertex3.x;
  B.y = vertex2.y - vertex3.y;
  B.z = vertex2.z - vertex3.z;

  normal.x = A.y * B.z - A.z * B.y;
  normal.y = A.z * B.x - A.x * B.z;
  normal.z = A.x * B.y - A.y * B.x;

  return normal;
}

void Ship::drawOFF() {
  glBegin(GL_TRIANGLES);
  for (unsigned int i = 0; i < nOfFaces; i += 1) {
    if (vertices[faces[3 * i]].y > 0.3f ||
        vertices[faces[3 * i + 1]].y > 0.3f ||
        vertices[faces[3 * i + 2]].y > 0.3f) {
      // Material settings.
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   Vector4D(BLUE_GLASS_COLOR, 1).Array());
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   Vector4D(BLUE_GLASS_COLOR, 1).Array());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   Vector4D(BLUE_GLASS_COLOR, 1).Array());
      glColor3f(BLUE_GLASS_COLOR);
    } else {
      // Material settings.
      glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                   Vector4D(SHIP_COLOR, 1).Array());
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                   Vector4D(SHIP_COLOR, 1).Array());
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                   Vector4D(SHIP_COLOR, 1).Array());
      glColor3f(SHIP_COLOR);
    }
    glVertex3f(dimensions.x * vertices[faces[3 * i]].x,
               dimensions.y * vertices[faces[3 * i]].y,
               dimensions.z * vertices[faces[3 * i]].z);
    glVertex3f(dimensions.x * vertices[faces[3 * i + 1]].x,
               dimensions.y * vertices[faces[3 * i + 1]].y,
               dimensions.z * vertices[faces[3 * i + 1]].z);
    glVertex3f(dimensions.x * vertices[faces[3 * i + 2]].x,
               dimensions.y * vertices[faces[3 * i + 2]].y,
               dimensions.z * vertices[faces[3 * i + 2]].z);
    Vector3D _normal =
        normal(vertices[faces[3 * i]], vertices[faces[3 * i + 1]],
               vertices[faces[3 * i + 2]]);
    glNormal3f(_normal.x, _normal.y, _normal.z);
  }
  glEnd();
}

// Constructor with size parameter.
Ship::Ship() : Object3D() {
  // Default values.
  this->SetPosition(Vector3D(0, 0, 0));
  color = Vector3D(SHIP_COLOR);
  diffuse = Vector4D(SHIP_COLOR, 1);
  ambient = Vector4D(SHIP_COLOR, 1);
  specular = Vector4D(SHIP_COLOR, 1);
  Wireframe = false; // By default, not wireframe.
  dimensions = Vector3D(100, 100, 100);
  translation = Vector3D(0, 0, 0);
  rotation = Vector3D(0, 0, 0);
  scale = Vector3D(1, 1, 1);
  smallestX = INF_DIM;
  smallestY = INF_DIM;
  smallestZ = INF_DIM;
  greatestX = -INF_DIM;
  greatestY = -INF_DIM;
  greatestZ = -INF_DIM;
  readOFF("spaceship.off");
}

void Ship::SetDimensions(Vector3D dimensions) { dimensions = dimensions; }

void Ship::SetDiffuse(Vector4D color) { diffuse = color; }

// Overwriting by polimorphism.
void Ship::Draw() {

  if (!Visible)
    return;

  glPushMatrix();

  // Translation.
  glTranslatef(translation.x, translation.y, translation.z);

  // Rotation.
  glRotatef(rotation.x, 1, 0, 0);
  glRotatef(rotation.y, 0, 1, 0);
  glRotatef(rotation.z, 0, 0, 1);

  // Scale.
  glScalef(scale.x, scale.y, scale.z);

  // Material settings.
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
               Vector4D(diffuse.x, diffuse.y, diffuse.z, 1).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
               Vector4D(ambient.x, ambient.y, ambient.z, 1).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
               Vector4D(specular.x, specular.y, specular.z, 1).Array());

  if (Wireframe) {
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  } else {
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
  }

  glColor4f(color.x, color.y, color.z, 1);
  drawOFF();

  glPopMatrix();
}
