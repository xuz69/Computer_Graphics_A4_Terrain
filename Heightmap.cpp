#include "Heightmap.hpp"
#include <cstdlib>
#include <cmath>

double PI = 3.14159265;

Heightmap::Heightmap(vector<vector<Point3D> > & two_d_array_points, vector<vector<float> > & grayscaleColor, int in_size){
    this->size = in_size;
    vector<Point3D> points;
    vector<float> colors;
    float max_height=0;
    float min_height=1000;
    /* initialized points with height 0 */
    for(int x = 0; x < this->size; x ++){
        points.clear();
        colors.clear();
        for(int z = 0; z < this->size; z++){
            points.push_back(Point3D(x-this->size/2,0,z-this->size/2));
            colors.push_back(0.0);
        }
        two_d_array_points.push_back(points);
        grayscaleColor.push_back(colors);
    }
    
    /* random number of circles */
    int number_circle = in_size/2;
    /* circles algorithm
     *
     * for each terrain point (tx,tz) do
     *     pd = distance from circle center * 2 / terrainCircleSize
     *     if fabs(pd) <= 1.0
     *          height(tx,tz) +=  disp/2 + cos(pd*3.14)*disp/2;
     *
     */
    srand((int)time(NULL));
    for(int i = 0; i < number_circle; i++){
        int c_x = rand() % (this->size) - this->size/2;
        int c_z = rand() % (this->size) - this->size/2;
        int disp = rand() % 30 - 15;
        int c_size = rand() % 20 + 10;
        
        for(int j = 0; j < this->size; j++){
            for(int z = 0; z < this->size; z++){
                float pd = sqrt(pow(two_d_array_points[j][z].mX - c_x,2) + pow(two_d_array_points[j][z].mZ - c_z,2))*2 / c_size;
                if(pd <= 1.0){
                    two_d_array_points[j][z].mY += disp/2+cos(pd*PI)*disp/2;
                }
                /* find max height and min height */
                if(two_d_array_points[j][z].mY > max_height){
                    max_height = two_d_array_points[j][z].mY;
                }
                if(two_d_array_points[j][z].mY < min_height){
                    min_height = two_d_array_points[j][z].mY;
                }
                /* set Gray Scale Color */
                float scale = (two_d_array_points[j][z].mY - min_height)/(max_height - min_height);
                grayscaleColor[j][z] = scale;
            }
        }
    }
    
}
