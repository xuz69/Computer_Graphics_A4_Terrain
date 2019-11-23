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

vector< vector<Point3D> > terrain_points;
vector< vector<float> > points_color;


int terrain_size = 200;

double pi = 3.14159265;

int wireFrame = 0;

bool lightOn = false;

bool is_zero = true;

bool is_flat_shading = false;

bool is_texture = false;

int polygan = 0;

//arrays for image data
GLubyte* sea;
GLubyte* mountain;
GLubyte* grass;

GLuint textures[3];

int width1, height1, max1;
int width2, height2, max2;
int width3, height3, max3;

/*
 * Two sets of light properties
 *
 * Positions of two lights:
 *      - Two lights is above oppsite corners of the terrain.
 *      - The terrain is centered at (0,0,0), so (x, z) value of two corners will be (terrain_size/2,terrain_size/2) and (-terrain_size/2,-terrain_size/2)
 *
 */
GLfloat light_pos[2][4] = {
    {terrain_size/2,50,terrain_size/2,1}, //light0 position
    {-terrain_size/2,50,-terrain_size/2,1} //light1 position
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


void DrawTerrainInitMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]); // face normal determined by three points
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

void drawTerrainWithTexture(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            float max_height_percent = calcHeight(i,j);
            Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]); // face normal determined by three points
            if(max_height_percent<0.55){
                glBindTexture(GL_TEXTURE_2D, textures[0]);
            }
            else if(max_height_percent<0.75 && max_height_percent>=0.55){
                glBindTexture(GL_TEXTURE_2D, textures[1]);
            }
            else{
                glBindTexture(GL_TEXTURE_2D, textures[2]);
            }
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
        }
    }
}

void drawTerrainWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glNormal3f(face_normal.mX,face_normal.mY,face_normal.mZ);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}


void DrawTerrainPolyWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]);
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

void DrawTerrainInitTriMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face1_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]); // face normal determined by three points
            Vec3D face2_normal = Vec3D::normal(terrain_points[i+1][j],terrain_points[i][j+1],terrain_points[i+1][j+1]); // face normal determined by three points
            
            glBegin(GL_TRIANGLES);
            glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
            glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            
            
            // Second Triangle
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


void drawTerrainTriWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face1_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]); // face normal determined by three points
            Vec3D face2_normal = Vec3D::normal(terrain_points[i+1][j],terrain_points[i][j+1],terrain_points[i+1][j+1]); // face normal determined by three points
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            
            glNormal3f(face2_normal.mX,face2_normal.mY,face2_normal.mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}


void DrawTerrainTriPolyWireMode(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            Vec3D face1_normal = Vec3D::normal(terrain_points[i][j],terrain_points[i][j+1],terrain_points[i+1][j]); // face normal determined by three points
            Vec3D face2_normal = Vec3D::normal(terrain_points[i+1][j],terrain_points[i][j+1],terrain_points[i+1][j+1]); // face normal determined by three points
            glBegin(GL_TRIANGLES);
            glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
            glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            
            
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            
            
            // Second Triangle
            glNormal3f(face2_normal.mX,face2_normal.mY,face2_normal.mZ);
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            
            glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glEnd();
            
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glNormal3f(face1_normal.mX,face1_normal.mY,face1_normal.mZ);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY+0.05,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY+0.05,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY+0.05,terrain_points[i+1][j].mZ);
            
            glNormal3f(face2_normal.mX,face2_normal.mY,face2_normal.mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY+0.05,terrain_points[i][j+1].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY+0.05,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY+0.05,terrain_points[i+1][j].mZ);
            glEnd();
        }
    }
}


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
    if(is_texture){
        drawTerrainWithTexture();
    }
    else{
        if (polygan % 2 == 0) {
            if (wireFrame % 3 == 0) {
                DrawTerrainInitMode();
            }else if (wireFrame % 3 == 1) {
                drawTerrainWireMode();
            }else if (wireFrame % 3 == 2) {
                DrawTerrainPolyWireMode();
            }
        } else if (polygan % 2 == 1){
            if (wireFrame % 3 == 0) {
                DrawTerrainInitTriMode();
            }else if (wireFrame % 3 == 1) {
                drawTerrainTriWireMode();
            }else if (wireFrame % 3 == 2) {
                DrawTerrainTriPolyWireMode();
            }
        }
    }
    
    glutSwapBuffers();
}

/*
 * Special key functions
 *
 * 1. Right and Left arrow: rotate about y-axis
 *      a. right arrow: rotate about y-axis in CW
 *      b. left arrow: rotate about y-axis in CCW
 *
 * 1. Top and down arrow: rotate about x-axis
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
 */
void reset(){
    terrain_points.clear();
    points_color.clear();
    wireFrame = 0;
    lightOn = false;
    is_zero = true;
    is_texture = false;
    polygan = 0;
    
    Heightmap(terrain_points,points_color,terrain_size);// generate a new random terrain using the heightmap generation algorithm
    
    eye[0] = terrain_size*1.5;
    eye[1] = 50;
    eye[2] = terrain_size*1.5;
    
    
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


void init(void){
    
    printf("Welcome to Terrain Generator!\n");
    
    printf("Please enter the size of Terrain(number between 50 and 300):\n");
    scanf("%d", &terrain_size);
    /* eye position based on the terrain size entered by the user*/
    eye[0] = terrain_size*1.5;
    eye[1] = 50;
    eye[2] = terrain_size*1.5;
    
    Heightmap(terrain_points,points_color,terrain_size);
    
    sea = LoadPPM("sea.ppm",&width1, &height1, &max1);
    mountain = LoadPPM("mountain.ppm",&width2, &height2, &max2);
    grass = LoadPPM("grass.ppm",&width3, &height3, &max3);
    
}

void texturing(){
    glMatrixMode(GL_TEXTURE);
    
    glGenTextures(3, textures);
    
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

void kbd(unsigned char key, int x, int y){
    switch(key){
        case 27:
        case 'q':
        case 'Q':
            std::cout << "Terrain Program closed! \n";
            exit(0);
            break;
        case 'W':
            wireFrame += 1;
            break;
        case 'L':
        case 'l':
            lightOn = !lightOn;
            if(lightOn){
                std::cout << "Light turn on!\n";
            }
            else{
                std::cout << "Light turn off!\n";
            }
            break;
        case '0':
            is_zero = true;
            std::cout << "Changed to light0!\n";
            break;
        case '1':
            is_zero = false;
            std::cout << "Changed to light1!\n";
            break;
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
        case 'r':
        case 'R':
            reset();
            break;
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
        case 'T':
            is_texture = !is_texture;
            if(is_texture){
                glEnable(GL_TEXTURE_2D);
                std::cout << "Texture Mode!\n";
            }
            else{
                glDisable(GL_TEXTURE_2D);
                std::cout << "Normal Mode!\n";
            }
            break;
        case 'S':
            polygan += 1;
            std::cout << "POLYGAN CHANGED\n";
            break;
        default:
            break;
    }
}





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
    glutTimerFunc(17, FPS, 0);
}

/* Reshape function */
void handleReshape(int w, int h) {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1, 1, 1000);
    
    glMatrixMode(GL_MODELVIEW);
    glViewport(0, 0, w, h);
}

void callbackInit(){
    glutDisplayFunc(display);
    glutReshapeFunc(handleReshape);
    glutSpecialFunc(special);
    glutKeyboardFunc(kbd);
    glutTimerFunc(0, FPS, 0);
}

int main(int argc, char** argv) {
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    
    callbackInit();
    
    glEnable(GL_DEPTH_TEST);
    init();
    texturing();
    /* using backface culling */
    glFrontFace(GL_CCW);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    
    
    glutMainLoop();
    
    return 0;
}
