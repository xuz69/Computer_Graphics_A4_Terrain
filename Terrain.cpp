#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <vector>
#include <math.h>
#include "Heightmap.hpp"
#include "Snowman.hpp"

/*
 * Description of some global variables
 * 
 * terrain_points: stores all the 3d points of the Terrain
 * 
 * points_color: Grey scale color for each points of the Terrain
 * 
 * character: stores list of character
 * 
 * terrain_size: size of the Terrain (ex. 50x50 ~ 300x300)
 * 
 * wireFrame: used to control the wireFrame mode between polygon, wireFrame and poly-wireframe
 * 
 * texture: control the texture modes
 * 
 * is_texture: texturing mode is on or not
 * 
 * is_zero: if true, light0 is selected; else, light1. (used to move the selected light's position)
 * 
 * is_flat_shading: if true, flat shading is on; else, Gouraud shading is on.
 * 
 * snowman_showup: if true, a character is moving on the terrain; else, character will not show up.
 * 
 * polygon: Drawing terrain using triangles or quads based on varibale `polygon`.
 * 
 * algorithm: Algorithm used to create the terrain. (Circle (by default), Fault Algorithm).
 * 
 */


vector< vector<Point3D> > terrain_points;
vector< vector<float> > points_color;

vector<Snowman> character;

int terrain_size = 200;

double pi = 3.14159265;

int wireFrame = 0;

int texture = 0;

bool lightOn = false;

bool is_zero = true;

bool is_flat_shading = false;

bool is_texture = false;

bool snowman_showup = false;

int polygon = 0;

char* algorithm = "CIRCLE";

/* Three texture */
GLubyte* sea;
GLubyte* mountain;
GLubyte* grass;

/* arrays for image data */
GLuint textures[3];

int width1, height1, max1;
int width2, height2, max2;
int width3, height3, max3;

/*
 * Two sets of light properties
 *
 * Positions of two lights:
 *      - Two lights is above oppsite corners of the terrain.
 *      - The terrain is centered at (0,0,0), so (x, z) value of two corners will be
 *             (terrain_size/2,terrain_size/2) and (-terrain_size/2,-terrain_size/2)
 *
 */
GLfloat light_pos[2][4] = {
    {-terrain_size/2,terrain_size,terrain_size/2,1}, //light0 position
    {terrain_size/2,terrain_size,-terrain_size/2,1} //light1 position
};
GLfloat amb[2][4] = {
    { 1, 1, 1, 1 }, //light0
    { 1, 1, 1, 1 } //light1
};
GLfloat diff[2][4] = {
    { 1, 0, 0, 1 },//light0
    { 0, 0, 1, 1 } //light1
};
GLfloat spec[2][4] = {
    { 1, 1, 1, 1 },//light0
    { 1, 1, 1, 1 } //light1
};


//perspective setup
GLdouble eye[] = {0,0,0}; //eye position will be setted in the init() function
GLdouble lookAt[] = {0,0,0};
GLdouble up[] = {0,1,0};

/*
 * calcHeight Function
 * 
 * Calculate the max height within four points in a face.
 *  - input n, k is the upper left point.
 *  - four points are (n,k), (n+1,k), (n,k+1), (n+1,k+1)
 * 
 */
float calcHeight(int n, int k){
    float max = points_color[n][k];
    
    for(int i = 0; i< 2; i++){
        for(int j = 0; j< 2; j++){
            if(points_color[n+i][k+j] > max){
                max = points_color[n+i][k+j];
            }
        }
    }
    
    return max;
}

/*
 * Initial Quadrilateral Terrain Model
 * 
 * Calculate the surface normal for each face, then draw terrain by using 4 vertices as
 * an independent quadrilateral and set color for the flat (using grey scale color methods)
 */
void DrawTerrainInitMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            // face normal determined by three points
            Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
            //Draw a face in CCW
            glBegin(GL_QUADS);
            glNormal3f(face_normal.mX,face_normal.mY,face_normal.mZ);
            glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
* Quadrilateral Terrain Wireframe Model
*
* Set the color before we draw each face of the terrian, then 
* draw the quadrilateral wireframe terrain by using GL_LINE_LOOP
*/
void drawTerrainWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            // wireframe color: green
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            // Consider with 4 vertices for a face in CCW
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
* Quadrilateral Terrain Model- both wireframe and solid model
* So we just need to combine wireframe function and solid function, then every point in wireframe, we added 0.05 for
* its y-coordinates, because we want to prevent wireframe model and solid model from overlapping.
*/
void DrawTerrainPolyWireMode(){
    //call DrawTerrainInitMode() to draw solid tarrrain
    DrawTerrainInitMode();
    // wireframe model, add 0.05 for every point's y-coordinate
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY+0.05,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY+0.05,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY+0.05,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY+0.05,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
 * DrawTerrainInitTriMode() Function
 * 
 * Triangle Terrain Model - solid model
 * 
 * Similar to the quadrilateral terrain, but we draw a terrain by using GL_TRIANGLES, which we only take 3 vertices.
 * 
 * Two triangle = one quad.
 */
void DrawTerrainInitTriMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            // face normal determined by three points, first one is for first triangle, the second one is for second
            Vec3D face1_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
            Vec3D face2_normal = Vec3D::normal(terrain_points[i+1][j],terrain_points[i][j+1],terrain_points[i+1][j+1]);
            
            glBegin(GL_TRIANGLES);
            // first triangle with face normal 1
            glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
            glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            
            // Second Triangle with face normal 2
            glNormal3f(face2_normal.mX,face2_normal.mY,face2_normal.mZ);
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            
            glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
 * drawTerrainTriWireMode() Function
 * 
 * Triangle Terrain Model - wireframe model
 * 
 * Similar to the quadrilateral terrain, but we draw a terrain by taking only 3 vertices each time.
 * 
 */
void drawTerrainTriWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            // wireframe color: green
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            // first triangle
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            
            // second triangle
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
 * drawTerrainTriWireMode() Function
 * 
 * Triangle Terrain Model - both wireframe model and solid model
 * 
 * Similar to the quadrilateral terrain, combining wireframe model and solid model, and for the wireframe model, we added
 * 0.05 for each y-coordinate
 * 
 */
void DrawTerrainTriPolyWireMode(){
    DrawTerrainInitTriMode();
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){           
            // Wireframe Model
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            //first triangle
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY+0.05,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY+0.05,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY+0.05,terrain_points[i+1][j].mZ);
            //second triangle
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY+0.05,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY+0.05,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY+0.05,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}

/*
 * Draw Terrian's Texture
 * drawing the terrain using the loaded textures
 */
void drawTexture(int txt){
    /*
     * cycle variable(txt) for draw textures
     * 
     * txt % 5 = 0, return the terrain without texture, and disable textures
     * txt % 5 = 1, return the terrain with the first textures: water (or we call it blue)
     * txt % 5 = 2, return the terrain with the second textures: ground (or we call it yellow)
     * txt % 5 = 3, return the terrain with the third textures: grassy (or we call it green)
     * 
     * Additional Feature:
     * When txt % 5 = 4, return the terrain with all three textures, ple, the terrain is loaded
     * with a ground terrain. For mountains after a certain point, grassy textures will be applied.
     * And, for dips lower than “ground zero”, a water texture will be applied.
     */
    if (txt % 5 == 0){//no texture
        glDisable(GL_TEXTURE_2D);
        is_texture = false;
    }else{// enable texture
        glEnable(GL_TEXTURE_2D);
        for(int i = 0; i < terrain_size-1; i++){
            for(int j = 0; j < terrain_size -1; j++){
                float max_height_percent = calcHeight(i,j);
                if(txt % 5 == 1){
                    glBindTexture(GL_TEXTURE_2D, textures[0]);
                }
                else if(txt % 5 == 2){
                    glBindTexture(GL_TEXTURE_2D, textures[1]);
                }
                else if (txt % 5 == 3){
                    glBindTexture(GL_TEXTURE_2D, textures[2]);
                }else if (txt % 5 == 4){ // additional feature
                    if(max_height_percent<0.55){
                        glBindTexture(GL_TEXTURE_2D, textures[0]);
                    }
                    else if(max_height_percent<0.75 && max_height_percent>=0.55){
                        glBindTexture(GL_TEXTURE_2D, textures[1]);
                    }
                    else{
                        glBindTexture(GL_TEXTURE_2D, textures[2]);
                    }
                }
                // determine which type of terrain we want to add texture(quads or triangles)
                // polygam % 2 = 0 is quads
                if (polygon % 2 == 0) {
                    Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
                    glBegin(GL_QUADS);
                    glNormal3f(face_normal.mX,face_normal.mY,face_normal.mZ);
                    
                    glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
                    glTexCoord2f(1, 0);
                    glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
                    
                    glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
                    glTexCoord2f(0, 0);
                    glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
                    
                    glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
                    glTexCoord2f(0, 1);
                    glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);

                    glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
                    glTexCoord2f(1, 1);
                    glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
                    glEnd();
                }else {// polygam % 2 = 1 is triangles
                    
                    // face normal determined by three points, first one is for first triangle, the second one is for second
                    Vec3D face1_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
                    Vec3D face2_normal = Vec3D::normal(terrain_points[i+1][j],terrain_points[i][j+1],terrain_points[i+1][j+1]);
                    glBegin(GL_TRIANGLES);
                    // First Triangle
                    glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
                    glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
                    glTexCoord2f(1, 0);
                    glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
                    
                    glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
                    glTexCoord2f(0, 0);
                    glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
                    
                    glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
                    glTexCoord2f(1, 1);
                    glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
                    
                    // Second Triangle
                    glNormal3f(face2_normal.mX,face2_normal.mY,face2_normal.mZ);
                    glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
                    glTexCoord2f(0, 0);
                    glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
                    
                    glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
                    glTexCoord2f(0, 1);
                    glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
                    
                    glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
                    glTexCoord2f(1, 1);
                    glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
                    glEnd();
                }
            }
        }
        
    }
}

/*
 *
 * Display Function 
 * 
 * Set up the lights
 * 
 * Set up the camera
 * 
 * Call drawTexture if texture mode is on
 * 
 * Call draw Terrain function
 * 
 * Using double buffer
 */
void display(){
    /* Setting light properties*/
    for(int i = 0; i < 2; i++){
        glLightfv(GL_LIGHT0+i,GL_POSITION,light_pos[i]);
        glLightfv(GL_LIGHT0+i,GL_DIFFUSE,diff[i]);
        glLightfv(GL_LIGHT0+i,GL_AMBIENT,amb[i]);
        glLightfv(GL_LIGHT0+i,GL_SPECULAR,spec[i]);
    }
    
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2],up[0],up[1],up[2]);
    
    // is_texture means to check the key "T" pressed or not, if pressed, we draw terrain with texture
    if(is_texture){
        drawTexture(texture);
    }else{
        // which terrain model we want to draw, quads or triangles
        if (polygon % 2 == 0) {
            // draw terrain, or draw wireframe, or draw both
            if (wireFrame % 3 == 0) {
                DrawTerrainInitMode();
            }else if (wireFrame % 3 == 1) {
                drawTerrainWireMode();
            }else if (wireFrame % 3 == 2) {
                DrawTerrainPolyWireMode();
            }
        } else if (polygon % 2 == 1){
            if (wireFrame % 3 == 0) {
                DrawTerrainInitTriMode();
            }else if (wireFrame % 3 == 1) {
                drawTerrainTriWireMode();
            }else if (wireFrame % 3 == 2) {
                DrawTerrainTriPolyWireMode();
            }
        }
    }
    
    // draw snow man if snowman_showup is true
    if(snowman_showup){
        character[0].drawSnowman();
    }
    
    glutSwapBuffers();
}

/*
 * Special key functions
 *
 * 1. Right and Left arrow: rotate camera about y-axis
 *      a. right arrow: rotate about y-axis in CW
 *      b. left arrow: rotate about y-axis in CCW
 *
 * 1. Top and down arrow: rotate camera about x-axis
 *      a. up arrow: rotate about x-axis in CW
 *      b. down arrow: rotate about y-axis in CCW
 *
 */
void special(int key, int x, int y){
    double eye_x = eye[0];
    double eye_y = eye[1];
    double eye_z = eye[2];
    double rotate_angle = 5.0 * pi /180 ;
    switch(key){
        case GLUT_KEY_RIGHT: //right rotate 5 degrees about the y-axis
            eye[0] = eye_x*cos(rotate_angle)+eye_z*sin(rotate_angle);
            eye[2] = eye_z*cos(rotate_angle)-eye_x*sin(rotate_angle);
            std::cout << "Rotate the camera position about y-axis in CW by 5 degrees!\n";
            break;
        case GLUT_KEY_LEFT: //left rotate 5 degrees about the y-axis
            eye[0] = eye_x*cos(-rotate_angle)+eye_z*sin(-rotate_angle);
            eye[2] = eye_z*cos(-rotate_angle)-eye_x*sin(-rotate_angle);
            std::cout << "Rotate the camera position about y-axis in CCW by 5 degrees!\n";
            break;
        case GLUT_KEY_UP: //up rotate 5 degrees about the x-axis
            std::cout << "Rotate the camera position about x-axis in CW by 5 degrees!\n";
            eye[1] = eye_y*cos(-rotate_angle)-eye_z*sin(-rotate_angle);
            eye[2] = eye_y*sin(-rotate_angle)+eye_z*cos(-rotate_angle);
            break;
        case GLUT_KEY_DOWN: //down rotate 5 degrees about the x-axis
            std::cout << "Rotate the camera position about x-axis in CCW by 5 degrees!\n";
            eye[1] = eye_y*cos(rotate_angle)-eye_z*sin(rotate_angle);
            eye[2] = eye_y*sin(rotate_angle)+eye_z*cos(rotate_angle);
            break;
    }
}

/*
 * Reset Function
 * 
 * reset all the global variables, eye position and re-draw terrain
 */
void reset(){
    terrain_points.clear();
    points_color.clear();
    character.clear();
    wireFrame = 0;
    lightOn = false;
    is_zero = true;
    is_texture = false;
    snowman_showup = false;
    polygon = 0;
    algorithm = "CIRCLE";
    texture = 0;
    is_flat_shading = false;
    glDisable(GL_TEXTURE_2D);
    
    // generate a new random terrain using the heightmap generation algorithm
    Heightmap(terrain_points,points_color,terrain_size, algorithm);
    
    //reset eye position
    eye[0] = terrain_size*1.5;
    eye[1] = 50;
    eye[2] = terrain_size*1.5;
    //light position
    light_pos[0][0] = -terrain_size/2;
    light_pos[0][2] = terrain_size/2;
    light_pos[1][0] = terrain_size/2;
    light_pos[1][2] = -terrain_size/2;

    
}

/*
 *  LoadPPM -- loads the specified ppm file, and returns the image data as a GLubyte
 *  (unsigned byte) array. Also returns the width and height of the image, and the
 *  maximum colour value by way of arguments
 *  usage: GLubyte myImg = LoadPPM("myImg.ppm", &width, &height, &max);
 */
GLubyte* LoadPPM(char* file, int* width, int* height, int* max){
    GLubyte* image;
    FILE *f;
    int n, m;
    int  k, nm;
    char c;
    int i;
    char b[100];
    float s;
    int red, green, blue;
    
    f = fopen(file,"r");
    fscanf(f,"%[^\n] ",b);
    
    if(b[0]!='P'|| b[1] != '3')
    {
        printf("%s is not a PPM file!\n",file);
        exit(0);
    }
    printf("%s is a PPM file\n", file);
    fscanf(f, "%c",&c);
    while(c == '#')
    {
        fscanf(f, "%[^\n] ", b);
        printf("%s\n",b);
        fscanf(f, "%c",&c);
    }
    ungetc(c,f);
    fscanf(f, "%d %d %d", &n, &m, &k);
    
    printf("%d rows  %d columns  max value= %d\n",n,m,k);
    
    nm = n*m;
    
    image = (GLubyte*)malloc(3*sizeof(GLuint)*nm);
    
    
    s=255.0/k;
    
    
    for(i=0;i<nm;i++)
    {
        fscanf(f,"%d %d %d",&red, &green, &blue );
        image[3*nm-3*i-3]=red*s;
        image[3*nm-3*i-2]=green*s;
        image[3*nm-3*i-1]=blue*s;
    }
    
    *width = n;
    *height = m;
    *max = k;
    
    return image;
}
/* 
 * Create snowman charater
 */
void createSnowman(){
    character.push_back(Snowman(terrain_points));
}

/*
 * Check the snow man move position, and re-direct if its position out of the terrain range
 */
void checkDir(){
    // Check the position of the snow man whather or not out of the terrain flat
    if(!(character[0].position.mX <= terrain_size/2 -1.5 && character[0].position.mX >= -terrain_size/2 +1.5)){
        character[0].direction.mX = - character[0].direction.mX;
    }
    
    // Check the position of the snow man whather or not reached the max/min height of the terrain
    if(!(character[0].position.mZ <= terrain_size/2 -1.5 && character[0].position.mZ >= -terrain_size/2 +1.5)){
        character[0].direction.mZ = - character[0].direction.mZ;
    }
}

/*
 * Movement of Snowman character
 * 
 * Update the position of the snowman charater
 */
void moveSnowman(){
    checkDir();
    /* update x, z coordinate of the position of the charater */
    character[0].position.mX += character[0].direction.mX;
    character[0].position.mZ += character[0].direction.mZ;
    
    /* update height value of the position of the character */
    float xIndex = character[0].position.mX + terrain_size/2;
    float zIndex = character[0].position.mZ + terrain_size/2;
    
    float height1 = terrain_points[(int)floor(xIndex)][(int)floor(zIndex)].mY;
    float height2 = terrain_points[(int)floor(xIndex+1)][(int)floor(zIndex+1)].mY;
    
    float proportion = sqrt(pow(character[0].position.mX - floor(character[0].position.mX),2)+pow(character[0].position.mZ - floor(character[0].position.mZ),2));
    
    character[0].position.mY = height1 + proportion*(height2-height1);
 
}


/*
 * Initialization Function
 */
void init(void){
    
    printf("Please enter the size of Terrain(number between 50 and 300):\n");
    scanf("%d", &terrain_size);
    /* eye position based on the terrain size entered by the user*/
    eye[0] = terrain_size*1.5;
    eye[1] = 50;
    eye[2] = terrain_size*1.5;
    
    // draw the initial terrain by using circle algorithm
    Heightmap(terrain_points,points_color,terrain_size, algorithm);
    
    //create a snowman charaters;
    createSnowman();
    
    // load textures
    sea = LoadPPM("sea.ppm",&width1, &height1, &max1);
    mountain = LoadPPM("mountain.ppm",&width2, &height2, &max2);
    grass = LoadPPM("grass.ppm",&width3, &height3, &max3);
    
    // operation guide of particles system in terminal.
    std::cout << "Welcome to The Terrain Generator!\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "KEYBOARD commands:\n";
    std::cout << "1. Pressing 'q' or 'ESC' or 'Q' to exit the system.\n";
    std::cout << "2. Pressing the 'W'(upper case latter) can toggle wireframe mode between three options (poly, wireframe or poly-wire).\n";
    std::cout << "3. Pressing the 'S'(upper case latter) can toggle drawing the terrain using triangles or using quads.\n";
    std::cout << "4. Pressing the ‘r’ or ‘R’ key to reset the terrain (will generate a new terrain).\n";
    std::cout << "5. Pressing ‘l’ or ‘L’ will turn the lights on and off.\n";
    std::cout << "6. Pressing 'c' or 'C' will change the algorithm of drawing terrain. Circle (by default) or Fault\n";
    std::cout << "7. Pressing 't' or 'T' will cycle drawing the terrain using the loaded textures. Three simple textures and one is complex (additional feature).\n";
    std::cout << "8. Pressing 'f' or 'F' can toggle between flat shading and Gouraud shading.\n";
    std::cout << "9. Press '0' to change the light mode to Light0, which means users can move the Light0 position using 'adsw'\n";
    std::cout << "10. Press '1' to change the light mode to Light1, which means users can move the Light1 position using 'adsw'\n";
    std::cout << "11. Press 'a' to change the light0(or 1) position along x axis (negative)\n";
    std::cout << "12. Press 'd' to change the light0(or 1) position along x axis (positive)\n";
    std::cout << "13. Press 'w' to change the light0(or 1) position along y axis (positive)\n";
    std::cout << "14. Press 's' to change the light0(or 1) position along y axis (negative)\n";
    std::cout << "15. Press right arrow button to rotate the camera position about y-axis in CW\n";
    std::cout << "16. Press left arrow button to rotate the camera position about y-axis in CCW\n";
    std::cout << "17. Press down arrow button to rotate the camera position about x-axis in CCW\n";
    std::cout << "18. Press up arrow button to rotate the camera position about x-axis in CW\n";
    std::cout << "19. Press 'm' or 'M' to turn on or off the Snowman character mode\n";
    std::cout << "----------------------------------------------------------------\n";
    std::cout << "Other features:\n";
    std::cout << "1. Elevation Texturing. Depending on elevation, different textures will be applied to the terrain. This feature mixed with regular texturing mode. Press 'T' or 't' to show it. \n";
    std::cout << "2. Press 'm' or 'M' to turn on or off the Snowman character mode. A snowman generated with random position within the terrain, and walk around the terrain.\n";
    std::cout << "3. Additional terrain algorithm - Fault Algorithm, which is an additional terrain generation algorithm \n";
    std::cout << "-------------------------------------------------------------\n";
    std::cout << "Have Fun!\n";
    std::cout << std::endl;
}

/*
 * Texturing Function
 */
void texturing(){
    glMatrixMode(GL_TEXTURE);
    
    glGenTextures(3, textures); //three textures
    
    glBindTexture(GL_TEXTURE_2D, textures[0]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width1, height1, 0, GL_RGB, GL_UNSIGNED_BYTE, sea);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, textures[1]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width2, height2, 0, GL_RGB, GL_UNSIGNED_BYTE, mountain);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glBindTexture(GL_TEXTURE_2D, textures[2]);
    
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width3, height3, 0, GL_RGB, GL_UNSIGNED_BYTE, grass);
    
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    
    glClearColor(0, 0, 0, 0);
    glColor3f(1, 1, 1);
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 100);
}

/*
 * Keyboards Operation
 * 
 * Pressing 'q' or 'ESC' or 'Q' to exit the system.
 * 
 * Pressing the 'W'(upper case latter) can toggle wireframe mode between three options (poly, wireframe or poly-wire).
 * 
 * Pressing the 'S'(upper case latter) can toggle drawing the terrain using triangles or using quads.
 * 
 * Pressing the ‘r’ or ‘R’ key to reset the terrain (will generate a new terrain).
 * 
 * Pressing ‘l’ or ‘L’ will turn the lights on and off.
 * 
 * Pressing 'c' or 'C' will change the algorithm of drawing terrain. Circle (by default) or Fault
 * 
 * Pressing 't' or 'T' will cycle drawing the terrain using the loaded textures. Three simple textures and one is complex (additional feature).
 * 
 * Pressing 'f' or 'F' can toggle between flat shading and Gouraud shading.
 * 
 * Press '0' to change the light mode to Light0, which means users can move the Light0 position using 'adsw'
 * 
 * Press '1' to change the light mode to Light1, which means users can move the Light1 position using 'adsw'
 * 
 * Press 'a' to change the light0(or 1) position along x axis (negative)
 * 
 * Press 'd' to change the light0(or 1) position along x axis (positive)
 * 
 * Press 'w' to change the light0(or 1) position along y axis (positive)
 * 
 * Press 's' to change the light0(or 1) position along y axis (negative)
 * 
 * Press right arrow button to rotate the camera position about y-axis in CW
 * 
 * Press left arrow button to rotate the camera position about y-axis in CCW
 * 
 * Press down arrow button to rotate the camera position about x-axis in CCW
 * 
 * Press up arrow button to rotate the camera position about x-axis in CW
 * 
 * Press 'm' or 'M' to turn on or off the Snowman character mode
 * 
 */
void kbd(unsigned char key, int x, int y){
    switch(key){
        case 27:
        case 'q':
        case 'Q':
            std::cout << "Terrain Program closed! \n";
            exit(0);
            break;
        // Pressing the 'W'(upper case latter) can toggle wireframe mode between three options.
        case 'W':
            wireFrame += 1;
            break;
        // Pressing ‘l’ or ‘L’ will toggle the lights on and off.
        case 'l':
        case 'L':
            lightOn = !lightOn;
            if(lightOn){
                std::cout << "Light turn on!\n";
            }
            else{
                std::cout << "Light turn off!\n";
            }
            break;
        // Pressing “0” to choose light 0, you can change the light 0 position.
        case '0':
            is_zero = true;
            std::cout << "Changed to light0!\n";
            break;
        // Pressing “1” to choose light 1, you can change the light 1 position.
        case '1':
            is_zero = false;
            std::cout << "Changed to light1!\n";
            break;
        // Pressing (lowercase letter)'w', 's', 'a', or 'd' to move the light position.
        case 'd':
            if(is_zero){
                light_pos[0][0] += 10;
                std::cout << "Light0 move to right along x-axis!\n";
            }
            else{
                light_pos[1][0] += 10;
                std::cout << "Light1 move to right along x-axis!\n";
            }
            break;
        case 'a':
            if(is_zero){
                light_pos[0][0] -= 10;
                std::cout << "Light0 move to left along x-axis!\n";
            }
            else{
                light_pos[1][0] -= 10;
                std::cout << "Light1 move to left along x-axis!\n";
            }
            break;
        case 'w':
            if(is_zero){
                light_pos[0][1] += 10;
                std::cout << "Light0 move to up along y-axis!\n";
            }
            else{
                light_pos[1][1] += 10;
                std::cout << "Light1 move to up along y-axis!\n";
            }
            break;
        case 's':
            if(is_zero){
                light_pos[0][1] -= 10;
                std::cout << "Light0 move to down along y-axis!\n";
            }
            else{
                light_pos[1][1] -= 10;
                std::cout << "Light1 move to down along y-axis!\n";
            }
            break;
        // Pressing the ‘r’ or ‘R’ key can re-draw terrain.
        case 'r':
        case 'R':
            reset();
            std::cout << "reset!\n";
            break;
        // Pressing 'f' or 'F' can toggle between flat shading and Gouraud shading.
        case 'f':
        case 'F':
            is_flat_shading = !is_flat_shading;
            if(is_flat_shading){
                std::cout << "Changed to Flat Shading Mode!\n";
            }
            else{
                std::cout << "Changed to Gouraud Shading Mode!\n";
            }
            break;
        // Pressing 't' or 'T' will cycle drawing the terrain using the loaded textures.
        case 't':
        case 'T':
            is_texture = true;
            texture += 1;
            if(texture % 5 == 0){
                std::cout << "Normal Mode" << std::endl;
            }
            else{
                std::cout << "Texture Mode " << texture % 5 <<std::endl;
            }
            break;
        // Pressing the 'S'(upper case latter) can toggle drawing the terrain using triangles or using quads.
        case 'S':
            polygon += 1;
            std::cout << "POLYGON CHANGED\n";
            break;
        // Pressing 'c' or 'C' will change the algorithm of drawing terrain.
        case 'c':
        case 'C':
            if (algorithm == "CIRCLE"){
                terrain_points.clear();
                points_color.clear();
                algorithm = "FAULT";
                Heightmap(terrain_points,points_color,terrain_size, algorithm);
            }
                
            else if (algorithm == "FAULT"){
                terrain_points.clear();
                points_color.clear();
                algorithm = "CIRCLE";
                Heightmap(terrain_points,points_color,terrain_size, algorithm);
            }
            break;
        case 'm':
        case 'M':
            snowman_showup = !snowman_showup;
            if(snowman_showup){
                std::cout << "Snowman Character is added into the scene." << std::endl;
            }
            else{
                std::cout << "Snowman Character is removed from the scene." << std::endl;
            }
            break;
        default:
            break;
    }
}

/* 
 * FPS function
 * 
 * If lightOn, then enable the lighting; Else, disable the light
 * 
 * If is_flat_shading, then call glShadeModel(GL_FLAT); else, call glShadeModel(GL_SMOOTH)
 * 
 * If snowman_showup, then call moveSnowman function
 * 
 * Call glutPostRedisplay() continously.
 */
void FPS(int val){
    glutPostRedisplay();
    if(lightOn){
        glEnable(GL_LIGHTING);
        glEnable(GL_LIGHT0);
        glEnable(GL_LIGHT1);
    }
    else{
        glDisable(GL_LIGHTING);
    }
    if(is_flat_shading){
        glShadeModel(GL_FLAT);
    }
    else{
        glShadeModel(GL_SMOOTH);
    }
    if(snowman_showup){
        // snow man movement
        moveSnowman();
    }
    glutTimerFunc(17, FPS, 0);
}

/*
 * Reshape function
 */
void handleReshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

/*
 * CallBack Function
 */
void callbackInit(){
    glutDisplayFunc(display);
    glutReshapeFunc(handleReshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(kbd);
    glutTimerFunc(0, FPS, 0);
}

/*
* main function
*/
int main(int argc, char** argv) {
    init();
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    callbackInit();
    texturing();

    glEnable(GL_DEPTH_TEST);
    glClearColor(0.2,0.2,0.2,0);
    /* using backface culling */
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    
    glutMainLoop();
    
    return 0;
}
