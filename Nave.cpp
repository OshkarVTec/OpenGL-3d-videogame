#include "Nave.h"
#include "math.h"

Nave::Nave(int dim, float vel, float x, float y, float z)
{
    DimBoard = dim;
    //srand(time(nullptr));
    int c;
    Position[0] = x;
    Position[1] = y;
    Position[2] = z;
    Direction[2] *= vel;

    //cal. del radio cubo
    radio = sqrt(3*10*10);
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
    glColor3f(0.9f, 0.9f, 0.9f);
    glEnableClientState(GL_VERTEX_ARRAY);
    glEnableClientState(GL_TEXTURE_COORD_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, vertexCoords);
    glTexCoordPointer(2, GL_FLOAT, 0, texCoords);
    glDrawElements(GL_QUADS,24,GL_UNSIGNED_INT,elementArray);
    glDisableClientState(GL_VERTEX_ARRAY);
    glDisableClientState(GL_TEXTURE_COORD_ARRAY);

    glPopMatrix();
}

void Nave::update(int dir) //0-left 1-right 2-stop
{
        switch (dir){
            case 0: Direction[0] = -5; break;
            case 1: Direction[0] = 5; break;
            case 2: Direction[0] = 0; break;
        }
        if(!collision){
         float new_x = Position[0] + Direction[0];
         float new_z = Position[2] + Direction[2];

         //cout << "X=" << Position[0] << "; Y= " << Position[2] << endl;

         if (abs(new_x) <= DimBoard)
               Position[0] = new_x;


        //cout << "X=" << Position[0] << "; Y= " << Position[2] << endl;
    }
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