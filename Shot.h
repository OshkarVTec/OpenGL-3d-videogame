#ifndef SHOT_H
#define SHOT_H

#include <math.h>
#include <random>
#include <iomanip>
#include <GL/glut.h>
#include <iostream>
#include <tuple>


using namespace std;

class Shot
{
    public:
        Shot(int, float, float, float, float, float, float, bool);
        ~Shot();
        void draw();
        void update();
        tuple<float,float,float> getPos();
        float getRadio();
        void collided();

    protected:

    private:
        //Vector de direccion
        float Position[3];
        float Direction[3] = {0,0,0};
        int DimBoard;
        float radio;
        bool collision;
        float height;
        bool enemy;
};

#endif // Shot_H
