#ifndef Snowman_hpp
#define Snowman_hpp

#include <stdio.h>
#include "mathLib3D.hpp"
#include <vector>

using namespace std;


class Snowman{
public:
    Snowman(vector<vector<Point3D> > points);
    int size; //size of the snowman
    float speed; //moving speed of the snowman
    Vec3D direction; //moving direction of the snowman
    Point3D position; //position of the snowman
    void drawSnowman(); //function to draw the snowman
};

#endif