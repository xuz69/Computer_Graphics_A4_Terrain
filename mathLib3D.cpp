#include <math.h>
#include "mathLib3D.hpp"

Point3D::Point3D(float inX, float inY, float inZ){
    this->mX = inX;
    this->mY = inY;
    this->mZ = inZ;
}

Point3D::Point3D(){
    this->mX = 0.0;
    this->mY = 0.0;
    this->mZ = 0.0;
}

float Point3D::distanceTo(Point3D other){
    return sqrt(pow(other.mX-this->mX, 2) + pow(other.mY-this->mY, 2) + pow(other.mZ-this->mZ, 2));
}

float Point3D::fastDistanceTo(Point3D other){
    return pow(other.mX-this->mX, 2) + pow(other.mY-this->mY, 2) + pow(other.mZ-this->mZ, 2);
}

Vec3D::Vec3D(float inX, float inY, float inZ){
    this->mX = inX;
    this->mY = inY;
    this->mZ = inZ;
}

Vec3D::Vec3D(){
    this->mX = 0.0;
    this->mY = 0.0;
    this->mZ = 0.0;
}

float Vec3D::length(){
    return sqrt(pow(this->mX , 2) + pow(this->mY , 2) + pow(this->mZ , 2));
}

Vec3D Vec3D::normalize(){
    if(this->length() != 0){
        return Vec3D(this->mX/this->length(), this->mY/this->length(), this->mZ/this->length());
    }
    else{
        return Vec3D();
    }
}

Vec3D Vec3D::multiply(float scalar){
    return Vec3D(this->mX * scalar, this->mY * scalar, this->mZ * scalar);
}

Point3D Vec3D::movePoint(Point3D source){
    return Point3D(source.mX+this->mX, source.mY+this->mY, source.mZ+this->mZ);
}

Vec3D Vec3D::add(Vec3D other){
    return Vec3D(this->mX+other.mX, this->mY+other.mY, this->mZ+other.mZ);
}


Vec3D Vec3D::createVector(Point3D p1, Point3D p2){
    return Vec3D(p2.mX-p1.mX, p2.mY-p1.mY, p2.mZ-p1.mZ);
}

Vec3D Vec3D::crossProduct(Vec3D p, Vec3D q){
    return  Vec3D(p.mY * q.mZ - p.mZ * p.mY,
                 -p.mX * q.mZ + p.mZ * q.mX,
                 p.mX * q.mY - p.mY * q.mX).normalize();
}

Vec3D Vec3D::normal(Point3D a, Point3D b, Point3D c){
    Vec3D x = Vec3D::createVector(a, b);
    Vec3D y = Vec3D::createVector(a, c);
    
    return Vec3D::crossProduct(x, y);
}
