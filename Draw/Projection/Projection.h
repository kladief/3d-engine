#ifndef DRAW_H
#define DRAW_H
#define _USE_MATH_DEFINES
#define ANGLE 45
#define WINDOW 400
#define COS(a) cos((float)a/(float)180*(float)M_PI)
#define SIN(a) sin((float)a/(float)180*(float)M_PI)
#define TAN(a) tan((float)a/(float)180*(float)M_PI)
#include <cmath>
#include <functional>
#include <windows.h>
#include <vector>
#include "D:\c++\paint\3d engine\CoordStructures.h"

struct angle{
    int _1;
    int _2;
    void operator+=(angle in){
        this->_1+=in._1;
        this->_2+=in._2;
    }
    angle operator-(angle in){
        angle toReturn;
        toReturn._1=this->_1-in._1;
        toReturn._2=this->_2-in._2;
        return toReturn;
    }
};

struct polyAngles{
    angle _1;
    angle _2;
    angle _3;
    polyAngles operator-(polyAngles in){
        polyAngles toReturn;
        toReturn._1=this->_1-in._1;
        toReturn._2=this->_2-in._2;
        toReturn._3=this->_3-in._3;
        return toReturn;
    }
    polyAngles operator-(angle in){
        polyAngles toReturn;
        toReturn._1=this->_1-in;
        toReturn._2=this->_2-in;
        toReturn._3=this->_3-in;
        return toReturn;
    }
};
angle getAngle(double ,double ,double );

struct poly{
private:
    polyAngles angles;
    double distance_to_poly;
public:

    COORD_3_POINT p1;
    COORD_3_POINT p2;
    COORD_3_POINT p3;
    poly(){};
    poly(COORD_3_POINT a,COORD_3_POINT b,COORD_3_POINT c){
        p1 = a;
        p2 = b;
        p3 = c;
    };
    void updateAngles(COORD_3_POINT); 
    polyAngles getAngles();
};
class polyProcessing{
private:
    angle cumRay={180,180};
    bool inView(angle*);
    void leadAngle(angle*);
public:
    struct mesh{
        private:
            int _pointsNum;
            POINT* _points;
        public:
            mesh(POINT* points,int pointsNum):_points(points),_pointsNum(pointsNum){}
            POINT* getPoint();
            POINT* getAllPoint();
            ~mesh();
    }; 
    angle updateCumAngle(angle);
    mesh** getTriangles(polyAngles*,int);
    mesh* getTriangles(polyAngles);
};
#endif