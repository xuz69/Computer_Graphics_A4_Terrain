#ifndef Snowman_hpp
#define Snowman_hpp

#include <stdio.h>
#include "mathLib3D.hpp"
#include <vector>

using namespace std;


class Snowman{
public:
    Snowman(vector<vector<Point3D> > points);
    int size;
    float speed;
    Vec3D direction;
    Point3D position;
    void drawSnowman();
};

#endif