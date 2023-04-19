#include "Meteorito.h"
#include "math.h"

Meteorito::Meteorito(int dim, float vel,float x, float y, float z, float radio)
{
    DimBoard = dim;
    //srand(time(nullptr));
    int c;

    //Se inicializa una posicion aleatoria dentro del tablero
    Position[0] = x;
    Position[1] = y;
    Position[2] = z;

    //Se inicializa el vector de direccion con un valor aleatorio
    Direction[0] = rand();
    Direction[1] = rand();
    Direction[2] = rand();

    //Se normaliza el vector de direccion
    float m = sqrt(Direction[0]*Direction[0] + Direction[1]*Direction[1] + Direction[2]*Direction[2]);
    Direction[0] /= m;
    Direction[1] /= m;
    Direction[2] /= m;
    //se multiplica el vector de direccion por la magnitud de la velocidad
    Direction[0] *= vel;
    Direction[1] *= vel;
    Direction[2] *= vel;

    //cal. del radio Meteorito
    this->radio = radio;
    cout << Direction[0] << endl;
    cout << Direction[2] << endl; 
    collision = false;
}

Meteorito::~Meteorito()
{
    //dtor
}

void Meteorito::draw()
{
    glPushMatrix();
    glTranslatef(Position[0], Position[1], Position[2]);
    glColor3f(1.0f, 0.0f, 0.0f);
    glutSolidSphere(radio, 30, 30);
    glPopMatrix();
}

void Meteorito::update()
{
        if(!collision){
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
    }
}

float Meteorito::getRadio(){
   return radio;
}

tuple<float, float, float> Meteorito::getPos(){
   return make_tuple(Position[0], Position[1], Position[2]);
}

void Meteorito::collided(){
   collision = true;
}