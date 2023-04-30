#ifndef NAVE_H
#define NAVE_H

#include <math.h>
#include <random>
#include <iomanip>
#include <GL/glut.h>
#include <iostream>
#include <tuple>


using namespace std;

class Nave
{
    public:
        Nave(int, float, float, float, float);
        ~Nave();
        void draw();
        void update(int);
        tuple<float,float,float> getPos();
        float getRadio();
        void collided();

    protected:

    private:
        float vertexCoords[48] =  {-1.0f, 0.0f, 1.0f,   1.0f, 0.0f, 1.0f,   1.0f, 1.0f, 1.0f,  -1.0f, 1.0f, 1.0f,
  -1.0f, 1.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 0.0f, -1.0f, -1.0f, 0.0f, -1.0f,
  1.0f, 0.0f, 1.0f,   1.0f, 0.0f, -1.0f,  1.0f, 1.0f, -1.0f,  1.0f, 1.0f, 1.0f,
  -1.0f, 0.0f, -1.0f,  -1.0f, 0.0f, 1.0f,  -1.0f, 1.0f, 1.0f, -1.0f, 1.0f, -1.0f
  };

        float vertexColors[24] = {  // An RGB color value for each vertex
                   1,1,1,   1,0,0,   1,1,0,   0,1,0,
                   0,0,1,   1,0,1,   0,0,0,   0,1,1  };

        int elementArray[24] = {0,1,2,3, 4,5,6,7, 3,2,5,4, 7,6,1,0,
                                  8,9,10,11, 12,13,14,15};

        float texCoords[32] = { 0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0,
                               0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0,
                               0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0,
                               0.0,0.0, 1.0,0.0, 1.0,1.0, 0.0,1.0
                             };

        //Vector de direccion
        float Position[3];
        float Direction[3];
        int DimBoard;
        float radio;
        bool collision;
};

#endif // NAVE_H
