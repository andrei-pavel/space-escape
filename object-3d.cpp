#include "object-3d.hpp"

// Static variables.
Vector3D Object3D::SelectedColor = Vector3D(RED_COLOR);
Vector3D Object3D::ColorIncrement = Vector3D(
    0.04f, 0.04f,
    0.04f); // Value with which SelectedColor is incremented / decremented.

// Basic constructor.
Object3D::Object3D() { defaultSettings(); }

// Constructor with type parameter.
Object3D::Object3D(ObjectType _Type) {
  defaultSettings();
  Type = _Type;
}

// Constructor with position parameter.
Object3D::Object3D(Vector3D _translation) {
  defaultSettings();
  this->SetPosition(_translation);
}

// Constructor with position, rotation and scale parameters.
Object3D::Object3D(Vector3D _translation, Vector3D _rotation, Vector3D _scale) {
  defaultSettings();
  this->SetPosition(_translation);
  this->SetRotation(_rotation);
  this->SetScale(_scale);
}

// Constructor with default settings.
void Object3D::defaultSettings() {
  translation = Vector3D(0, 0, 0);
  rotation = Vector3D(0, 0, 0);
  scale = Vector3D(1, 1, 1);
  normal = Vector3D(0, 1, 0);

  Wireframe = false;
  Visible = true;
  Lighted = true;
  Type = GlutSphere;

  levelOfDetail = 12;
  selected = false;

  SelectedColor = Vector3D(RED_COLOR);
  ColorIncrement = Vector3D(0.01f, 0.01f, 0.01f);
}

void Object3D::Draw() {

  if (!Visible)
    return;

  glPushMatrix();

  // Translation.
  glTranslatef(translation.x, translation.y, translation.z);

  // Rotation.
  glRotatef(rotation.x, 1.0, 0.0, 0.0);
  glRotatef(rotation.y, 0.0, 1.0, 0.0);
  glRotatef(rotation.z, 0.0, 0.0, 1.0);

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

  // daca este wireframe
  if (Wireframe)
    switch (Type) {
    case GlutCube:
      glutWireCube(1);
      break;
    case GlutSphere:
      glutWireSphere(1, levelOfDetail, levelOfDetail);
      break;
    case Custom:
      customDraw();
    }
  else
    switch (Type) {
    case GlutCube:
      glutSolidCube(1);
      break;
    case GlutSphere:
      glutSolidSphere(1, levelOfDetail, levelOfDetail);
      break;
    case Custom:
      customDraw();
    }

  glPopMatrix();
}

// Wireframe should be treated inside.
void Object3D::customDraw() {
  // TODO: draw personalized object.
}

void Object3D::Select() { selected = true; }

void Object3D::Deselect() { selected = false; }

// Setters.
void Object3D::SetColor(Vector3D _color) { color = _color; }

void Object3D::SetPosition(Vector3D _translation) {
  translation = _translation;
}

void Object3D::SetRotation(Vector3D _rotation) { rotation = _rotation; }

void Object3D::SetScale(Vector3D _scale) { scale = _scale; }

void Object3D::SetLevelOfDetail(int _levelOfDetail) {
  if (_levelOfDetail > 0 && _levelOfDetail < MAXIMUM_LOD)
    levelOfDetail = _levelOfDetail;
}

// Getters.
Vector3D Object3D::GetPosition() { return translation; }

Vector3D Object3D::GetRotation() { return rotation; }

Vector3D Object3D::GetScale() { return scale; }

int Object3D::GetLevelOfDetail() { return levelOfDetail; }
