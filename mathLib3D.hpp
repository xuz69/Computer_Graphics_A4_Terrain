#ifndef mathLib3D_hpp
#define mathLib3D_hpp

#include <stdio.h>

class Point3D {
public:
    Point3D();
    Point3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;

    float distanceTo(Point3D other);
    float fastDistanceTo(Point3D other);
};

class Vec3D {
public:
    Vec3D();
    Vec3D(float inX, float inY, float inZ);
    float mX;
    float mY;
    float mZ;

    float length();
    Vec3D normalize();
    Vec3D multiply(float scalar);
    Point3D movePoint(Point3D source);
    Vec3D add(Vec3D other);

    static Vec3D createVector(Point3D p1, Point3D p2);
};

#endif