#include "asteroid.hpp"

unsigned long Asteroid::mix(unsigned long a, unsigned long b, unsigned long c) {
  a = a - b;
  a = a - c;
  a = a ^ (c >> 13);
  b = b - c;
  b = b - a;
  b = b ^ (a << 8);
  c = c - a;
  c = c - b;
  c = c ^ (b >> 13);
  a = a - b;
  a = a - c;
  a = a ^ (c >> 12);
  b = b - c;
  b = b - a;
  b = b ^ (a << 16);
  c = c - a;
  c = c - b;
  c = c ^ (b >> 5);
  a = a - b;
  a = a - c;
  a = a ^ (c >> 3);
  b = b - c;
  b = b - a;
  b = b ^ (a << 10);
  c = c - a;
  c = c - b;
  c = c ^ (b >> 15);
  return c;
}

// Constructor with size parameter.
Asteroid::Asteroid() : Object3D() {
  // Default values.
  srand(time(NULL));
  color =
      Vector3D((GLfloat)(rand() % 100) / 1000, (GLfloat)(rand() % 100) / 1000,
               (GLfloat)(rand() % 100) / 1000);
  diffuse = Vector4D(color.x, color.y, color.z, 1);
  ambient = Vector4D(color.x, color.y, color.z, 1);
  specular = Vector4D(color.x, color.y, color.z, 1);
  scale = Vector3D(25 + (GLfloat)(rand() % 100) / 4,
                   25 + (GLfloat)(rand() % 100) / 4,
                   25 + (GLfloat)(rand() % 100) / 4);
  translation = Vector3D(400, (GLfloat)(rand() % 400 - 200),
                         (GLfloat)(rand() % 400 - 200));
  Wireframe = false; // By default, not wireframe.
}

void Asteroid::SetSize(GLfloat size) { size = size; }

void Asteroid::SetDiffuse(Vector4D color) { diffuse = color; }

// Overwriting by polimorphism.
void Asteroid::Draw() {

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
  if (!selected) {
    glColor3f(color.x, color.y, color.z);
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
                 Vector4D(color.x, color.y, color.z, 1).Array());
  } else {
    glColor3f(SelectedColor.x, SelectedColor.y, SelectedColor.z);
    glMaterialfv(
        GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE,
        Vector4D(SelectedColor.x, SelectedColor.y, SelectedColor.z, 1).Array());
  }
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
               Vector4D(0.1f, 0.1f, 0.1f, 1).Array());

  if (Wireframe)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

  // Draw here.
  glutSolidDodecahedron();

  glPopMatrix();
}
