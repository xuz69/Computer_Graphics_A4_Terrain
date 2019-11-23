#ifdef __APPLE__
#  include <OpenGL/gl.h>
#  include <OpenGL/glu.h>
#  include <GLUT/glut.h>
#else
#  include <GL/gl.h>
#  include <GL/glu.h>
#  include <GL/freeglut.h>
#endif

#include "Snowman.hpp"
#include <cstdlib>
#include <cmath>



Snowman::Snowman(vector<vector<Point3D> > points){
    this->size = 1;

    this->speed = 0.5;


    int ran_x = rand() % points.size();
    int ran_z = rand() % points.size();

    this->position = points[ran_x][ran_z];
    //this->position = Point3D();
    float ran_dir_x = ((float)rand()/(float)RAND_MAX)*2 - 1 ;
    float ran_dir_z = ((float)rand()/(float)RAND_MAX)*2 - 1 ;
    this->direction = Vec3D(ran_dir_x,0,ran_dir_z).normalize().multiply(this->speed);
};

void Snowman::drawSnowman(){
    glTranslatef(0,this->size,0);
    //glScalef(5,5,5);

    //body
    glPushMatrix();
        glColor3f(1,1,1);
        glTranslatef(this->position.mX, this->position.mY, this->position.mZ);
        glutSolidSphere(1, 10, 10);

        //buttons
        glColor3f(0,0,0);
        // middle button
        glPushMatrix();
            glTranslatef(0,0,1);
            glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
        // top button
        glPushMatrix();
        glTranslatef(0,0.4,0.9);
            glutSolidSphere(0.1, 10, 10);
        glPopMatrix();
        // down button
        glPushMatrix();
            glTranslatef(0,-0.4,0.9);
            glutSolidSphere(0.1, 10, 10);
        glPopMatrix();

        //head
        glColor3f(1,1,1);
        glPushMatrix();
            glTranslatef(0,1,0);
            glutSolidSphere(0.7, 10, 10);

            //eyes
            glColor3f(0,0,0);
            glPushMatrix();
                glTranslatef(-0.25,0.3,0.6);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();
            glPushMatrix();
                glTranslatef(0.25,0.3,0.6);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();
            //nose
            glColor3f(1,0.5,0);
            glPushMatrix();
                glTranslatef(0,0,0.65);
                glutSolidSphere(0.1, 10, 10);
            glPopMatrix();
        glPopMatrix();

    glPopMatrix();



};