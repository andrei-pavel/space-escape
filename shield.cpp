#include "shield.hpp"

// Constructor with size parameter
Shield::Shield() : Object3D() {
  // Default values.
  this->SetPosition(Vector3D(0, 0, 0));
  opacity = 0.5;
  color = Vector3D(YELLOW_COLOR);
  scale = Vector3D(1, 1, 1);
  diffuse = Vector4D(YELLOW_COLOR, opacity);
  ambient = Vector4D(YELLOW_COLOR, opacity);
  specular = Vector4D(YELLOW_COLOR, opacity);
  translation = Vector3D(0, 0, 0);
  size = 50;
  Wireframe = true;
  Visible = true;
}

void Shield::SetSize(GLint _size) { size = _size; }

void Shield::SetDiffuse(Vector4D color) { diffuse = color; }

// Overwriting by polimorphism.
void Shield::Draw() {

  if (!Visible)
    return;

  // Updating opacity.
  diffuse = Vector4D(YELLOW_COLOR, opacity);
  ambient = Vector4D(YELLOW_COLOR, opacity);
  specular = Vector4D(YELLOW_COLOR, opacity);

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
               Vector4D(diffuse.x, diffuse.y, diffuse.z, opacity).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
               Vector4D(ambient.x, ambient.y, ambient.z, opacity).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
               Vector4D(specular.x, specular.y, specular.z, opacity).Array());

  glColor4f(color.x, color.y, color.z, opacity);
  glutSolidSphere(size, size + 2, size + 2);
  glPopMatrix();
}
