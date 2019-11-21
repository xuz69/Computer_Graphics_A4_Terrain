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


int terrain_size = 50;
double pi = 3.14159265;


//perspective setup
GLdouble eye[] = { terrain_size+50, 50, terrain_size+50};
GLdouble lookAt[] = { 0 , 0, 0 };
GLdouble up[] = { 0, 1, 0 };

void createHeightmap(){
    Heightmap(terrain_points,points_color,terrain_size);
}

void drawTerrain(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            glColor3f(0.0f, 1.0f, 0.0f);
            glBegin(GL_LINE_LOOP);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glEnd();
        }
    }
}

void initDrawTerrain(){
    for(int i = 0; i < terrain_size-1; i++){
        for(int j = 0; j < terrain_size -1; j++){
            glBegin(GL_QUADS);
            glColor3f(points_color[i][j],points_color[i][j],points_color[i][j]);
            glVertex3f(terrain_points[i][j].mX,terrain_points[i][j].mY,terrain_points[i][j].mZ);
            glColor3f(points_color[i+1][j],points_color[i+1][j],points_color[i+1][j]);
            glVertex3f(terrain_points[i+1][j].mX,terrain_points[i+1][j].mY,terrain_points[i+1][j].mZ);
            glColor3f(points_color[i+1][j+1],points_color[i+1][j+1],points_color[i+1][j+1]);
            glVertex3f(terrain_points[i+1][j+1].mX,terrain_points[i+1][j+1].mY,terrain_points[i+1][j+1].mZ);
            glColor3f(points_color[i][j+1],points_color[i][j+1],points_color[i][j+1]);
            glVertex3f(terrain_points[i][j+1].mX,terrain_points[i][j+1].mY,terrain_points[i][j+1].mZ);
            glEnd();
        }
    }
}

void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    gluLookAt(eye[0], eye[1], eye[2], lookAt[0], lookAt[1], lookAt[2],up[0],up[1],up[2]);
    
    initDrawTerrain();
    
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

void FPS(int val){
    glutPostRedisplay();
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
    glutTimerFunc(0, FPS, 0);
}

int main(int argc, char** argv) {
    createHeightmap();
    glutInit(&argc, argv);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(300,300);
    glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
    glutCreateWindow("Terrain");
    
    callbackInit();
    
    glEnable(GL_DEPTH_TEST);
    /* using backface culling
     glFrontFace(GL_CW);
     glEnable(GL_CULL_FACE);
     glCullFace(GL_BACK);*/
    
    
    glutMainLoop();
    
    return 0;
}
