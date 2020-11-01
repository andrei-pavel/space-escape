#include "light.hpp"

int Light::baseId = 0;

// Constructor with type parameter.
Light::Light() : Object3D(GlutSphere) {
  // Default values.
  diffuse = Vector4D(4, 4, 4, 1);
  ambient = Vector4D(0, 0, 0, 0);
  specular = Vector4D(1, 1, 1, 1);
  translation = Vector3D(0, 0, 0);
  color = Vector3D(1, 1, 1);
  scale = Vector3D(1, 1, 1);

  id = baseId; // id is unique.
  baseId += 1;
  Wireframe = false;
  LightType = Ideal; // By default, light is omnidrectional.
}

void Light::SetLightType(IlluminationType _LightType) {
  LightType = _LightType;
}

void Light::Render() { // Placing light in scene.

  // Standard attenuations.
  glLightf(GL_LIGHT0 + id, GL_CONSTANT_ATTENUATION, 1);
  glLightf(GL_LIGHT0 + id, GL_LINEAR_ATTENUATION, 0.01f);

  glLightfv(GL_LIGHT0 + id, GL_DIFFUSE,
            Vector4D(diffuse.x, diffuse.y, diffuse.z, diffuse.a)
                .Array());                                  // Light color.
  glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient.Array());   // Ambiental color.
  glLightfv(GL_LIGHT0 + id, GL_SPECULAR, specular.Array()); // Specular color.
  glLightfv(GL_LIGHT0 + id, GL_POSITION,
            Vector4D(translation.x, translation.y, translation.z, 1)
                .Array()); // Light position.

  if (LightType == Ideal) {
    ambient = Vector4D(1, 1, 1, 1);
    glLightfv(GL_LIGHT0 + id, GL_AMBIENT, ambient.Array()); // Ambiental color.
    glLightfv(
        GL_LIGHT0 + id, GL_SPOT_DIRECTION,
        Vector3D(0, 0, -1)
            .Array()); // Light orietnation will be towards dynamic camera.
  }

  if (LightType == Spot) {
    glLightfv(GL_LIGHT0 + id, GL_SPOT_DIRECTION,
              Vector3D(1, 0, 0)
                  .Array()); // Spot orietnation will be towards asteroids.
    glLightf(GL_LIGHT0 + id, GL_SPOT_CUTOFF, 45); // Opening of 45 degrees.
    glLightf(GL_LIGHT0 + id, GL_SPOT_EXPONENT, 2);
  }

  glEnable(GL_LIGHT0 + id); // Activating light.

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

  glColor4f(color.x, color.y, color.z, 1);
  glutSolidSphere(1, 10, 10);

  glPopMatrix();
}

// Deactivating light.
void Light::Disable() { glDisable(GL_LIGHT0 + id); }
