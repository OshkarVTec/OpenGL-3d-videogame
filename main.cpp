//g++ main.cpp Meteorito.cpp Shot.cpp Nave.cpp -lGL -lGLU -lglut -o output
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
using namespace std;

bool* keyStates = new bool[256]; 

//Variables dimensiones de la pantalla
int WIDTH=500;
int HEIGTH=500;
//Variables para establecer los valores de gluPerspective
float FOVY=60.0;
float ZNEAR=0.01;
float ZFAR=1000.0;
//Variables para definir la posicion del observador
//gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z)
float EYE_X=0.0;
float EYE_Y=200.0;
float EYE_Z=500.0;
float CENTER_X=0;
float CENTER_Y=0;
float CENTER_Z=0;
float UP_X=0;
float UP_Y=1;
float UP_Z=0;
//Variables para dibujar los ejes del sistema
float X_MIN=-500;
float X_MAX=500;
float Y_MIN=-500;
float Y_MAX=500;
float Z_MIN=-500;
float Z_MAX=500;
//Size del tablero
int DimBoard = 200;
//Direccion de nave
int dir = 2;

//timers
time_t disparoAnterior = time(NULL);
time_t disparoActual;
time_t delta;

time_t meteoritosActuales;
time_t meteoritosAnteriores = time(NULL);



bool trigger = false;


#define NObjetos 10


vector<int> activeShots;
vector<int> activeMeteoritos;

void *objetos[NObjetos];
void *shots[NObjetos];
void *meteoritos[20];
int i;

void drawAxis()
{
     glLineWidth(3.0);
     //X axis in red
     glColor3f(1.0f,0.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(X_MIN,0.0,0.0);
       glVertex3f(X_MAX,0.0,0.0);
     glEnd();
     //Y axis in green
     glColor3f(0.0f,1.0f,0.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,Y_MIN,0.0);
       glVertex3f(0.0,Y_MAX,0.0);
     glEnd();
     //Z axis in blue
     glColor3f(0.0f,0.0f,1.0f);
     glBegin(GL_LINES);
       glVertex3f(0.0,0.0,Z_MIN);
       glVertex3f(0.0,0.0,Z_MAX);
     glEnd();
     glLineWidth(1.0);
 }

 void init()
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(FOVY, (GLfloat)WIDTH/HEIGTH, ZNEAR, ZFAR);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(EYE_X,EYE_Y,EYE_Z,CENTER_X,CENTER_Y,CENTER_Z,UP_X,UP_Y,UP_Z);
    glClearColor(0,0,0,0);
    glEnable(GL_DEPTH_TEST);
    srand(time(nullptr));


    for(i = 0; i < 1; i++)
        objetos[i] = new Nave(DimBoard, 0.3);

}

bool checkCollision(float r1, float r2, tuple<float,float,float> const &p1 , tuple<float,float,float> const &p2){
  //cout << get<0>(p1) << " " << get<0>(p2) << endl;
  float dist = sqrt(pow(get<0>(p1)-get<0>(p2),2)+pow(get<1>(p1)-get<1>(p2),2)+pow(get<2>(p1)-get<2>(p2),2));
  //cout << dist << endl;
  dist -= (r1+r2);
  bool c;
  if (dist <= 0) c = true;
  else c = false;
  return c;
}

void keyOperations (void) {  
  if (keyStates['a'] || keyStates['A']) dir = 0;
  else if (keyStates['d'] || keyStates['D']) dir = 1;
  else  dir = 2;

  if(keyStates['w'] || keyStates['W']) trigger = true;
}  

void fire(tuple<float,float,float> pos){
  for (i = 0; i < 10; i++){
    if (find(activeShots.begin(), activeShots.end(), i) == activeShots.end()){
      activeShots.push_back(i);
      shots[i] = new Shot(DimBoard, 0.3, get<0>(pos), get<1>(pos),get<2>(pos), 5);
      break;
    }
  }
  cout << "fire" << endl;
  disparoAnterior = time(NULL);
}

void lineaMeteoritos(){
  for (i = 0; i < 20; i++){
    if (find(activeMeteoritos.begin(), activeMeteoritos.end(), i) == activeMeteoritos.end()){
      activeMeteoritos.push_back(i);
      meteoritos[i] = new Meteorito(DimBoard, 2, 0, 10, -800, 30);
      break;
    }
  }
  meteoritosAnteriores = time(NULL);
}

void display()
{   
    float r1, r2;
    tuple<float,float,float> pos;
    Nave *aux;
    Shot *auxS;
    Meteorito *auxM;
    keyOperations();  
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    //drawAxis();
    glColor3f(0.3, 0.3, 0.3);

    //El piso es dibujado
    glBegin(GL_QUADS);
        glVertex3d(-DimBoard, 0.0, -DimBoard);
        glVertex3d(-DimBoard, 0.0, DimBoard);
        glVertex3d(DimBoard, 0.0, DimBoard);
        glVertex3d(DimBoard, 0.0, -DimBoard);
    glEnd();

    //Se dibujan los meteoritos
    meteoritosActuales = time(NULL);
    delta = meteoritosActuales - meteoritosAnteriores;
    if(delta > 5) lineaMeteoritos();
    for(auto j : activeMeteoritos){
      auxM = (Meteorito *)meteoritos[j];
      auxM->draw();
      auxM->update();
      pos = auxM->getPos();
      if (get<2>(pos )> 200){
        remove(activeMeteoritos.begin(),activeMeteoritos.end(),j);
      }
    }

    //Se dibuja la nave
    aux = (Nave *)objetos[0];
    aux->draw();
    aux->update(dir);

    //Se dibujan los disparos
    for(auto j : activeShots){
      auxS = (Shot *)shots[j];
      auxS->draw();
      auxS->update();
      pos = auxS->getPos();
      if (get<2>(pos)<-200){
        remove(activeShots.begin(),activeShots.end(),j);
      }
    }

    //Disparo
    if(trigger){
      disparoActual = time(NULL);
      delta = disparoActual - disparoAnterior;
      if(delta > 1){
        pos = aux->getPos();
        fire(pos);
      }
      trigger = false;
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
    if (key == 27)  // escape
      exit(0);
}

void keyUp (unsigned char key, int x, int y) {  
  keyStates[key] = false;
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH );
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