#include "Nave.h"
#include "math.h"

Nave::Nave(int dim, float vel)
{
    DimBoard = dim;
    //srand(time(nullptr));
    int c;


    //Se normaliza el vector de direccion
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;
    //se multiplica el vector de direccion por la magnitud de la velocidad
    Direction[0] *= vel;
    Direction[1] *= vel;
    Direction[2] *= vel;

    //cal. del radio cubo
    radio = sqrt(3*5*5);
    /* cout << Direction[0] << endl;
    cout << Direction[2] << endl; */
    collision = false;
}

Nave::~Nave()
{
    //dtor
}

void Nave::draw()
{
    glPushMatrix();
    glTranslatef(Position[0], Position[1], Position[2]);
    glScaled(10,10,10);
    //Se dibuja el cubo
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_COLOR_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexCoords);
    glColorPointer(3, GL_FLOAT, 0, vertexColors);
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,elementArray);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_COLOR_ARRAY);

    glPopMatrix();
}

void Nave::update()
{
/*         if(!collision){
         float new_x = Position[0] + Direction[0];
         float new_z = Position[2] + Direction[2];

         //cout << "X=" << Position[0] << "; Y= " << Position[2] << endl;

         if (abs(new_x) <= DimBoard)
               Position[0] = new_x;
         else {
               Direction[0] *= -1.0;
               Position[0] += Direction[0];
         }

         if (abs(new_z) <= DimBoard)
               Position[2] = new_z;
         else {
               Direction[2] *= -1.0;
               Position[2] += Direction[2];
         }

        //cout << "X=" << Position[0] << "; Y= " << Position[2] << endl;
    } */
}

float Nave::getRadio(){
   return radio;
}

tuple<float, float, float> Nave::getPos(){
   return make_tuple(Position[0], Position[1], Position[2]);
}

void Nave::collided(){
   collision = true;
}