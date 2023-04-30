#ifndef METEORITO_H
#define METEORITO_H

#include <math.h>
#include <random>
#include <iomanip>
#include <GL/glut.h>
#include <iostream>
#include <tuple>


using namespace std;

class Meteorito
{
    public:
        Meteorito(float, float, float, float, float);
        ~Meteorito();
        void draw();
        void update();
        tuple<float,float,float> getPos();
        float getRadio();

    protected:

    private:
        //Vector de direccion
        float Position[3];
        float Direction[3] = {0.0, 0.0, 1.0};
        float radio;
};

#endif // Meteorito_H
