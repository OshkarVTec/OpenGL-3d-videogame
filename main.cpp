// g++ main.cpp Meteorito.cpp Shot.cpp Nave.cpp RgbImage.cpp -lGL -lGLU -lglut -o output
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <random>
#include <iomanip>
#include <vector>

#include "Shot.h"
#include "Meteorito.h"
#include "Nave.h"
#include <tuple>
#include <time.h>
#include <algorithm>
#include "math.h"
#include "RgbImage.h"
using namespace std;

bool *keyStates = new bool[256];

// Variables dimensiones de la pantalla
int WIDTH = 500;
int HEIGTH = 500;
// Variables para establecer los valores de gluPerspective
float FOVY = 60.0;
float ZNEAR = 0.01;
float ZFAR = 1000.0;
// Variables para definir la posicion del observador
// gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X = 0.0;
float EYE_Y = 200.0;
float EYE_Z = 500.0;
float CENTER_X = 0;
float CENTER_Y = 0;
float CENTER_Z = 0;
float UP_X = 0;
float UP_Y = 1;
float UP_Z = 0;
// Variables para dibujar los ejes del sistema
float X_MIN = -500;
float X_MAX = 500;
float Y_MIN = -500;
float Y_MAX = 500;
float Z_MIN = -500;
float Z_MAX = 500;
// Size del tablero
int DimBoard = 200;
// Direccion de nave
int dir = 2;
// Direccion de nave enemiga
int dirEnemy = 2;
// Bandera de fin de juego
bool gameOver = false;
bool win = false;

// timers
time_t disparoAnterior = time(NULL);
time_t disparoActual;
time_t disparoEnemyActual;
time_t disparoEnemyAnterior = time(NULL);
time_t delta;

time_t meteoritosActuales;
time_t meteoritosAnteriores = time(NULL);

time_t start = time(NULL);
time_t current;

char *filename0 = "img/meteoritoTexture.bmp";
char *filename1 = "img/naveTexture.bmp";
char *filename2 = "textura2.bmp";
char *filename3 = "textura3.bmp";
char *filename4 = "textura4.bmp";
char *filename5 = "textura5.bmp";

bool trigger = false;
float health = 1.0f;

#define NObjetos 10

vector<void *> naves;
vector<void *> shots;
vector<void *> shotsEnemy;
vector<void *> meteoritos;

int i;

// se define la cantidad de texturas que se manejaran
#define NTextures 6
GLuint texture[NTextures];

// Read a texture map from a BMP bitmap file.
void loadTextureFromFile(char *filename, int index)
{
  glClearColor(0.0, 0.0, 0.0, 0.0);
  glShadeModel(GL_FLAT);
  // glEnable(GL_DEPTH_TEST);

  RgbImage theTexMap(filename);

  // generate an OpenGL texture ID for this texture
  glGenTextures(1, &texture[index]);
  // bind to the new texture ID
  glBindTexture(GL_TEXTURE_2D, texture[index]);

  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexImage2D(GL_TEXTURE_2D, 0, 3, theTexMap.GetNumCols(), theTexMap.GetNumRows(), 0,
               GL_RGB, GL_UNSIGNED_BYTE, theTexMap.ImageData());
  theTexMap.Reset();
}

void drawAxis()
{
  glLineWidth(3.0);
  // X axis in red
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3f(X_MIN, 0.0, 0.0);
  glVertex3f(X_MAX, 0.0, 0.0);
  glEnd();
  // Y axis in green
  glColor3f(0.0f, 1.0f, 0.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0, Y_MIN, 0.0);
  glVertex3f(0.0, Y_MAX, 0.0);
  glEnd();
  // Z axis in blue
  glColor3f(0.0f, 0.0f, 1.0f);
  glBegin(GL_LINES);
  glVertex3f(0.0, 0.0, Z_MIN);
  glVertex3f(0.0, 0.0, Z_MAX);
  glEnd();
  glLineWidth(1.0);
}

void init()
{
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(FOVY, (GLfloat)WIDTH / HEIGTH, ZNEAR, ZFAR);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(EYE_X, EYE_Y, EYE_Z, CENTER_X, CENTER_Y, CENTER_Z, UP_X, UP_Y, UP_Z);
  glClearColor(0, 0, 0, 0);
  glEnable(GL_DEPTH_TEST);
  srand(time(nullptr));
  naves.push_back(new Nave(DimBoard, 0.3, 0, 10, 200));
  naves.push_back(new Nave(DimBoard, 0.3, 0, 10, -200));
  loadTextureFromFile(filename0, 0);
  loadTextureFromFile(filename1, 1);
  loadTextureFromFile(filename2, 2);
  loadTextureFromFile(filename3, 3);
  loadTextureFromFile(filename4, 4);
  loadTextureFromFile(filename5, 5);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
}

bool checkCollision(float r1, float r2, tuple<float, float, float> const &p1, tuple<float, float, float> const &p2)
{
  // cout << get<0>(p1) << " " << get<0>(p2) << endl;
  float dist = sqrt(pow(get<0>(p1) - get<0>(p2), 2) + pow(get<1>(p1) - get<1>(p2), 2) + pow(get<2>(p1) - get<2>(p2), 2));
  dist -= (r1 + r2);
  // cout << dist << endl;
  bool c = false;
  if (dist <= 0)
    c = true;
  // cout << c << endl;
  return c;
}

void keyOperations(void)
{
  if (keyStates['a'] || keyStates['A'])
    dir = 0;
  else if (keyStates['d'] || keyStates['D'])
    dir = 1;
  else
    dir = 2;

  if (keyStates['w'] || keyStates['W'])
    trigger = true;
}

void fire(tuple<float, float, float> pos)
{
  shots.push_back(new Shot(DimBoard, 0.3, get<0>(pos), get<1>(pos), get<2>(pos), 5, -10, false));
  cout << "fire" << endl;
  disparoAnterior = time(NULL);
}

void fireEnemy(tuple<float, float, float> pos)
{
  shotsEnemy.push_back(new Shot(DimBoard, 0.3, get<0>(pos), get<1>(pos), get<2>(pos), 5, 10, true));
  cout << "fireEnemy" << endl;
}

void lineaMeteoritos()
{
  double probability = 0.5;
  for (i = -200; i <= 200; i += 80)
  {
    double randomNumber = (double)rand() / RAND_MAX;
    if (randomNumber < probability)
    {
      meteoritos.push_back(new Meteorito(DimBoard, 4, i, 10, -800, 30));
    }
  }
  meteoritosAnteriores = time(NULL);
}

// Function to display text on the screen
void drawString(string str, int x, int y)
{
  glRasterPos2i(x, y);
  for (int i = 0; i < str.length(); i++)
  {
    glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, str[i]);
  }
}

// From GeeksForGeeks
float minimax(int depth, int nodeIndex, bool isMax,
              vector<float> &scores, int h)
{
  // Terminating condition. i.e
  // leaf node is reached
  if (depth == h)
    return scores[nodeIndex];

  //  If current move is maximizer,
  // find the maximum attainable
  // value
  if (isMax)
  {
    // cout << "max " << nodeIndex*2 << ":"  << scores[nodeIndex*2] << " " << nodeIndex*2+1 <<  ":" << scores[nodeIndex*2+1] << endl;
    return max(minimax(depth + 1, nodeIndex * 2, false, scores, h),
               minimax(depth + 1, nodeIndex * 2 + 1, false, scores, h));
  }
  // Else (If current move is Minimizer), find the minimum
  // attainable value
  else
  {
    // cout << "min " << nodeIndex*2 << ":" << scores[nodeIndex*2] << " " << nodeIndex*2+1 <<  ":" << scores[nodeIndex*2+1] << endl;
    return min(minimax(depth + 1, nodeIndex * 2, true, scores, h),
               minimax(depth + 1, nodeIndex * 2 + 1, true, scores, h));
  }
}

int minimaxStarter(int depth, int nodeIndex,
                   vector<float> &scores, int h)
{
  float a = minimax(depth + 1, nodeIndex * 2, false, scores, h);
  float b = minimax(depth + 1, nodeIndex * 2 + 1, false, scores, h);
  if (a > b)
    return 0;
  else
    return 1;
}

float f(tuple<float, float, float> posCurrentNave, vector<void *> &currentShots, tuple<float, float, float> posContraNave)
{
  Shot *auxS;
  float res = 0;
  tuple<float, float, float> posCurrentShot;
  for (int j = 0; j < currentShots.size(); j++)
  {
    auxS = (Shot *)shots[j];
    posCurrentShot = auxS->getPos();
    res += 2 * sqrt(pow(get<0>(posCurrentNave) - get<0>(posCurrentShot), 2) + pow(get<1>(posCurrentNave) - get<1>(posCurrentShot), 2) + pow(get<2>(posCurrentNave) - get<2>(posCurrentShot), 2));
  }
  res -= abs(get<0>(posContraNave) - get<0>(posCurrentNave));
  // cout << res << endl;
  return res;
}

void buildTree(vector<float> &scores, int depth, int h, tuple<float, float, float> posEnemy, tuple<float, float, float> posNave)
{
  if (depth == h)
  {
    scores.push_back(f(posEnemy, shots, posNave));
    return;
  }
  tuple<float, float, float> newPos;
  if (depth % 2 == 0)
  {
    newPos = make_tuple(get<0>(posEnemy) - 10 > -200 ? get<0>(posEnemy) - 10 : -200, get<1>(posEnemy), get<2>(posEnemy));
    buildTree(scores, depth + 1, h, newPos, posNave);
    newPos = make_tuple(get<0>(posEnemy) + 10 < 200 ? get<0>(posEnemy) + 10 : 200, get<1>(posEnemy), get<2>(posEnemy));
    buildTree(scores, depth + 1, h, newPos, posNave);
  }
  else
  {
    newPos = make_tuple(get<0>(posNave) - 10 > -200 ? get<0>(posNave) - 10 : -200, get<1>(posNave), get<2>(posNave));
    buildTree(scores, depth + 1, h, posEnemy, newPos);
    newPos = make_tuple(get<0>(posNave) + 10 < 200 ? get<0>(posNave) + 10 : 200, get<1>(posNave), get<2>(posNave));
    buildTree(scores, depth + 1, h, posEnemy, newPos);
  }
  return;
}

void normalGame()
{
  float r1, r2;
  tuple<float, float, float> pos;
  tuple<float, float, float> posNave;
  Nave *nave;
  Shot *auxS;
  Meteorito *auxM;
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // drawAxis();
  glColor3f(0.3, 0.3, 0.3);

   // Enable texturing and bind the texture
  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  // Se dibuja la nave
  nave = (Nave *)naves[0];
  nave->draw();
  nave->update(dir);

  glBindTexture(GL_TEXTURE_2D, texture[0]);
  // Se dibujan los meteoritos
  meteoritosActuales = time(NULL);
  delta = meteoritosActuales - meteoritosAnteriores;
  if (delta > 0.5)
    lineaMeteoritos();
  for (int j = 0; j < meteoritos.size(); j++)
  {
    auxM = (Meteorito *)meteoritos[j];
    auxM->draw();
    auxM->update();
    pos = auxM->getPos();
    if (get<2>(pos) > 200)
    {
      meteoritos.erase(meteoritos.begin() + j);
      break;
    }
    // Colision con nave
    if (checkCollision(nave->getRadio(), auxM->getRadio(), nave->getPos(), pos))
    {
      gameOver = true;
    }
  }
  glDisable(GL_TEXTURE_2D);

  // Se dibujan los disparos
  for (int j = 0; j < shots.size(); j++)
  {
    auxS = (Shot *)shots[j];
    auxS->draw();
    auxS->update();
    pos = auxS->getPos();
    if (get<2>(pos) < -800)
    {
      shots.erase(shots.begin() + j);
      break;
    }
    // Colision meteorito-disparo
    for (int k = 0; k < meteoritos.size(); k++)
    {
      auxM = (Meteorito *)meteoritos[k];
      if (checkCollision(auxS->getRadio(), auxM->getRadio(), pos, auxM->getPos()))
      {
        meteoritos.erase(meteoritos.begin() + k);
        shots.erase(shots.begin() + j);
        goto after;
      }
    }
  }
after:
  // Disparo
  if (trigger)
  {
    disparoActual = time(NULL);
    delta = disparoActual - disparoAnterior;
    if (delta > 1)
    {
      fire(nave->getPos());
    }
    trigger = false;
  }
}

void boss()
{
  tuple<float, float, float> pos;
  tuple<float, float, float> posNave;
  // Arbol de juego
  vector<float> scores;
  Shot *auxS;
  Meteorito *auxM;
  Nave *nave, *enemy;
  keyOperations();
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  // drawAxis();
  glColor3f(0.3, 0.3, 0.3);

  glEnable(GL_TEXTURE_2D);
  glBindTexture(GL_TEXTURE_2D, texture[1]);
  // Se dibuja la nave
  nave = (Nave *)naves[0];
  nave->draw();
  nave->update(dir);

  // Se dibuja la nave enemiga
  enemy = (Nave *)naves[1];
  enemy->draw();
  // dirEnemy = minimaxStarter(0,0, scores, 5);// A simple C++ program to find
  enemy->update(dirEnemy);

  scores.clear();
  buildTree(scores, 0, 3, enemy->getPos(), nave->getPos());
  dirEnemy = minimaxStarter(0, 0, scores, 3);

  glBindTexture(GL_TEXTURE_2D, texture[0]);
  // Se dibujan los meteoritos
  for (int j = 0; j < meteoritos.size(); j++)
  {
    auxM = (Meteorito *)meteoritos[j];
    auxM->draw();
    auxM->update();
    pos = auxM->getPos();
    if (get<2>(pos) > 210)
    {
      meteoritos.erase(meteoritos.begin() + j);
      break;
    }
    // Colision con nave
    if (checkCollision(nave->getRadio(), auxM->getRadio(), nave->getPos(), pos))
    {
      gameOver = true;
    }
  }
  glDisable(GL_TEXTURE_2D);

  // Se dibujan los disparos
  for (int j = 0; j < shots.size(); j++)
  {
    auxS = (Shot *)shots[j];
    auxS->draw();
    auxS->update();
    pos = auxS->getPos();
    if (get<2>(pos) < -210)
    {
      shots.erase(shots.begin() + j);
      break;
    }
    // Colision disparo-enemy
    if (checkCollision(enemy->getRadio(), auxS->getRadio(), enemy->getPos(), pos))
    {
      win = true;
    }
    // Colision meteorito-disparo
    for (int k = 0; k < meteoritos.size(); k++)
    {
      auxM = (Meteorito *)meteoritos[k];
      if (checkCollision(auxS->getRadio(), auxM->getRadio(), pos, auxM->getPos()))
      {
        meteoritos.erase(meteoritos.begin() + k);
        shots.erase(shots.begin() + j);
        goto afterB;
      }
    }
  }
afterB:

  // Se dibujan los disparos enemigos
  for (int j = 0; j < shotsEnemy.size(); j++)
  {
    auxS = (Shot *)shotsEnemy[j];
    auxS->draw();
    auxS->update();
    pos = auxS->getPos();
    if (get<2>(pos) > 210)
    {
      shotsEnemy.erase(shotsEnemy.begin() + j);
      break;
    }
    // Colision disparo-nave
    if (checkCollision(nave->getRadio(), auxS->getRadio(), nave->getPos(), pos))
    {
      gameOver = true;
    }
  }
  // Disparo
  if (trigger)
  {
    disparoActual = time(NULL);
    delta = disparoActual - disparoAnterior;
    if (delta > 1)
    {
      fire(nave->getPos());
    }
    trigger = false;
  }
  disparoEnemyActual = time(NULL);
  delta = disparoEnemyActual - disparoEnemyAnterior;
  if (delta > 0.8)
  {
    fireEnemy(enemy->getPos());
    disparoEnemyAnterior = time(NULL);
  }
}

void display()
{
  if (!gameOver && !win)
  {
    current = time(NULL);
    delta = current - start;
    if (delta < 20)
      normalGame();
    else
      boss();
  }
  else if (win)
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);

    // Display game over message
    glColor3f(1.0f, 0.0f, 0.0f);      // Set color to red
    drawString("You win!", -60, 100); // Display text at position (100, 100)

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  else
  {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // Disable depth testing
    glDisable(GL_DEPTH_TEST);

    // Display game over message
    glColor3f(1.0f, 0.0f, 0.0f);       // Set color to red
    drawString("Game Over", -60, 100); // Display text at position (100, 100)

    // Enable depth testing
    glEnable(GL_DEPTH_TEST);
  }
  glutSwapBuffers();
}

void idle()
{
  display();
}

void keyboard(unsigned char key, int x, int y)
{
  keyStates[key] = true;
  if (key == 27) // escape
    exit(0);
}

void keyUp(unsigned char key, int x, int y)
{
  keyStates[key] = false;
}

int main(int argc, char **argv)
{
  srand(time(NULL));
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(100, 100);
  glutInitWindowSize(WIDTH, HEIGTH);
  glutCreateWindow("Space war");
  init();
  glutDisplayFunc(display);
  glutIdleFunc(idle);
  glutKeyboardFunc(keyboard);
  glutKeyboardUpFunc(keyUp);
  glutMainLoop();
  return 0;
}