#include "Heightmap.hpp"
#include <cstdlib>

double PI = 3.14159265;

Heightmap::Heightmap(vector<vector<Point3D> > & two_d_array_points, int in_size){
    this->size = in_size;
    vector<Point3D> points;
    /* initialized points with height 0 */
    for(int x = 0; x < this->size; x ++){
        points.clear();
        for(int z = 0; z < this->size; z++){
            points.push_back(Point3D(x,0,z));
        }
        two_d_array_points.push_back(points);
    }

    /* random number of circles */
    int number_circle = in_size/3;
    for(int i = 0; i < number_circle; i++){
        int c_x = rand() % (this->size-1);
        int c_y = rand() % 15 + 1;
        int c_z = rand() % (this->size-1);
        int c_size = rand() % (this->size/2) + 1;
    }

}