#include "frame.hpp"

// Constructor.
Frame::Frame() : Object3D() {
  // Default values.
  color = Vector3D(WHITE_COLOR);
  diffuse = Vector4D(color.x, color.y, color.z, 1);
  ambient = Vector4D(color.x, color.y, color.z, 1);
  specular = Vector4D(color.x, color.y, color.z, 1);
  dimensions = Vector3D(800, 600, 800);
  scale = Vector3D(1, 1, 1);
  Wireframe = false; // By default, not wireframe.
}

// Overwriting by polimorphism.
void Frame::Draw() {
  if (!Visible)
    return;

  GLfloat x = dimensions.x / 2;
  GLfloat y = dimensions.y / 2;
  GLfloat z = dimensions.z / 2;

  glPushMatrix();

  // Material settings.
  glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
               Vector4D(diffuse.x, diffuse.y, diffuse.z, 1).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
               Vector4D(ambient.x, ambient.y, ambient.z, 1).Array());
  glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
               Vector4D(specular.x, specular.y, specular.z, 1).Array());

  // Translation.
  glTranslatef(translation.x, translation.y, translation.z);

  // Rotation.
  glRotatef(rotation.x, 1, 0, 0);
  glRotatef(rotation.y, 0, 1, 0);
  glRotatef(rotation.z, 0, 0, 1);

  // Scale.
  glScalef(scale.x, scale.y, scale.z);

  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
  glColor4f(color.x, color.y, color.z, 1);
  glBegin(GL_QUADS);

  // Back face.
  glNormal3f(0, 0, -1);
  glVertex3f(x, y, -z);
  glVertex3f(-x, y, -z);
  glVertex3f(-x, -y, -z);
  glVertex3f(x, -y, -z);

  // Front face.
  glNormal3f(0, 0, 1);
  glVertex3f(x, y, z);
  glVertex3f(-x, y, z);
  glVertex3f(-x, -y, z);
  glVertex3f(x, -y, z);

  // Left face.
  glNormal3f(-1, 0, 0);
  glVertex3f(-x, y, -z);
  glVertex3f(-x, y, z);
  glVertex3f(-x, -y, z);
  glVertex3f(-x, -y, -z);

  // Right face.
  glNormal3f(1, 0, 0);
  glVertex3f(x, y, -z);
  glVertex3f(x, y, z);
  glVertex3f(x, -y, z);
  glVertex3f(x, -y, -z);

  // Top face.
  glNormal3f(0, 1, 0);
  glVertex3f(x, y, -z);
  glVertex3f(-x, y, -z);
  glVertex3f(-x, y, z);
  glVertex3f(x, y, z);

  // Bottom face.
  glNormal3f(0, -1, 0);
  glVertex3f(x, -y, -z);
  glVertex3f(-x, -y, -z);
  glVertex3f(-x, -y, z);
  glVertex3f(x, -y, z);

  glEnd();

  glPopMatrix();
}
