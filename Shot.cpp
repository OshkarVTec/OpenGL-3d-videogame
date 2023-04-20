#include "Shot.h"
#include "math.h"

Shot::Shot(int dim, float vel,float x, float y, float z, float height)
{
    DimBoard = dim;
    //srand(time(nullptr));
    int c;

    //Se inicializa una posicion 
    Position[0] = x;
    Position[1] = y;
    Position[2] = z;


    //se multiplica el vector de direccion por la magnitud de la velocidad
    Direction[0] *= vel;
    Direction[1] *= vel;
    Direction[2] *= vel;

    //cal. del radio Shot
    this->height = height;
    radio = height / 2;
    //cout << Direction[0] << endl;
    //cout << Direction[2] << endl; 
    collision = false;
}

Shot::~Shot()
{
    //dtor
}

void Shot::draw()
{
    glPushMatrix();
    glTranslatef(Position[0],Position[1], Position[2]); 
    glRotatef(90.0f, 0.0f, 0.0f, 1.0f); 
    glColor3f(0.0f, 1.0f, 0.0f);
    GLUquadricObj* quad = gluNewQuadric();
    gluQuadricNormals(quad, GLU_SMOOTH);
    gluCylinder(quad, 5, 5, height, 30, 30);

    // Draw top disk
    gluQuadricOrientation(quad, GLU_INSIDE);
    gluDisk(quad, 0.0, 5, 30, 1);
    gluQuadricOrientation(quad, GLU_OUTSIDE);

    // Draw bottom disk
    glTranslatef(0.0, 0.0, height);
    gluQuadricOrientation(quad, GLU_INSIDE);
    gluDisk(quad, 0.0, 5, 30, 1);
    gluQuadricOrientation(quad, GLU_OUTSIDE);

    gluDeleteQuadric(quad);
    glPopMatrix();
}

void Shot::update()
{
        if(!collision){
            float new_z = Position[2] + Direction[2];
            Position[2] = new_z;

        //cout << "X=" << Position[0] << "; Y= " << Position[2] << endl;
    }
}

float Shot::getRadio(){
   return radio;
}

tuple<float, float, float> Shot::getPos(){
   return make_tuple(Position[0], Position[1], Position[2]);
}

void Shot::collided(){
   collision = true;
}