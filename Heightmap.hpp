#ifndef Heightmap_hpp
#define Heightmap_hpp

#include <stdio.h>
#include <vector>
#include "mathLib3D.hpp"

using namespace std;

class Heightmap{
public:
    Heightmap(vector<vector<Point3D> > & two_d_array_points, vector<vector<float> > & grayscaleColor, int in_size);
    int size;
};

#endif
