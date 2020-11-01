/*------------------------------------------------------------------------------
/							Tem� 4: SpaceEscape 2012
/
/							Pavel Andrei-Cristian
/									331CA
/
/							Read Files/README.txt
/-------------------------------------------------------------------------------
*/

#include <cstdarg>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <iostream>
#include <vector>

#include "asteroid.hpp"
#include "camera.hpp"
#include "frame.hpp"
#include "light.hpp"
#include "shield.hpp"
#include "ship.hpp"

float *Vector3D::arr;
float *Vector4D::arr;

// Global Variables
bool depthEnabled, blendEnabled, lineSmoothEnabled, asteroidIsBeingKilled,
    shipDestroyed, asteroidCameraMode, asteroidHalt;
int object, cameraIsOnAsteroid;
unsigned long iteration, currentIteration;
Camera *dynamicCam, *shipCam, *asteroidCam, *cam;
Frame *frame;
Ship *ship;
Asteroid *asteroid;
Shield *shield;
Light *light_left, *light_right, *light_o1, *light_o2;
std::vector<Asteroid *> asteroids;
std::vector<float> speed;

float max3f(float a, float b, float c) {
  if (b <= a && c <= a)
    return a;
  else if (a <= b && c <= b)
    return b;
  else if (a <= c && b <= c)
    return c;
  else
    return a; // Never gonna happen, but gets rid of warning.
}

int collision() {
  for (unsigned int i = 0; i < asteroids.size(); i += 1)
    if (abs(asteroids[i]->translation.x - shield->translation.x) <
            asteroid->scale.x + ship->dimensions.x / 2 &&
        abs(asteroids[i]->translation.y - shield->translation.y) <
            asteroid->scale.y + ship->dimensions.y / 2 &&
        abs(asteroids[i]->translation.z - shield->translation.z) <
            asteroid->scale.z + ship->dimensions.z / 2)
      return i; // Collision with asteroid i.
  return -1;    // No collision.
}

void init() {
  // Just another new scene in OpenGL.
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);
  glEnable(GL_LIGHTING);
  glEnable(GL_NORMALIZE);
}

void initScene() {

  // Variables.
  depthEnabled = false;
  blendEnabled = false;
  lineSmoothEnabled = false;
  asteroidIsBeingKilled = false;
  shipDestroyed = false;
  asteroidCameraMode = false;
  asteroidHalt = false;
  object = 0;
  cameraIsOnAsteroid = -1;

  // Initializing vector arrays.
  Vector3D::arr = new float[3];
  Vector4D::arr = new float[4];

  frame = new Frame();
  ship = new Ship();
  light_left = new Light();
  light_left->SetLightType(Spot);
  light_right = new Light();
  light_right->SetLightType(Spot);
  light_o1 = new Light();
  light_o1->SetLightType(Ideal);
  light_o1->diffuse = Vector4D(0, 1, 0, 1);
  light_o1->ambient = Vector4D(0, 1, 0, 1);
  light_o1->specular = Vector4D(0, 1, 0, 1);
  light_o1->color = Vector3D(0, 1, 0);
  light_o2 = new Light();
  light_o2->SetLightType(Ideal);
  light_o1->diffuse = Vector4D(1, 0, 0, 1);
  light_o1->ambient = Vector4D(1, 0, 0, 1);
  light_o1->specular = Vector4D(1, 0, 0, 1);
  light_o1->color = Vector3D(1, 0, 0);
  light_o1->SetPosition(Vector3D(400, 300, 400));
  light_o2->SetPosition(Vector3D(-400, 300, 400));
  shield = new Shield();
  shield->size =
      (int)(max3f(ship->dimensions.x * (ship->greatestX - ship->smallestX),
                  ship->dimensions.y * (ship->greatestY - ship->smallestY),
                  ship->dimensions.y * (ship->greatestZ - ship->smallestZ)) *
            0.55);

  dynamicCam = new Camera();
  dynamicCam->SetForwardVector(Vector3D(0, 0, 1));
  dynamicCam->SetRightVector(Vector3D(-1, 0, 0));
  dynamicCam->SetUpVector(Vector3D(0, 1, 0));
  dynamicCam->SetPosition(Vector3D(0, 0, DYNAMIC_CAMERA_DISTANCE));
  shipCam = new Camera();
  shipCam->SetForwardVector(Vector3D(1, 0, 0));
  shipCam->SetRightVector(Vector3D(0, 0, 1));
  shipCam->SetUpVector(Vector3D(0, 1, 0));
  shipCam->SetPosition(shipCam->Position +
                       ship->dimensions * shipCam->ForwardVector / 10);
  asteroidCam = new Camera();
  asteroidCam->SetForwardVector(Vector3D(1, 0, 0));
  asteroidCam->SetRightVector(Vector3D(0, 0, 1));
  asteroidCam->SetUpVector(Vector3D(0, 1, 0));
  asteroidCam->SetPosition(shipCam->Position +
                           ship->dimensions * shipCam->ForwardVector / 10);
  cam = dynamicCam;

  light_left->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  light_right->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  shield->translation = shipCam->Position - ship->dimensions *
                                                shipCam->ForwardVector *
                                                CAM_SHIP_DISTANCE_MULTIPLIER;
  ship->translation = shipCam->Position -
                      ship->dimensions *
                          (shipCam->ForwardVector + Vector3D(2.2f, 1, 2.2f)) *
                          CAM_SHIP_DISTANCE_MULTIPLIER;

  GLfloat globalAmbient[] = {1, 1, 1, 1};
  glLightModelfv(GL_LIGHT_MODEL_AMBIENT, globalAmbient);

  glInitNames();

  glNewList(STATIC_LIST, GL_COMPILE);
  frame->Draw();
  light_o1->Render();
  light_o2->Render();
  glEndList();

  glNewList(SHIP_LIST, GL_COMPILE);
  light_left->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  light_right->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  shield->translation = shipCam->Position - ship->dimensions *
                                                shipCam->ForwardVector *
                                                CAM_SHIP_DISTANCE_MULTIPLIER;
  ship->translation = shipCam->Position -
                      ship->dimensions *
                          (shipCam->ForwardVector + Vector3D(2.2f, 1, 2.2f)) *
                          CAM_SHIP_DISTANCE_MULTIPLIER;
  ship->Draw();
  shield->Draw();
  light_left->Render();
  light_right->Render();
  glEndList();
}

void drawScene() {

  light_left->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  light_right->translation =
      shipCam->Position -
      ship->dimensions * shipCam->ForwardVector * CAM_SHIP_DISTANCE_MULTIPLIER;
  shield->translation =
      shipCam->Position - ship->dimensions *
                              (shipCam->ForwardVector + Vector3D(0, -1, 0)) *
                              CAM_SHIP_DISTANCE_MULTIPLIER;
  ship->translation = shipCam->Position -
                      ship->dimensions *
                          (shipCam->ForwardVector + Vector3D(2.2f, 0, 2.2f)) *
                          CAM_SHIP_DISTANCE_MULTIPLIER;

  glCallList(STATIC_LIST);
  glPushMatrix();
  glTranslatef(shield->translation.x, shield->translation.y,
               shield->translation.z);
  if (!shipDestroyed)
    glCallList(SHIP_LIST);
  glPopMatrix();

  if (iteration % 100 == 0) {
    asteroid = new Asteroid;
    asteroids.push_back(asteroid);
    speed.push_back((float)(10 + rand() % 100) / 100);
  }
  srand(time(NULL));
  for (unsigned int i = 0; i < asteroids.size(); i += 1) {
    asteroid = asteroids[i];
    if (asteroid->translation.x < -400) {
      if (object == i + 1)
        object = 0;
      asteroids.erase(asteroids.begin() + i);
      speed.erase(speed.begin() + i);
    } else {
      if (object == i + 1 && !shipDestroyed)
        asteroid->Select();
      else
        asteroid->Deselect();
      if (!asteroidHalt)
        asteroid->translation.x -= speed[i];
      glPushName(i + 1);
      if (cameraIsOnAsteroid != i)
        asteroid->Draw();
      glPopName();
    }
    if (asteroid->selected && !shipDestroyed) {
      if (asteroidCameraMode) {
        asteroidCam->SetPosition(asteroid->translation);
        asteroidCam->SetForwardVector(shield->translation -
                                      asteroid->translation);
        cameraIsOnAsteroid = i;
        cam = asteroidCam;
      } else if (!asteroidIsBeingKilled) {
        currentIteration = iteration;
        asteroidIsBeingKilled = true;
      } else if (iteration - currentIteration < 25) {
        // Material settings.
        glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE,
                     Vector4D(RED_COLOR, 1).Array());
        glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT,
                     Vector4D(RED_COLOR, 1).Array());
        glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,
                     Vector4D(RED_COLOR, 1).Array());
        glColor3f(RED_COLOR);
        glBegin(GL_QUADS);
        // Ship side.
        glVertex3f(shield->translation.x - 1, shield->translation.y - 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x + 1, shield->translation.y - 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x - 1, shield->translation.y + 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x - 1, shield->translation.y - 1,
                   shield->translation.z + 1);
        // Ship-Asteroid side.
        glVertex3f(shield->translation.x - 1, shield->translation.y - 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x - 1, shield->translation.y + 1,
                   shield->translation.z - 1);
        glVertex3f(asteroid->translation.x - 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glVertex3f(asteroid->translation.x - 1, asteroid->translation.y - 1,
                   asteroid->translation.z + 1);
        // Ship-Asteroid side.
        glVertex3f(shield->translation.x - 1, shield->translation.y - 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x - 1, shield->translation.y - 1,
                   shield->translation.z + 1);
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z - 1);
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y - 1,
                   asteroid->translation.z - 1);
        // Ship-Asteroid side.
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glVertex3f(shield->translation.x + 1, shield->translation.y + 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x + 1, shield->translation.y - 1,
                   shield->translation.z + 1);
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z - 1);
        // Ship-Asteroid side.
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glVertex3f(asteroid->translation.x - 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glVertex3f(shield->translation.x - 1, shield->translation.y + 1,
                   shield->translation.z - 1);
        glVertex3f(shield->translation.x + 1, shield->translation.y + 1,
                   shield->translation.z - 1);
        // Asteroid side.
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y + 1,
                   asteroid->translation.z - 1);
        glVertex3f(asteroid->translation.x + 1, asteroid->translation.y - 1,
                   asteroid->translation.z + 1);
        glVertex3f(asteroid->translation.x - 1, asteroid->translation.y + 1,
                   asteroid->translation.z + 1);
        glEnd();
      } else if (iteration - currentIteration > 10) {
        if (object == i + 1)
          object = 0;
        if (cameraIsOnAsteroid == i && cam != shipCam && cam != dynamicCam)
          cam = dynamicCam;
        asteroids.erase(asteroids.begin() + i);
        speed.erase(speed.begin() + i);
        asteroidIsBeingKilled = false;
      }
    }
  }
  int collided = collision();
  if (collided != -1 && !shipDestroyed) {
    if (object == collided + 1)
      object = 0;
    if (cameraIsOnAsteroid == collided)
      cam = shipCam;
    asteroids.erase(asteroids.begin() + collided);
    speed.erase(speed.begin() + collided);
    shield->opacity -= 0.1f;
    glNewList(SHIP_LIST, GL_COMPILE);
    light_left->translation =
        shipCam->Position - ship->dimensions * shipCam->ForwardVector *
                                CAM_SHIP_DISTANCE_MULTIPLIER;
    light_right->translation =
        shipCam->Position - ship->dimensions * shipCam->ForwardVector *
                                CAM_SHIP_DISTANCE_MULTIPLIER;
    shield->translation = shipCam->Position - ship->dimensions *
                                                  shipCam->ForwardVector *
                                                  CAM_SHIP_DISTANCE_MULTIPLIER;
    ship->translation = shipCam->Position -
                        ship->dimensions *
                            (shipCam->ForwardVector + Vector3D(2.2f, 1, 2.2f)) *
                            CAM_SHIP_DISTANCE_MULTIPLIER;
    glTranslatef(-shield->translation.x, -shield->translation.y,
                 -shield->translation.z);
    ship->Draw();
    shield->Draw();
    light_left->Render();
    light_right->Render();
    glEndList();
    if (shield->opacity < 0)
      shipDestroyed = true;
  }

  cam->Render();

  iteration += 1;
}

// Picking at the position clicked by mouse.
void pick(int x, int y) {
  GLuint buffer[1024]; // Selection buffer.
  GLint nhits = 0;     // Number of hits.
  GLint viewport[4];   // Current viewport coordonates.
  glGetIntegerv(GL_VIEWPORT,
                viewport); // Obtaining current viewport coordonates.
  memset(buffer, 0x0, 1024 * sizeof(int)); // Initializing selection buffer.
  glSelectBuffer(1024, buffer);
  glRenderMode(GL_SELECT);     // Entering selection mode.
  glMatrixMode(GL_PROJECTION); // Saving current projection matrix.
  glPushMatrix();
  glLoadIdentity();
  glGetIntegerv(
      GL_VIEWPORT,
      viewport); // Rendering takes place in a zone of [1,1] around the mouse.
  gluPickMatrix(x, viewport[3] - y, 1.0f, 1.0f, viewport);
  gluPerspective(
      45, (viewport[2] - viewport[0]) / (GLfloat)(viewport[3] - viewport[1]),
      Z_NEAR, Z_FAR);
  glMatrixMode(GL_MODELVIEW);
  drawScene(); // Drawing scene in selection buffer, not in frame buffer. It
               // isn't be visible.
  glMatrixMode(GL_PROJECTION); // Restoring initial projection matrix.
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW); // Restoring uzual rendering mode and obtaining
                              // number of hits.
  nhits = glRenderMode(GL_RENDER);
  // Hit processing.
  if (nhits != 0)
    for (unsigned int i = 1023; i >= 0; i -= 1)
      if (buffer[i] > 0) {
        object = buffer[i];
        break;
      } else
        object = 0;
}

// Display function is being called as fast as the video board can.
void display() {
  // Clear screen.
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  // Transparency mechanism with Alpha Testing / Blending.
  glEnable(GL_ALPHA_TEST);
  glAlphaFunc(GL_EQUAL, GL_ONE);
  // Render Pass, draw scene.
  drawScene();
  glAlphaFunc(GL_LESS, GL_ONE);
  glEnable(GL_BLEND);
  glDisable(GL_CULL_FACE);
  glBlendFunc(GL_SRC_ALPHA, GL_DST_ALPHA);
  glDepthMask(GL_FALSE);
  // Render Pass, draw scene.
  drawScene();
  glDepthMask(GL_TRUE);
  glDisable(GL_BLEND);
  glDisable(GL_ALPHA_TEST);

  glutSwapBuffers(); // Because of double buffering.

  glutPostRedisplay(); // Redraw scene.
}

// Keyboard Handler.
void keyboard(unsigned char key, int x, int y) {
  switch (key) {

  // Switching between camera
  case KEY_1:
    asteroidCameraMode = false;
    cam = dynamicCam;
    object = 0;
    cameraIsOnAsteroid = -1;
    break;
  case KEY_2:
    asteroidCameraMode = false;
    cam = shipCam;
    object = 0;
    cameraIsOnAsteroid = -1;
    break;
  case KEY_3:
    asteroidCameraMode = true;
    break;

  // Dynamic camera commands.
  case KEY_J:
  case KEY_J_CAPITAL:
    dynamicCam->MoveForward(SHIP_SPEED);
    break;
  case KEY_M:
  case KEY_M_CAPITAL:
    dynamicCam->MoveBackward(SHIP_SPEED);
    break;
  case KEY_O:
  case KEY_O_CAPITAL:
    dynamicCam->MoveLeft(SHIP_SPEED);
    break;
  case KEY_P:
  case KEY_P_CAPITAL:
    dynamicCam->MoveRight(SHIP_SPEED);
    break;
  case KEY_I:
  case KEY_I_CAPITAL:
    dynamicCam->MoveUpward(SHIP_SPEED);
    break;
  case KEY_K:
  case KEY_K_CAPITAL:
    dynamicCam->MoveDownward(SHIP_SPEED);
    break;
  case KEY_X:
  case KEY_X_CAPITAL:
    asteroidHalt = !asteroidHalt;
    break;

    // Ship commands.

  case KEY_W:
  case KEY_W_CAPITAL:
    if (shield->translation.x < 400)
      shipCam->MoveForward(SHIP_SPEED);
    break;
  case KEY_S:
  case KEY_S_CAPITAL:
    if (shield->translation.x > -400)
      shipCam->MoveBackward(SHIP_SPEED);
    break;
  case KEY_A:
  case KEY_A_CAPITAL:
    if (shield->translation.z > -400)
      shipCam->MoveLeft(SHIP_SPEED);
    break;
  case KEY_D:
  case KEY_D_CAPITAL:
    if (shield->translation.z < 400)
      shipCam->MoveRight(SHIP_SPEED);
    break;
  case KEY_R:
  case KEY_R_CAPITAL:
    if (shield->translation.y < 300)
      shipCam->MoveUpward(SHIP_SPEED);
    break;
  case KEY_F:
  case KEY_F_CAPITAL:
    if (shield->translation.y > -300)
      shipCam->MoveDownward(SHIP_SPEED);
    break;

  case KEY_ESC:
    exit(0);
    break;
  default:
    break;
  }
}

// Mouse callback function for input processing.
void mouse(int button, int state, int x, int y) {
  switch (button) {
  case GLUT_LEFT_BUTTON:
    if (state == GLUT_DOWN) {
      object = 0; // Picked object identifier.
      pick(x, y);
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if (state == GLUT_DOWN)
      break;
    break;
  }
}

// Projection function.
void reshape(int w, int h) {
  glViewport(0, 0, (GLsizei)w, (GLsizei)h); // Main Window.
  GLfloat aspect = (GLfloat)w / (GLfloat)h; // Aspect ratio ( Width / Height )

  // Entering Projection Mode
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  // Loading Perspective Matrix
  gluPerspective(45, aspect, Z_NEAR, Z_FAR);

  // Initializing OpenGL context
  init();
}

int main(int argc, char **argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  int w = 800, h = 600;
  glutInitWindowSize(w, h);
  glutInitWindowPosition(100, 100);

  glutCreateWindow("SpaceEscape 2012"); // Main window.

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutReshapeFunc(reshape);
  glutMouseFunc(mouse);

  initScene(); // Initializing 3D scene.

  glutMainLoop();
  return 0;
}
